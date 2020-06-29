#!/bin/bash

gcc -c main.c -o main.o
gcc -c function.c -o function.o
gcc main.o function.o -o shellexe
