Collections-C
=============

A library of common container types including a list, vector, hashtable, deque etc..

#### Setting up the environment

##### GNU/Linux
In order to successfully build the project, the following packages must installed first:
- gcc
- autoconf
- automake
- libtool
- m4

These packages can usually be installed through your distributions package manager with the fillowing commands:

- debian / ubuntu `apt-get install gcc autoconf automake libtool m4`
- fedora `yum install gcc autoconf automake libtool m4`
- arch linux `pacman -S gcc autoconf automake libtool m4`

##### Windows
Building on windows requires MinGW which provides all the tools needed to build the project and it can be downloaded from the [MinGW website](http://mingw.org).

#### Building the project
Using the therminal emulator navigate to the project directory
```
cd /path/to/project/Collections-C
```
once there, run the autogen script
```
./autogen.sh
```
If autogen complains that it couldn't find a directory named "m4", run: `mkdir m4` and then `./autogen.sh`.  
  
After autogen run:
```
./configure
```
At this point we should be able to build the project by running:
```
make
```
The test can be run by:
```
make check
```
