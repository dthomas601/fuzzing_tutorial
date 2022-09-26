# Exercise 1: libexif (AFL++)

This exercise in part was used from https://github.com/antonio-morales/Fuzzing101/tree/main/Exercise%202

In this exercise we will replicate and expliot CVE-2009-3895 in libexif 0.6.14. To download and build the libexif target, follow the steps provided below:

Create a directory to save the project:

```
cd $HOME
mkdir fuzzing_libexif && cd fuzzing_libexif/
```

Download and uncompress libexif 0.6.14:

```
wget https://github.com/libexif/libexif/archive/refs/tags/libexif-0_6_14-release.tar.gz
tar -xzvf libexif-0_6_14-release.tar.gz	
```

Build and install libexif:

```
cd libexif-libexif-0_6_14-release/
./autogen.sh
autoreconf -i
CC=afl-clang-lto CFLAGS="-O1 -g -fsanitize=address" ./configure --enable-shared=no --prefix="$HOME/fuzzing_libexif/install/"
make
make install
```

##### Choosing an interface application

For this example the library will be used with an inferface application (exif command-line). exif 0.6.15 can be downloaded and uncompressed by using the instructions below. The exif tool will use the libexif library downloaded previously.

```
cd $HOME/fuzzing_libexif
wget https://github.com/libexif/exif/archive/refs/tags/exif-0_6_15-release.tar.gz
tar -xzvf exif-0_6_15-release.tar.gz
```

Now, we can build and install the exif command-line utility:

```
cd exif-exif-0_6_15-release/
./autogen.sh
autoreconf -i
CC=afl-clang-lto CFLAGS="-O1 -g -fsanitize=address" ./configure --enable-shared=no --prefix="$HOME/fuzzing_libexif/install/" PKG_CONFIG_PATH=$HOME/fuzzing_libexif/install/lib/pkgconfig
make
make install
```

Test functionality by typing the following:

```
$HOME/fuzzing_libexif/install/bin/exif
```

##### Downloading Test File Corpus

We will now download pictures that will be used to test the exif tool for any potential vulnerabilities. Below we download an existing archive of photos.

```
cd $HOME/fuzzing_libexif
wget https://github.com/ianare/exif-samples/archive/refs/heads/master.zip
unzip master.zip
```

Test JPG images can be found can be found in the following directory:

```
 $HOME/fuzzing_libexif/install/bin/exif $HOME/fuzzing_libexif/exif-samples-master/jpg/Canon_40D.jpg
```

## Tasks

In this portion of the assignment we will provide different tasks that each student should complete. The answers are provided below, but the goal for the user should be to complete the activities without assistance. Only look at portions of the answers when all other alternatives have been exhausted.


### Task 1: Minimize the test corpus

In fuzzing tools and software the minimizizing the test corpus that is used can save time.  Tools such as ```afl-cmin``` are used to create the smallest subset of input files that still trigger as much coverage of the target application.

Use ```afl-cmin``` to reduce the number of files that will be used in the corpus. Apply the tools to the following directory that was downloaded in previous steps:

```$HOME/fuzzing_libexif/exif-samples-master/jpg/```

Save the minimized corpus into the following directory:

```$HOME/fuzzing_libexif/fuzz_min_corpus```

For usage instructions use ```afl-cmin --help```.


<details>
<summary>Task 1 Answer Summary</summary>
<br>

The input directory for this task is the corpus that needs to be minimized. You should also select the location that the minimized corpus should be saved. Our selection is provided below:

```
mkdir $HOME/fuzzing_libexif/fuzz_min_corpus
```

Next, we use ```afl-cmin``` to create a minimized corpus:

```
afl-cmin -i $HOME/fuzzing_libexif/exif-samples-master/jpg/ -o $HOME/fuzz_min_corpus $HOME/fuzzing_libexif/install/bin/exif @@
```

* -i - input directory with starting corpus
* -o - output directory for minimized files
* $HOME/fuzzing_libexif/install/bin/exif - This is the tool that is being fuzzed
* @@ - This marks the the location in the target’s command line where the input file name should be placed

The results should look something like the following:

```
[afl++ cd3aeace4d36] ~/fuzzing_libexif/exif-samples-master/jpg # afl-cmin -i /root/fuzzing_libexif/exif-samples-master/jpg/ -o $HOME/fuzz_min_corpus $HOME/fuzzing_libexif/install/bin/exif @@
corpus minimization tool for afl++ (awk version)

[*] Testing the target binary...
[+] OK, 79 tuples recorded.
[*] Obtaining traces for 143 input files in '/root/fuzzing_libexif/exif-samples-master/jpg/'.
    Processing 143 files (forkserver mode)...
[*] Processing traces for input files in '/root/fuzzing_libexif/exif-samples-master/jpg/'.
    Processing file 143/143
    Processing tuple 1913/1913 with count 143...[+] Found 1913 unique tuples across 143 files.
[+] Narrowed down to 48 files, saved in '/root/fuzz_min_corpus'.
[afl++ cd3aeace4d36] ~/fuzzing_libexif/exif-samples-master/jpg #
```
</details>


### Task 2: Fuzz the target application

Use ```afl-fuzz``` to find testcases that will crash the the ```$HOME/fuzzing_libexif/install/bin/exif``` application. For input use the minimized corpus that you generated in the previous task ```$HOME/fuzz_min_corpus```.

Save the output to ```$HOME/afl_exif_testcases```.

<details 
<summary>Task 2 Answer Summary </summary>

The input directory for this task is the corpus that was minimized previously. The output directory is where crash data is being placed.

```
afl-fuzz -i $HOME/fuzz_min_corpus/ -o $HOME/afl_exif_testcases -- $HOME/fuzzing_libexif/install/bin/exif @@
```

When executed, the AFL++ dashboard should load and begin testing inputs. Let this run for about 5-10 minutes or until you see at least five unique crashes that have occurred.

### Task 3: Find crash test files

After stopping AFL++, navigate to the ```afl_exif_testcases``` directory. Here you will find a directory structure that has been created by AFL++. What we will focus on is the crashes directory: $HOME/afl_exif_testcases --> default --> crashes.

Here are all of the files that potentially causes crashes.

<details>

Display the directory that contains the files where crashes occurred:

```ls  $HOME/afl_exif_testcases/default/crashes/``` 

</details>


### Task 4: Minimize the existing testcases that have been generated from ```afl-fuzz```.

Use the tool ```afl-tmin``` to reduce the size of the testcases that the were previously generated. ```afl-tmin``` shrinks the test input that caused a crash to the smallest size that still generates the same result. This tool is applied to one testcase file at a time.

Apply ```afl-tmin``` to the first five testcases that were created from step two.

<details>
<summary>Task 4 Answer Summary</summary>
<br>

This example shows how we used ```afl-tmin``` to reduce the size of one test file.

Take this example and apply it to your first five test files that were generated.

```
afl-tmin -i id\:000000\,sig\:11\,src\:000176\,time\:223135\,execs\:65494\,op\:havoc\,rep\:16 -o crash0 -- $HOME/fuzzing_libexif/install/bin/exif @@ 
```

* -i - input file whose size will be reduced
* -o - output file that contains the minimized test file
* -- seperates input arguments from executing the target application
* $HOME/fuzzing_libexif/install/bin/exif - This is the target application being used
* @@ - signals that inputs should be passed as if they came from the command line

</details>
```


### Task 5 Analyze Crash Data

From the testcases that have been created, lets understand where ```exif``` is crashing.

For each crash file in ```$HOME/afl_exif_testcases/default/crashes/``` display the filename and line number where the crash was reported.



<details>

<summary>Task 5 Answer Summary</summary>
<br>

For each crash file that was generated, we pass them to ```exif```. When compiled, we enabled AddressSantizer libraries that help to identify memory errors in target application. 

We show an example of this below.

First we execute ```exif``` with a crash file:

```
[afl++ 65b46ee0debf] ~/afl_exif_testcases/default/crashes.2022-09-23-17:22:50 (main) # $HOME/fuzzing_libexif/install/bin/exif id:000000,sig:11,src:000030,time:148428,execs:26661,op:havoc,rep:16

AddressSanitizer:DEADLYSIGNAL
=================================================================
==6618==ERROR: AddressSanitizer: SEGV on unknown address 0x61a100000083 (pc 0x5618762476c9 bp 0x0000fffffffd sp 0x7fffa0522700 T0)
==6618==The signal is caused by a READ memory access.
    #0 0x5618762476c9 in exif_get_sshort /src/fuzzing_tutorial/3_Exercises/Exercise_1/fuzzing_libexif/libexif-libexif-0_6_14-release/libexif/exif-utils.c
    #1 0x5618762476c9 in exif_get_short /src/fuzzing_tutorial/3_Exercises/Exercise_1/fuzzing_libexif/libexif-libexif-0_6_14-release/libexif/exif-utils.c:104:10
    #2 0x5618762201da in exif_data_load_data /src/fuzzing_tutorial/3_Exercises/Exercise_1/fuzzing_libexif/libexif-libexif-0_6_14-release/libexif/exif-data.c:819:6
    #3 0x56187624070e in exif_loader_get_data /src/fuzzing_tutorial/3_Exercises/Exercise_1/fuzzing_libexif/libexif-libexif-0_6_14-release/libexif/exif-loader.c:387:2
    #4 0x561876210452 in main /src/fuzzing_tutorial/3_Exercises/Exercise_1/fuzzing_libexif/exif-exif-0_6_15-release/exif/main.c:438:9
    #5 0x7fe0a01a7d8f  (/lib/x86_64-linux-gnu/libc.so.6+0x29d8f) (BuildId: 69389d485a9793dbe873f0ea2c93e02efaa9aa3d)
    #6 0x7fe0a01a7e3f in __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e3f) (BuildId: 69389d485a9793dbe873f0ea2c93e02efaa9aa3d)
    #7 0x5618761496f4 in _start (/src/fuzzing_tutorial/3_Exercises/Exercise_1/fuzzing_libexif/install/bin/exif+0x8e6f4) (BuildId: 9dd183d32fc8ff90)

AddressSanitizer can not provide additional info.
SUMMARY: AddressSanitizer: SEGV /src/fuzzing_tutorial/3_Exercises/Exercise_1/fuzzing_libexif/libexif-libexif-0_6_14-release/libexif/exif-utils.c in exif_get_sshort
==6618==ABORTING
```

To get the full trace of the crash, we analyze this with ```gdb```. We pass ```exif``` to ```gdb``` and then also the crash file.

```
[afl++ 65b46ee0debf] ~/afl_exif_testcases/default/crashes.2022-09-23-17:22:50 (main) # gdb $HOME/fuzzing_libexif/install/bin/exif
GNU gdb (Ubuntu 12.0.90-0ubuntu1) 12.0.90
Copyright (C) 2022 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.
Type "show copying" and "show warranty" for details.
This GDB was configured as "x86_64-linux-gnu".
Type "show configuration" for configuration details.
For bug reporting instructions, please see:
<https://www.gnu.org/software/gdb/bugs/>.
Find the GDB manual and other documentation resources online at:
    <http://www.gnu.org/software/gdb/documentation/>.

For help, type "help".
Type "apropos word" to search for commands related to "word"...
Reading symbols from /src/fuzzing_tutorial/3_Exercises/Exercise_1/fuzzing_libexif/install/bin/exif...

```

After starting ```gdb``` we can then pass a crash file to be executed. In the output below we use the command ```r``` and then the file to be executed. We could have also used the command ```run``` to complete the same task.

Once executed, ```gdb``` points to where the error occurred.

```

(gdb) r id:000000,sig:11,src:000030,time:148428,execs:26661,op:havoc,rep:16
Starting program: /src/fuzzing_tutorial/3_Exercises/Exercise_1/fuzzing_libexif/install/bin/exif id:000000,sig:11,src:000030,time:148428,execs:26661,op:havoc,rep:16
warning: Error disabling address space randomization: Operation not permitted
[Thread debugging using libthread_db enabled]
Using host libthread_db library "/lib/x86_64-linux-gnu/libthread_db.so.1".

Program received signal SIGSEGV, Segmentation fault.
0x00005614eac3a6c9 in exif_get_sshort (buf=<optimized out>, order=EXIF_BYTE_ORDER_MOTOROLA) at exif-utils.c:92
92                      return ((buf[0] << 8) | buf[1]);
```

To then see the full backtrace of the crash, we use the ```bt``` command in the ```gdb``` environment.

```
(gdb) bt
#0  0x00005614eac3a6c9 in exif_get_sshort (buf=<optimized out>, order=EXIF_BYTE_ORDER_MOTOROLA) at exif-utils.c:92
#1  exif_get_short (buf=<optimized out>, order=EXIF_BYTE_ORDER_MOTOROLA) at exif-utils.c:104
#2  0x00005614eac131db in exif_data_load_data (data=0x606000000080, d_orig=<optimized out>, ds_orig=<optimized out>)
    at exif-data.c:819
#3  0x00005614eac3370f in exif_loader_get_data (loader=<optimized out>) at exif-loader.c:387
#4  0x00005614eac03453 in main (argc=<optimized out>, argv=<optimized out>) at main.c:438
```

From this example we can begin to examine the issue at ```exif-utils.c:92```

We leave as an exercise to identify what line number is the cause of each error for the other files.


[CVE-2009-3895](https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2009-3895)

[CVE-2012-2836](https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2012-2836)

</details>












