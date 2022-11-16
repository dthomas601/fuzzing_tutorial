# American Fuzzy Lop (AFL) ++

AFL++ is a fuzzing tool that leverages instrumentation-guided genetic algorithms to identify vulnerabilities in software.

This code is based on AFL, which was created by Michael Zelwaski. This tool has been effective at finding a variety of software bugs.

AFL provides capabilities for finding vulnerabilities, minimizing test cases, optimizing processing time, etc. It can also fuzz binary-only, network services, and GUI programs.

In the Dockerfile provided in this repo, AFL++ has been already been installed.

AFL++ Documentation: https://aflplus.plus/docs/
AFL++ Quickstart: https://github.com/AFLplusplus/AFLplusplus#quick-start-fuzzing-with-afl


## Usage Breakdown

One of the first decisions that will need to be made is which AFL++ compiler will be used to instrument the target. AFL++ comes with a central compiler ```afl-cc```. The AFL++ documentation provides a breakdown of which compilers to use:

```
+--------------------------------+
| clang/clang++ 11+ is available | --> use LTO mode (afl-clang-lto/afl-clang-lto++)
+--------------------------------+     see [instrumentation/README.lto.md](instrumentation/README.lto.md)
    |
    | if not, or if the target fails with LTO afl-clang-lto/++
    |
    v
+---------------------------------+
| clang/clang++ 3.8+ is available | --> use LLVM mode (afl-clang-fast/afl-clang-fast++)
+---------------------------------+     see [instrumentation/README.llvm.md](instrumentation/README.llvm.md)
    |
    | if not, or if the target fails with LLVM afl-clang-fast/++
    |
    v
 +--------------------------------+
 | gcc 5+ is available            | -> use GCC_PLUGIN mode (afl-gcc-fast/afl-g++-fast)
 +--------------------------------+    see [instrumentation/README.gcc_plugin.md](instrumentation/README.gcc_plugin.md) and
                                       [instrumentation/README.instrument_list.md](instrumentation/README.instrument_list.md)
    |
    | if not, or if you do not have a gcc with plugin support
    |
    v
   use GCC mode (afl-gcc/afl-g++) (or afl-clang/afl-clang++ for clang)
```

## Fuzzing methods used by AFL

Let's try out AFL++ on a test file. See the program pop.cpp provided below:

```
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


using namespace std;


int main() {

        string data;

        cout << "enter input string: ";
        getline(cin, data);

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
From reviewing the code there is a certain input that will trigger the assert statement. We will use AFL to determine this input string.

To start, the program should be compiled using AFL compilers. In this example we will use ```afl-clang-lto++```.

#### pop.cpp compilation
```
[afl++ 87d3227744fd] # afl-clang-lto++ pop.cpp -o pop.run
afl-cc++4.02c by Michal Zalewski, Laszlo Szekeres, Marc Heuse - mode: LLVM-LTO-PCGUARD
afl-llvm-lto++4.02c by Marc "vanHauser" Heuse <mh@mh-sec.de>
AUTODICTIONARY: 1 string found
[+] Instrumented 2 locations (0 selects) without collisions (0 collisions have been avoided) (non-hardened mode).
```

This creates an instrumented executable, that AFL can use to track the coverage of the program as inputs are provided. From this point, we will set up the execution of AFL.

#### What is an input corpus?

AFL takes seed input values that can be mutated and will eventually be used to evolve and discover inputs that will generate a crash. The input to this program will be provided from user input via the ```cin``` function. AFL then monitors this to see if the coverage of the program increases as different inputs are tested. Code coverage is a metric that can help you understand how much of your source has been tested. 

#### Corpus creation

This section will walk through how to create initial inputs that AFL will use to generate input values. Start by making an input and output directory for AFL.

```
[afl++ 87d3227744fd] # mkdir in out
```

We then create a few input files that AFL can build from:

```
[afl++ 87d3227744fd] # echo hello > in/input1
[afl++ 87d3227744fd] # echo crash > in/input2
```

## Execution Command Line

```
afl-fuzz -i in/ -o output/ pop.run
```

* afl-fuzz - This program takes a binary and attempts a variety of fuzzing strategies, paying close attention to how they affect the execution path. To operate correctly, the fuzzer requires one or more starting files containing the typical input formatexpected by the targeted application.

* -i - This option points to a path where input corpus files are located.

* -o - This option points to the output directory where the result from AFL will be saved

* pop.run - This is the name of the executable that will be fuzzed. It was created in the above sections.


## AFL Tool Dashboard

When the tool has been executed, a menu is shown on the screen that outlines what is happening. In this section, we will provide an overview of the main aspects to be aware of. The full breakdown of the dashboard is discussed here: https://github.com/AFLplusplus/AFLplusplus/blob/stable/docs/afl-fuzz_approach.md#understanding-the-status-screen




<div align="center">Example AFL Dashboard </div>


![image](https://github.com/dthomas601/fuzzing_tutorial/blob/main/images/afl_dashboard.PNG)

At the top of the image, the tool specifies the version of AFL that is running and the file that is currently being tested.

##### Processing Time

```
  +----------------------------------------------------+
  |        run time : 0 days, 8 hrs, 32 min, 43 sec    |
  |   last new find : 0 days, 0 hrs, 6 min, 40 sec     |
  | last uniq crash : none seen yet                    |
  |  last uniq hang : 0 days, 1 hrs, 24 min, 32 sec    |
  +----------------------------------------------------+
```

##### Overall results

```
  +-----------------------+
  |  cycles done : 0      |
  |  total paths : 2095   |
  | uniq crashes : 0      |
  |   uniq hangs : 19     |
  +-----------------------+
```

This section tells you how long the fuzzer has been running and how much time has elapsed since its most recent finds. This is broken down into "paths" (a shorthand for test cases that trigger new execution patterns), crashes, and hangs.


##### Cycle progress
```
  +-------------------------------------+
  |  now processing : 1296 (61.86%)     |
  | paths timed out : 0 (0.00%)         |
  +-------------------------------------+

```

This box tells you how far along the fuzzer is with the current queue cycle: it shows the ID of the test case it is currently working on, plus the number of inputs it decided to ditch because they were persistently timing out.


##### Stage progress

```
 +-------------------------------------+
  |  now trying : interest 32/8         |
  | stage execs : 3996/34.4k (11.62%)   |
  | total execs : 27.4M                 |
  |  exec speed : 891.7/sec             |
  +-------------------------------------+
```

The remaining fields should be fairly self-evident: there's the exec count progress indicator for the current stage, a global exec counter, and a benchmark for the current program execution speed. This may fluctuate from one test case to another, but the benchmark should be ideally over 500 execs/sec most of the time - and if it stays below 100, the job will probably take very long.

The fuzzing process will continue until you press Ctrl-C. At a minimum, you want to allow the fuzzer to complete one queue cycle, which may take anywhere from a couple of hours to a week or so.


## AFL Output Directory

There are three subdirectories created within the output directory and updated in real time:

* crashes (directory) - Inside of the ```crashes``` directory will be ```README.txt``` that outlines what command was used to generate crashes. Also included are unique test cases that caused the tested program to receive a fatal signal (e.g., SIGSEGV, SIGILL, SIGABRT). The entries are grouped by the received signal.

* hangs (directory) -  Unique test cases that cause the tested program to time out. The default time limit before something is classified as a hang is the larger of 1 second and the value of the -t parameter. The value can be fine-tuned by setting AFL_HANG_TMOUT, but this is rarely necessary.

* queue (directory) - Test cases for every distinctive execution path, plus all the starting files given by the user. This is the synthesized corpus.

The test cases from each of these directories can then be tested against the program outside of the AFL fuzzing campaign to ensure the cases induce the described error.


## AFL Testcase Minimization

A key tool that is used once AFL is run on a target application is to minimize the size of the test case input and still generate the associated crash. This is done by using the utility ```afl-tmin```. The usage for the tool can be found with the following command ```afl-tmin --help```.

We will apply this to the example used earlier. The crashes for example are located in ```output/default/crashes```. We will use the following command to minimize the input that caused a crash:

```
afl-tmin -i output/default/crashes/id:000000,sig:0c:000005+000001,time:106200,execs:103046,op:splice,rep:2.txt -o output/default/crashes/min_crash0 ./pop.run
```

* -i - Provides the input file that is going to be minimized
* -o - Shows the minimized output file will be saved
* ./pop.run - This is the target application that is used to conduct the minimization


Results may vary, but here are the results before and after minimization in our example.


##### Before Minimization

```
[afl++ 44de1e1c4e9a] /src/fuzzing_tutorial/2_Fuzzing_Tools/AFL (main) # cat output/default/crashes/id:000000,sig:06,src:000005+000001,time:106200,execs:103046,op:splice,rep:2.txt
pop!aaal
```

##### After Minimization

```
[afl++ 44de1e1c4e9a] /src/fuzzing_tutorial/2_Fuzzing_Tools/AFL (main) # cat output/default/crashes/min_crash0
pop!
```
