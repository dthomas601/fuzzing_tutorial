# Honggfuzz

honggfuzz is a command-line security fuzzing tool. It supplies and modifies input to test programs to detect and log crashes.

Some of the capabilities for honggfuzz includes support for strategies such as feedback-driven and evolutionary style fuzzing.

A major benefit of honggfuzz is that it provides multi-process and multi-threaded capabilities with no requirements for additional fuzzing instances. The fuzzing corpus is automatically shared and improved between all fuzzing processes.

* Github Repo: https://github.com/google/honggfuzz
* Project Page: https://honggfuzz.dev/

## Usage Breakdown

To use honggfuzz, we will start with building the target executable. The ```hfuzz-clang++``` compiler is used in this example to build an executable.

```
hfuzz-clang++ pop.cpp -o pop_hfuzz.run
```

With the executable created, we now will use ```honggfuzz``` to fuzz the newly created binary.

This is performed with the following command:

```
honggfuzz -i in/ -s --output output/ --crashdir=crashes --threads 4 -- ./pop_hfuzz.run
```

* -i in/ - Specify the input folder that contains the initial corpus values
* -s - Select inputs from honggfuzz to be entered via STDIN. 
* --output out_folder - Specify the output folder that the coverage corpus or minimized coverage corpus is written to
* --crashdir - Directory to save crash files
* --threads - By default, it the half the number of CPUs. This can be set manually using this option.
* pop_hfuzz.run - Name of the executable to be fuzzed.

From the fuzzing campaign, the files that crashed the executable are saved to ```--crashdir```.


```honggfuzz``` can also be used to provide files as input to a target binary.

An additional capability that can be utilized is the ability to reuse fuzzing harnesses from other tools such as libFuzzer. In the next example we use the ```hi_trap_honggfuzz.cpp``` example.

We would build this example with the following command:

```
hfuzz-clang++ -g -O1 -fsanitize=fuzzer,address -o harness hi_trap_honggfuzz.cpp
```

* hfuzz-clang++ - This is one of a set of compilers that can be used to instrument executables. Other options include: ```hfuzz-clang, hfuzz-gcc, hfuzz-g++```
* -g - Generates source-level debug information
* -fsanitize=fuzzer,address - Set the fuzzing options that will be used.
* -o harness - This is the output executable that will be created.
* hi_trap_honggfuzz.cpp - This is the file that is being compiled.

With the executable created, we can now fuzz with honggfuzz. This is done with the command provided below:

```
honggfuzz -i hi_inputs/ --crashdir=hi_trap_output -- ./harness
```

* -i hi_inputs/ - This provides the input folder that contains test cases to use. If no test cases are provided, it will start with its own selection of inputs.
* -s - This options specifies that input is being provided through STDIN.
* --crashdir=hi_trap_output - This option allows to select the folder that crash inputs will be saved to.
* ./harness - This is the name of the executable that will be fuzzed.


