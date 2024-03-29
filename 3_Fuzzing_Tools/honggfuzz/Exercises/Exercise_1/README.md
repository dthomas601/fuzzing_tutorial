# Exercise 1: Search String (AFL++)

In this exercise use the fuzzing tools outlined previously to identify vulnerabilities in the source code provided.

In this portion of the assignment, we will provide different tasks that each student should complete. The answers are provided below, but the goal for the user should be to complete the activities without assistance. Only look at portions of the answers when all other alternatives have been exhausted.

## Sample input and output

Below are example inputs from the program being compiled normally with ```clang++```.

```
[afl++ e7b1c6fd0a5d] /src/fuzzing_tutorial/3_Exercises/Exercise_1 (main) # ./a.out
Enter the string that will be searched:
```

**Mary had a little lamb.**

```
Enter the character that is being searched for:
```

**a**

```
Length 23
Search char is 'a'
Checked string: Mary had a little lamb.
Chars count was: 4
```

## Task 1: Compile the files with AFL

Use the files provided in this section, ```string_search.cpp``` and ```checker.h``` , to create an executable binary with AFL.


<details>
<summary>Solution Task 1</summary>

Start by selecting a compiler to build the binary. The instructions are provided below:

```
afl-clang-lto++ -o searcher string_search.cpp
```

* -o searcher - This will create an executable named ```searcher```
* string_search.cpp - This is the C++ source file that is used to create the executable

This will create an instrumented executable to be fuzzed.

</details>

## Task 2: Create input seed values

Create at least one seed input file for AFL to mutate and use to fuzz the created executable. The input file should include two lines with the first being the string that should be searched and the second line containing the character to search for.

<details>

<summary> Solution Task 2 </summary>

There are multiple ways to create files that will be used as a seed value. In this example a folder ```in``` is created and we use the ```echo``` command to create these files.

```
mkdir in
```

```echo -e "Row Row Row your boat\no" > in/input1```

Second example is provide below as well:

```echo -e "jaielafilena0ofna;amfk\na" > in/input2```

To show these files, you can show something like the following:

```
cat in/*
```

</details>


## Task 3: Fuzz the executable with AFL

Take the resulting exeutable and fuzz it with the tool ```afl-fuzz```. Options for the tool can be found using ```afl-fuzz --help```.

<details>

<summary>Solution Task 3 </summary>

There are multiple ways of fuzzing this file.

An example of fuzzing an executable is shown below.

```
afl-fuzz -i afl_input -o output_AFL ./searcher
```

* -i - Input directory where new test cases will be saved
* -o - Output directory where the results from the fuzzing campaign will be saved

In this example the ```searcher``` binary is actively fuzzed to identify potential vulnerabiities.

AFL fuzzed the binary using the STDIN option. An example test case that could be provided is shown below:


test1
```
welcome to the real world
w
```

This ```test1``` testcase would be placed inside of the ```input``` folder before running AFL.

</details>


## Task 4: Check the crash inputs generated

The crashes generated should be saved to the path similar to ```output_AFL```. The full path will look like ```output_AFL/default/crashes/```.

With these inputs, the executable can then be tested to determine if the generated crash string is valid.

<details>

<summary>Solution Task 4 </summary>

An example test string that crashed this program is shown below:

```
NNSssssssssssssssssssssssssssssssssssssssssssss��ssssssssssssssssssssssssssssssssssssssssssssssssssssssssssss����ssssssssssssssssssssssssssssssssssssssssssssssssss����sssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssss
```

Execute the ```searcher``` binary and provide this input (or the input generated by your fuzz campaign) to test if a crash occurs. Our example gets the following crash report:

```
[afl++ 5d7ea6d9cf96] /src/fuzzing_tutorial/3_Exercises/Exercise_1 (main) # ./searcher
Enter the string that will be searched:
NNSssssssssssssssssssssssssssssssssssssssssssss��ssssssssssssssssssssssssssssssssssssssssssssssssssssssssssss����ssssssssssssssssssssssssssssssssssssssssssssssssss����sssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssss
Enter the character that is being searched for:
s
Length 36
Search char is s
Checked string NNSssssssssssssssssssssssssssssssssssssssssssss��ssssssssssssssssssssssssssssssssssssssssssssssssssssssssssss����ssssssssssssssssssssssssssssssssssssssssssssssssss����sssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssss
Chars count was: 259
Segmentation fault
```

</details>

## Task 5: Perform the same action with libFuzzer

Lets now modify the source file so that libFuzzer can be used. The harness structure for libFuzzer requires that we use the function ```extern "C" int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) ```. (
Refer to the example 2_Fuzzing_Tools/libFuzzer)

Another hint, the data to the ```LLVMFuzzerTestOneInput``` function is one input. You can utilize tools such as ```stringstream``` to tokenize the data that is generated by libFuzzer and modify the ```getline``` and ```cin``` portions of ```string_search.cpp``` used previously. The structure of your seed test cases would change to values that are string delimited. A modify example is shown below for an input that is delimited by ```,```.

```
welcome to the real world,w
```

Example usage for stringstream can be found here:
* https://www.geeksforgeeks.org/stringstream-c-applications/
* https://www.tutorialspoint.com/stringstream-in-cplusplus

Now rewrite the ```string_search.cpp``` source code to to used by libFuzzer. Compile the new source file ```string_search_harness.cpp``` and create an executable named ```searcher_libfuzzer```.

<details>

<summary>Solution Task 5 </summary>

The restructured source code can be found in ```string_search_harness.cpp```

To compile use the following command:

```
clang++ -g -O1 -fsanitize=fuzzer -o searcher_libfuzzer string_search_harness.cpp
```

</details>



## Task 6: Execute the libFuzzer binary

Run the libFuzzer binary.

<details>

<summary>Solution Task 6 </summary>

The command is as follows:

```
./searcher_libfuzzer
```

The resulting crash file will be saved to the current directory. The fuzzing campaign will stop when a crash is detected.

</details>

## Task 7: Create executables with honggfuzz

honggfuzz can use either of the source files created previously: ```string_search.cpp``` or ```string_search_harness.cpp```.

Create binaries from both binaries and fuzz both.


<details>

<summary>Solution Task 7 </summary>

#### STDIN

Compile the file using the following command:
```
hfuzz-clang++ -o string_search_stdin.run string_search.cpp
```

Fuzz the created executable using the command below. The crashes will be saved to the folder ```output_honggfuzz```.

```
honggfuzz -i honggfuzz_input/ -s --crashdir=output_honggfuzz/ -- ./string_search_stdin.run
```

#### Using fuzzing harness

The main difference is the way that the executable is compiled. Following the same principles used with standard ```clang++```.

```
hfuzz-clang++ -g -fsanitize=fuzzer -o string_search_harness.run string_search_harness.cpp
```

Fuzz the created executable using the command below. The crashes will be saved to the folder ```output_honggfuzz_harness```.

```
honggfuzz -i honggfuzz_input/ --crashdir=output_honggfuzz_harness/ -- ./string_search_harness.run
```

</details>








