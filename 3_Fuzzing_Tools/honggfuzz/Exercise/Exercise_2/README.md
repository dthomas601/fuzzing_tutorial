# Exercise 3: Shape Measurement Fuzzing

In this example the file that will be fuzzed was written to measure different aspects of shapes.

The files to be examined were written to calculate the circumference and area for different shapes.

Examine ```shape_measurements.cpp``` and ```shape_measurements.h``` to identify vulnerable sections using AFL, libFuzzer, and honggfuzz.

## Task 1: Create and fuzz the program with AFL

Use ```afl-clang-fast++``` to build an executable. Then fuzz the created file using ```afl-fuzz```.

<details>
<summary>Solution Task 1</summary>

An option that can be taken is to change the structure of the file to use the function ```extern "C" int LLVMFuzzerTestOneInput```.

```
afl-clang-fast++ -fsanitize=fuzzer -o harness_afl shape_measurements_fuzzed.cpp
```

The created executable is saved to ```harness_afl```. In the edited file, the inputs accepted by the program are comma-delimited. To provide a seed input value, a test string is created and saved in a directory called ```input_afl```.

```
echo "1,15" > input_afl/test1
echo "2,30" > input_afl/test2
echo "3,10,10" > input_afl/test3
```

Now ```afl-fuzz``` is used to test the created file.

```
afl-fuzz -i input_afl/ -o output_afl/ ./harness_afl
``` 

* -i input_afl/ - The input directory that seeds inputs are saved.
* -o output_afl/ - Directory where results are saved.
* ./harness_afl - The target executable under test.


The results can be found in ```output_afl/default/crashes/```.

</details>


## Task 2: Create and fuzz the program with libFuzzer

Use ```clang++``` to build an executable. Then run the executable to fuzz it.

<details>
<summary>Solution Task 2</summary>

The file that can be used is the same harness that was created from Task 1. Start by building an executable with ```clang++```. Make sure to use the ```-fsanitize``` option.

```
clang++ -g -O1 -fsanitize=fuzzer -o harness_libfuzzer shape_measurements_fuzzed.cpp
```

From that executable, it can now be fuzzed. In the command provided below, we run libfuzzer with options to ignore crashes and save those crashes to a specific directory.

```
./harness_libfuzzer -artifact_prefix=libfuzzer_crashes/
``` 
* -artifact_prefix=libfuzzer_crashes/ - This option saved created crash files to the directory provided.


There are experimental options where libfuzzer can be run in a continious manner similar to AFL even when a crash is encountered. Instructions for performing such a run are provided below. A warning to be aware of is that after running this command, you will be unable to enter ```Ctrl+C``` to stop the run. Using ```Ctrl+Z``` will exit the Docker window, but will not close the container completely. 

```
./harness_libfuzzer -fork=1 -ignore_crashes=1 -artifact_prefix=libfuzzer_crashes
```

</details>


## Task 3: Create and fuzz the program with honggfuzz

Use ```hfuzz-clang++``` to build an executable. Then fuzz the created file using ```honggfuzz```.


<details>
<summary>Solution Task 3</summary>

An option that can be taken is to change the structure of the file to use the function ```extern "C" int LLVMFuzzerTestOneInput```.

Once the harness has been created, build an executable with ```hfuzz-clang++```.

```
hfuzz-clang++ -g -fsanitize=fuzzer,address shape_measurements_fuzzed.cpp -o harness_honggfuzz
```
* -g - Generates source-level debug information
* -fsanitize=fuzzer,address - Set the fuzzing options that will be used.


With the executable created, it can now be fuzzed. An input directory that contains sample inputs will be provided and then options to select STDIN and a crash directory is also provided. The same inputs from Task 1, will be used. These inputs will be saved into a directory named ```input_hongfuzz```

```
mkdir input_honggfuzz &&
echo "1,15" > input_honggfuzz/test1 &&
echo "2,30" > input_honggfuzz/test2 &&
echo "3,10,10" > input_honggfuzz/test3
```

```
mkdir output_honggfuzz
honggfuzz -i input_honggfuzz/ -s --crashdir=output_honggfuzz -- ./harness_honggfuzz
```

This will generate a large number of crash files inside of ```output_honggfuzz```. Currently there is not an option to minimize the generated outputs as there is with AFL ```afl-cmin``` and then the ```-merge``` flag with libfuzzer.

</details>
