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




#### Buffer Underflow


#### Memory Leak


#### Double Frees


#### Heap Overflows


#### Input Validation