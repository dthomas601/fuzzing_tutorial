# Exercise 4: C++ Ada Import

In this example we will build and identify vulnerabilities in a C++ program that imports an Ada function.

AFL's compilers will be used to compile both the C++ and Ada components. 

The Ada code that will be used is saved in ```addition_pack.adb ``` and the package definition is found here ```addition_pack.ads```. The Ada code is written to accept two integers and produce the sum of the two values.

The C++ code that imports an Ada function is saved to ```int_overflow_file_read.cpp```.

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
