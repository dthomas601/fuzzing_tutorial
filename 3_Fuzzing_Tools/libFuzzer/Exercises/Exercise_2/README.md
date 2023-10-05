# Exercise 2: Shape Measurement Fuzzing

In this example the file that will be fuzzed was written to measure different aspects of shapes.

The files to be examined were written to calculate the circumference and area for different shapes.

Examine ```shape_measurements.cpp``` and ```shape_measurements.h``` to identify vulnerable sections using AFL, libFuzzer, and honggfuzz.


## Task 1: Create and fuzz the program with libFuzzer

Change the structure of the file ```shape_measurements.cpp``` into a fuzzing harness that uses the function ```extern "C" int LLVMFuzzerTestOneInput```. Use ```clang++``` to build an executable. Then run the executable to fuzz it.

<details>
<summary>Solution Task 1</summary>

Start by building an executable with ```clang++```. Make sure to use the ```-fsanitize``` option.

```
clang++ -g -O1 -fsanitize=fuzzer -o harness_libfuzzer shape_measurements_fuzzed.cpp
```

From that executable, it can now be fuzzed. In the command provided below, we run libfuzzer with options to save crashes to a specific directory.

```
./harness_libfuzzer -artifact_prefix=libfuzzer_crashes/
``` 
* -artifact_prefix=libfuzzer_crashes/ - This option saved created crash files to the directory provided.

There are experimental options where libfuzzer can be run in a continious manner similar to AFL even when a crash is encountered. Instructions for performing such a run are provided below. A warning to be aware of is that after running this command in a Docker container. You will be unable to enter ```Ctrl+C``` to stop the run. Using ```Ctrl+Z``` will exit the Docker window, but will not close the container completely. 

```
./harness_libfuzzer -fork=1 -ignore_crashes=1 -artifact_prefix=libfuzzer_crashes
```

</details>