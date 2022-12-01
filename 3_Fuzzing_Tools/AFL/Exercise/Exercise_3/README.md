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