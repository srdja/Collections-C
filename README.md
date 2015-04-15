Collections-C
=============

A library of common container types including a list, vector, hashtable, deque etc..

#### Setup

#####Dependencies
- gcc
- autoconf
- automake
- libtool
- m4

These packages can usually be installed through your distributions package manager.

Building on windows requires MinGW which provides all the tools needed to build the project and it can be downloaded from the [MinGW website](http://mingw.org).

#### Building the project
In the project directory, run the following commands:
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
The tests can be run by:
```
make check
```
