# Software Vulnerabilities

Before exploiting a vulnerability, we have to have a clear understanding of what it is.

### What is a software vulnerability?

According to NIST, it is a security flaw, glitch, or weakness in software code that could be exploited by an attacker (threat source).

### Common Software Vulnerabilities

* Buffer Overflow
* String Vulnerabilities
* Unsigned Integer Wrapping
* Invalid-type conversion
* Read out of bounds
* Write out of bounds


### How can software vulnerabilities be used?


* Remote code execution – These vulnerabilities allow someone to execute malicious code on the victim's system.

* Denial of service – These vulnerabilities can allow someone to crash the software or entire system.

* Privilege Escalation – These vulnerabilities allow someone to escalate their privileges from a local account to an admin account.

* Broken authentication and identification theft - Vulnerabilities that allow an unauthorized system or information access

* Information leakage - Class of vulnerabilities that release information inadvertently from an information system.

* Etc.



## Vulnerabilities Explored

In each part of this section, the goal is to provide a brief introduction to the common software vulnerabilities. 
These examples are being shown using C/C++ code examples.


#### Integer Overflow

An integer overflow is a type of arithmetic overflow error where the result of an operation does not fit in a given memory space.

Instead of displaying an error, the result is often unexpected. It is listed as the 13th most dangerous weakness is the CWE Top 25 (cwe.mitre.org/top25/archive/2022/2022_cwe_top25.html).

In the majority of programming languages, integer values allocate a certain number of bits in memory. For example a 32-bit integer data type may store an unsigned integer between 0 and 4,294,967,295
a signed integer between -2,147,483,848 and 2,147,483,847. (For signed integers, the most significant bit denotes if the value is positive or negative.


Lets follow an example:

```
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

int main(int argc, char **argv){

    int val, i;
    char *mem;

    if (argc < 2)
        exit(1);

    val = atoi(argv[1]);

    printf("Received input\n");

    if (val > 0){

        int new_alloc = val * sizeof(char *);
        printf("Size of char * %ld\n",sizeof(char *));
        printf("Allocation size: %i\n",new_alloc);
        mem = malloc(new_alloc);
        printf("Created char pointer array\n");
        printf("INT_MAX     :   %d\n", INT_MAX);

        if (mem == NULL){
            printf("Failure\n");
            exit(2);

        }
    }

    printf("Outputting values\n");
    for (i=0; i < val; i++){
        mem[i] = 'A';
        printf("%c", mem[i]);
    }

    printf("\n");

    return 0;
}
```

In the program, an argument from the command-line is needed. The program checks to make sure an agrument is provided and that that number is greater than 0. This number should be larger than 0, because if it is negative this will cause an error when passing this argument to malloc. For smaller numbers, the number of 'A's that would be allocated is printed to the screen. The failure section of code can be triggered starting at line 81, if a overflowed integer value is passed to malloc.

What number would cause an overflow?

```
[afl++ 5c279dfbe5e3] /src/fuzzing_tutorial/1_Vulnerabilities (main) # ./a.out 268435456
Received input
Size of char * 8
Allocation size: -2147483648
Created char pointer array
INT_MAX     :   2147483647
Failure
```


#### Integer Underflow

Next we will see what happens when an Integer Underflow occurs. Review the code below:




#### Memory Leak
(developer.ibm.com/articles/au-toughgame/)

A memory leak occurs when a memory is allocated, but never freed. This can be detrimental for program that often do not terminate such as servers or daemons. If took much memory is consumed, then the functionality and performance of the underlying system can be impacted.

In the following two code example we will short poor and good practices when it comes to allocating memory and releasing it when done.


--Reassignment issues
--Handling return values from a function
--Don't create orphan memory sections

Principle, always give back what you acquire.


#### Double Frees

Below is an example program that outlines a double free error.

#include<stdlib.h>

int main(){

    char *ptr = (char*) malloc(sizeof(char));

    *ptr = 'a';

    free(ptr);
    free(ptr);
}

This example is admittedly trivial and does not depict the problem in a natural way. In many cases this happens when you have more than one pointer to the same memory location. When this memory location is freed multiple times, this can lead to undefined behavior.

#### Heap Overflows

Heap overflows are types of buffer overflows that occur with heap data.


#### Input Validation