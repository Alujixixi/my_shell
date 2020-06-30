#!/bin/bash

gcc -c exectest.c -o exectest.o
gcc -c function.c -o function.o
gcc exectest.o function.o -o exectest -no-pie
