# libFuzzer

LibFuzzer is an in-process, coverage-guided, evolutionary fuzzing engine.

LibFuzzer is linked with the library under test, and feeds fuzzed inputs to the library via a specific fuzzing entry point (aka “target function”); the fuzzer then tracks which areas of the code are reached, and generates mutations on the corpus of input data to maximize the code coverage.


## Usage Breakdown

The first task with using libFuzzer on a library is to implement a fuzz target. The fuzz target is a function that accepts inputs and uses those for target tests.

An example fuzz target looks like the following:

```
// fuzz_target.cc
extern "C" int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
  DoSomethingInterestingWithMyAPI(Data, Size);
  return 0;  // Values other than 0 and -1 are reserved for future use.
}
```

The created fuzz target must tolerate characteristics such as accepting any type of input, not exiting on any input, being deterministic and executing quickly with minimum setup.

Since version 6.0, libFuzzer has been included in Clang and requires no additional installation.

To build a binary, use the ```-fsanitize=fuzzer``` flag during the compilation and linking. See the example below:
 
```
clang -g -O1 -fsanitize=fuzzer mytarget.c # Builds the fuzz target.
```

Clang also provides tools that can help identify vulnerabilities. AddressSanitizer is a fast memory error detector. This tool detects the following types of bugs:

* Out-of-bounds accesses to heap, stack and globals
* Use-after-free
* Use-after-return (clang flag -fsanitize-address-use-after-return=(never|runtime|always) default: runtime)
* Enable with: ASAN_OPTIONS=detect_stack_use_after_return=1 (already enabled on Linux).
* Disable with: ASAN_OPTIONS=detect_stack_use_after_return=0.
* Use-after-scope (clang flag -fsanitize-address-use-after-scope)
* Double-free, invalid free
* Memory leaks (experimental)

To build a binary with different Sanitizers, the full list is provided below:

```
clang -g -O1 -fsanitize=fuzzer,address                 mytarget.c # Builds the fuzz target with ASAN
clang -g -O1 -fsanitize=fuzzer,signed-integer-overflow mytarget.c # Builds the fuzz target with a part of UBSAN
clang -g -O1 -fsanitize=fuzzer,memory                  mytarget.c # Builds the fuzz target with MSAN
```

Other Sanitizers that are available include ```UndefinedBehaviorSanitizer``` and ```MemorySanitizers```. Each provides detection capabilities.


## Fuzzing methods 

Let's try libFuzzer on a test library that was referenced previously in the ```2_Harness_Building``` section. Review the source code ```hi_trap.cpp``` below:

```
#include <stdint.h>
#include <stddef.h>
extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
  if (size > 0 && data[0] == 'H')
    if (size > 1 && data[1] == 'I')
       if (size > 2 && data[2] == '!')
       __builtin_trap();
  return 0;
}
```

In this example, we will walk through how libFuzzer triggers the crash statement in this program.

To start we will compile this program with Clang. We will make sure to specify the fuzzer option.

```
clang++ -g -O1 -fsanitize=fuzzer -o hi_trap.run hi_trap.cpp 
```
* -g - Generates debugging information 
* -O1 - Selects the code optimization strategies that will be used for the executable
* -fsanitize=fuzzer - This flag specifies that we would like to use libFuzzer
* -o - This flag specifies the name of the generated executable


To then run the executable:

```
./hi_trap.run
```

This will then run the file and identify the line that causes a crash. In the current example, the output file is also shown.

```
[afl++ aa2558fbcefc] /src/fuzzing_tutorial/2_Fuzzing_Tools/libFuzzer (main) # ./hi_trap.run
INFO: Seed: 3975666929
INFO: Loaded 1 modules   (8 inline 8-bit counters): 8 [0x4eb020, 0x4eb028),
INFO: Loaded 1 PC tables (8 PCs): 8 [0x4c55a0,0x4c5620),
INFO: -max_len is not provided; libFuzzer will not generate inputs larger than 4096 bytes
INFO: A corpus is not provided, starting from an empty corpus
#2      INITED cov: 2 ft: 2 corp: 1/1b exec/s: 0 rss: 24Mb
#519    NEW    cov: 3 ft: 3 corp: 2/3b lim: 8 exec/s: 0 rss: 24Mb L: 2/2 MS: 2 ChangeByte-InsertByte-
#576    NEW    cov: 4 ft: 4 corp: 3/4b lim: 8 exec/s: 0 rss: 24Mb L: 1/2 MS: 2 CopyPart-EraseBytes-
#2595   NEW    cov: 5 ft: 5 corp: 4/8b lim: 25 exec/s: 0 rss: 24Mb L: 4/4 MS: 4 CopyPart-ChangeASCIIInt-ChangeByte-ChangeBit- #2637   REDUCE cov: 5 ft: 5 corp: 4/7b lim: 25 exec/s: 0 rss: 24Mb L: 3/3 MS: 2 ChangeByte-EraseBytes-
#2638   REDUCE cov: 6 ft: 6 corp: 5/9b lim: 25 exec/s: 0 rss: 24Mb L: 2/3 MS: 1 EraseBytes-
==25552== ERROR: libFuzzer: deadly signal
.
.
artifact_prefix='./'; Test unit written to ./crash-7a8dc3985d2a90fb6e62e94910fc11d31949c348
```

Looking at the contents of the file, we see the following output:

```
[afl++ aa2558fbcefc] /src/fuzzing_tutorial/2_Fuzzing_Tools/libFuzzer (main) # cat crash-7a8dc3985d2a90fb6e62e94910fc11d31949c348
HI!
```


## Input Options

After a file is compiled there are options to provide seed values for libFuzzer. Some options allow tests cases in a corpus to be minimized to efficiently create new test cases. We will follow this with another example. Review the source code ```pop.cpp``` below. This is the same example from the AFL section, but with the required libFuzzer function.

```
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>
#include <stddef.h>

using namespace std;

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {

  
  if (sizeof(data) >= 4){

    // Test to find the input to get to the assert statement
    if(data[0] == 'p') {
      if(data[1] == 'o') {
        if(data[2] =='p') {
          if(data[3] == '!') {
              assert(0);
          }
        }
      }
    }
  }
  
  
  return 0;
}
```

We then compile pop.cpp and provide test cases to be used with the resulting executable ```pop.run```:

```
clang++ -g -O1 -fsanitize=fuzzer -o pop.run pop.cpp
``` 

The input seed values that were used are shown below:

```
[afl++ 88712b9985ba] /src/fuzzing_tutorial/2_Fuzzing_Tools/libFuzzer (main) # cat in/test1 in/test2 in/test3
pale
pppp
aaaa
```

To generate these input files the following command can be utilized:

```
echo pale > in/test1 && echo pppp > in/test2 && echo aaaa > in/test3
```

The command below will fuzz the target application while using the seed files provided from the ```in``` directory.

```
./pop.run -i in/
```


## Library to Test

To test libraries a harness must be created to do this properly. The harnesses use libraries that have been compiled by ```clang++```. The harness can then test different components of the library. libFuzzer provides inputs to test the input.

We have created a test library and we will build a harness to test the provided function. The library is shown below:

#### printchars.h
```
#include <iostream>
#include <stdio.h>
#include<string>
#include <assert.h>

using namespace std;

void printchars(std::string str){
  

  if (str.length() >= 16){
    if (str[15] == 'z'){

                  assert(0); 
          }

  
    for(int i=0;i<str.length()-1;i++){
      cout<<str[i];
    }

  }
  
}
```

In the above function, a crash occurs when a specific character is found in the 16th position of the string. In the harness below we show how we will use ```printchars.h```.

```
#include <stdio.h>
#include<string>

#include "printchar.h"
#include <stdint.h>
#include <stddef.h>

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
  
  
  std::string s(reinterpret_cast<const char *>(data), size);
  
  printchars(s);

  return 0;
}
```

From this harness, we can now compile, test, and identify any potential problems in the program. We compile the program as follows:

```
clang++ -g -O1 -fsanitize=fuzzer,address printchars_driver.cpp -o driver
```

* -g - Provides source-level debugging information
* -O1 - Sets compiler optimizations for the produced executable
* -fsanitize=fuzzer,address - This option specifies that libFuzzer and the AddressSanitizer are used. (AddressSanitizer is a memory error detector: https://clang.llvm.org/docs/AddressSanitizer.html) 
* -o driver - This sets the name of the produced executable.

Running the executable can then be done to determine if there are any vulnerabilities that libFuzzer was able to find:

```
[afl++ d1e1c28c171e] /src/fuzzing_tutorial/2_Fuzzing_Tools/libFuzzer (main) # ./driver
INFO: Seed: 2254693854
INFO: Loaded 1 modules   (47 inline 8-bit counters): 47 [0x5a9fa0, 0x5a9fcf),
INFO: Loaded 1 PC tables (47 PCs): 47 [0x571248,0x571538),
INFO: -max_len is not provided; libFuzzer will not generate inputs larger than 4096 bytes
INFO: A corpus is not provided, starting from an empty corpus
#2      INITED cov: 17 ft: 18 corp: 1/1b exec/s: 0 rss: 27Mb
.
.
ERROR: AddressSanitizer: heap-buffer-overflow
.
.
artifact_prefix='./'; Test unit written to ./crash-e7d7112dcf8805e43dc4dd2f027af65719f39266
```

libFuzzer was able to identify the ```assert(0)``` statement in ```printchars.h```. The input that caused the crash is saved to the file ```./crash-e7d7112dcf8805e43dc4dd2f027af65719f39266```.
