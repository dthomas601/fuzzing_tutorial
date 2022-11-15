# Environment Setup

To conduct the exercises in this course there are two options. First we have provided a Docker container that has all of the necessary tools provided by default. A short tutorial is provided below.

The second option is to use a Mint OS virtual machine (VM) that also contains the tools necessary. This VM can be provided upon request.

# Docker Overview

Docker is a open source platform that provides standardized, executable components in a container that allows developers to build, deploy, and run code in any environment.

In this section, the tools needed to use Docker for this course are discussed.

## Running Docker

There are several command options when using Docker. To start and run a container each command will start with ```docker run```. From this point, the user will provide additional options to specify exactly what Docker should execute. For a description of all the options associated use ```docker run --help```. Below is an example command:

```
docker run -it --rm -v /path/to/bind/to/container:/path/inside/of/container container_name
```

* run - Run a command in a new container
* -it - Allow the container to have an interactive terminal
* --rm - Once a container is exited, delete it completely
* -v - Use the bind command to connect files on the host to a directory inside of the container
* container_image - specifies the name of the Docker image that will be used to create a container

This will create a new container with an interactive shell from the specified image. Many pre-existing images can be found at ```hub.docker.com```

 ## Building Docker Image

 In addition to using pre-existing images, your own images can be created using a ```Dockerfile```. A ```Dockerfile``` is a text file that consists of instructions to build Docker images. A Dockerfile for this class has been provided at the Github repo: https://github.com/dthomas601/fuzzing_tutorial . These files can be downloaded anywhere, but in this example they have been saved to the Desktop. 

 Navigate to the directory where the Dockerfile is located. We will then build an image. The command below will be used:

 ```
docker build -t fuzz_class .
 ```
* build - Build an image from a Dockerfile
* -t fuzz_class - This names the image that is created ```fuzz_class```
* . - The dot specifies to look for a ```Dockerfile``` in the current directory

To confirm that the image was created, use the command ```docker images```. You will see the newly created image in this list.

## Starting Fuzzing Container

Finally to start the container, use the previous commands to start the container and follow the course materials. Use something similar to the command below:

```
docker run -it --rm -v C:\User\demo\Desktop\fuzzing_tutorial:/src/ fuzz_class
```

This command will start an interactive container and loads the materials to be used for each section of the source.

# Using Fuzzing Virtual Machine

The fuzzing VMs will not use Docker, but has all of the tools necessary to complete all exercises. To get the files necessary, download them from the Github repo: https://github.com/dthomas601/fuzzing_tutorial .



