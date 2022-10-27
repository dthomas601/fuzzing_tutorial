# Exercise 5: Shape Measurement Fuzzing

In this example the file that will be fuzzed was written to measure different aspects of shapes.

The files to be examined were written to calculate circumference and area for different shapes.

Examine ```shape_measurements.cpp``` and ```shape_measurements.h``` to identify vulnerable sections using AFL, libFuzzer, and honggfuzz.

## Task 1: Create and fuzz the program with AFL

Use ```afl-clang-fast++``` to build an executable. Then fuzz the created file using ```afl-fuzz```.

<details>
<summary>Solution Task 1</summary>

An option that can be taken is to change the structure of the file to use the function ```extern "C" int LLVMFuzzerTestOneInput```.

```
afl-clang-fast++ -fsanitize=fuzzer -o harness_afl shape_measurements_fuzzed.cpp
```

The created executable is saved to ```harness_afl```. In the edited file, the inputs accepted by the program are comma-delimited. To provide a seed input value we a test string is created and saved in a directory called ```input_afl```.

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


There are experimental options where libfuzzer can be run in a continious manner similar to AFL even when a crash is encountered. How to execute such a run is provided below. A warning to be aware of is that after running this command, you will be unable to enter ```Ctrl+C``` to stop the run. Using ```Ctrl+Z``` will exit the Docker window, but will not close the container completely. 

```
./harness_libfuzzer -fork=1 -ignore_crashes=1 -artifact_prefix=libfuzzer_crashes
```

</details>


## Task 3: Create and fuzz the program with honggfuzz

Use ```hfuzz-clang++``` to build an executable. Then fuzz the created file using ```hongfuzz```.


<details>
<summary>Solution Task 3</summary>

The file that can be used is the same harness that was created from Task 1 also. Start by building an executable with ```hfuzz-clang++```.

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

In this example we will build and identify vulnerabilities in a C++ program that imports an Ada function.

AFL's compilers will be used to compile both the C++ and Ada components. 

The Ada code that will be used is saved in ```addition_pack.adb ``` and the package definition is found here ```addition_pack.ads```. The Ada code is written to accept two integers and produce the sum of the two values.

The C++ code that imports an Ada function is saved to ```int_overflow_file_read.cpp```.

In this portion of the assignment, we will provide different tasks that each student should complete. The answers are provided below, but the goal for the user should be to complete the activities without assistance. Only look at portions of the answers when all other alternatives have been exhausted.

### Task 1: Compile Programs

Take the provided files and compile them using AFL's compilers. (Hint: afl-g++-fast has been tested for these exercises.)

<details>
<summary>Solution Task 1</summary>

```afl-g++-fast``` is used to compile these files and are shown below:

```
afl-g++-fast -c addition_pack.adb int_overflow_file_read.cpp
```

The ```-c``` flag is used to compile only. This generates a ```.o``` and ```.ali``` file from the Ada source code and a ```.o``` file from C++. 

</details>


### Task 2: Bind Ada file

Use ```gnatbind``` to bind ```addition_package```.

<details>
<summary>Solution Task 2</summary>

```gnatbind``` performs consistency check and will error if there are any discrepancies. This is highlighted below:

```
gnatbind -n addition_pack
```

-n - ```addition_pack``` is not a main Ada program and this is denoted by using this flag.

</details>


### Task 3: Link Ada and C++ files

Link the C++ and Ada components together using ```gnatlink```. 


<details>
<summary>Solution Task 3</summary>

Below is the command to build the executable ```runner_exec``` and link C++ and Ada components.

```
gnatlink addition_pack -o runner_exec --LINK=afl-g++-fast int_overflow_file_read.o
```

* addition_pack - This is the Ada package name that contains the function that will be imported
* -o runner_exec - Name of the output executable that will be created
* --LINK=afl-g++-fast - Selects the linker that will be used to combine files. 

</details>

### Task 4: Fuzz Executable

With the Ada and C++ mixed language executable built, now we can utilized AFL to determine if any vulnerabilities exist.

Fuzz the executable.

<details>
<summary>Solution Task 4</summary>

```
afl-fuzz -i inputs/ -o outputs ./runner_exec @@
```

* -i - Provide an input directory that contains seed testcases
* -o - Provide an output directory where all results from the fuzzing activities will be saved
* ./runner_exec @@ - Run the executed and specify that AFL will provide files that are provided as input


From the run above, integer overflow errors will be found in the given executable.

</details>

### Task 5: Replicate error in GDB

Following the paths used in ```Task 4```, crash files will be saved to ```./outputs/default/crashes```. Use these input files to replicate crashes with the GDB and directly running the executable.


<details>
<summary>Solution Task 5</summary>

For simplicity we have changed the name of our example crash file to ```crashfile1```. Expect your filename to be much longer. (The normal name describes the mutations used to identify the inputs used to crash the target program.)

To crash the program and identify the program source we can utilize GDB. Starting GDB with the target and parameters provided is shown below:

```
gdb -ex=r -ex=backtrace --args ./runner_exec outputs/default/crashes/crashfile1
```

* -ex=r - Execute a single GDB command: This command runs the executable with the provided input file
* -ex=backtrace - Execute a single GDB command: This command show a backtrace of the executables run and where errors occurred
* --args ./runner_exec outputs/default/crashes/crashfile1 - The target executable and input file are provided after the --args parameter. GDB then takes and executes these commands.

The resulting backtrace can then be used to identify the source of the error. 

</details>
