# What is fuzzing?

Fuzzing is a black box software testing technique that is designed to find implementation bugs using automated malformed inputs in an automated fashion. By utilizing fuzzing this technique enters large amounts of unexpected inputs and records how the target program responds. The results can then be analyzed to identify vulnerabilities in the target executable.  


# What are the benefits of fuzzing?

Building strong testing procedures is a key aspect of a software development process. Developers use these testing procedures to make sure that they are creating secure code. Writing robust test cases can take time, and utilizing automated techniques such as fuzzing are excellent compliments. This automated testing can be a great tool to find zero-day vulnerabilities.

There are also a variety of fuzzing tools that can be used collaborative to identify vulnerabilities.

# What are different fuzzing methods?

There are a number of fuzzing methods that can be utilized to automate the testing process. A subset of these categories are described below:

### Dumb Fuzzers

The input for these types of fuzzers are generated completely randomly. The tools do not consider the required input format and generally don't have the ability to understand how the target program responds to a given input. These tools provide trade-offs to consider as they are easy and inexpensive to set up, but are inefficient in their testing ability.


### Mutation-based/Smart Fuzzers

These types of fuzzer generate inputs based on a criteria of valid inputs. This provides added benefits because some program only function properly when inputs meet certain pattern constraints. Since understanding of these input formats are required, there are additional time requirements that are necessary to ensure inputs meet the specified format. The benefits of more efficient testing has to be weighed against the additional setup costs. 

### Feedback-Based/Coverage-Based Fuzzers

This type of fuzzer utilizes code coverage information when generating new inputs. Code coverage is a metric that can help developers understand how much of a code base as actually been tested. Based on this, the fuzzer can monitor which parts of a program has been reached and generate similar inputs with random, but small changes.


### Generation-Based Fuzzers

This fuzzing strategy can generate new inputs from scratch. It differs from mutation-based tools because it does not need any type of original input to generate new ones. Although not required, providing valide tests inputs can help the fuzzing process be more efficient.


### Tools Discussed Here

The tools used in this course are AFL++, libFuzzer, and hongfuzz. Each of these tools are discussed more in the Section 2: Fuzzing Tools.





