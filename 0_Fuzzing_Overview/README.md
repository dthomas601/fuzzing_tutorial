# What is fuzzing?

Fuzzing is a black box software testing technique that is designed to find implementation bugs using malformed inputs generated in an automated fashion. By utilizing fuzzing, large amounts of unexpected inputs are generated to determine how the target program will respond. The results can then be analyzed to identify vulnerabilities in the target executable.  


# What are the benefits of fuzzing?

Building strong testing procedures is a key aspect of a software development process. Developers use these testing procedures to write more secure code. Writing robust test cases can take time, and utilizing automated fuzzing techniques is an excellent compliment. This automated testing can be a great tool to find zero-day vulnerabilities.

There are also a variety of fuzzing tools that can be used in tandem to identify vulnerabilities.

# What are different fuzzing methods?

Several fuzzing methods can be utilized to automate the testing process. A subset of these categories is described below:

### Dumb (Random) Fuzzers

The input for these types of fuzzers is generated randomly. These types of tools do not consider the required input format and generally don't understand how the target program responds to a given input. These tools provide trade-offs to consider as they are easy and inexpensive to set up, but are inefficient in their testing ability.

### Mutation-based/Smart Fuzzers

These types of fuzzers generate inputs based on criteria of valid inputs. From these inputs, simple tactics such as bit-flipping, bit-shifting, simple math changes are applied.  

### Grammar-based Fuzzing

This provides added benefits because some programs only function properly when inputs meet certain pattern constraints. A template of valid values are provided. Since an understanding of input formats is required, there are additional time requirements that are necessary to ensure inputs meet the specified format. The benefits of more efficient testing have to be weighed against the additional setup costs.

### Feedback-Based/Coverage-Based Fuzzers

This type of fuzzer utilizes code coverage information when generating new inputs. Code coverage is a metric that can help developers understand how much of a code base has been tested. Based on this, the fuzzer can monitor which parts of a program have been reached and generate similar inputs with random, but small changes.


### Generation-Based Fuzzers

This fuzzing strategy can generate new inputs from scratch. It differs from mutation-based tools because it does not need any type of original input to generate new ones. Although not required, providing valid test inputs can help the fuzzing process be more efficient.


### Tools Discussed Here

The tools used in this course are AFL++, libFuzzer, and hongfuzz. Each of these tools are discussed more in Section 2: Fuzzing Tools.