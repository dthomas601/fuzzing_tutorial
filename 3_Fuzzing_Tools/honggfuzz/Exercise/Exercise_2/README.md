# Exercise 3: Shape Measurement Fuzzing

In this example the file that will be fuzzed was written to measure different aspects of shapes.

The files to be examined were written to calculate the circumference and area for different shapes.

Examine ```shape_measurements.cpp``` and ```shape_measurements.h``` to identify vulnerable sections using AFL, libFuzzer, and honggfuzz.



## Task 1: Create and fuzz the program with honggfuzz

Use ```hfuzz-clang++``` to build an executable. Then fuzz the created file using ```honggfuzz```.


<details>
<summary>Solution Task 1</summary>

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
