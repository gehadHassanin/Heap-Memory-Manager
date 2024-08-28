# Heap memory manager

This project implements a custom memory allocator, providing functionality similar to standard `malloc`, `calloc`, `realloc`, and `free` functions. The allocator manages memory dynamically using a free list, optimizing memory usage and reducing fragmentation.

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Installation](#installation)
- [Building and Running](#Building-and-Running)
  - [Compiling the Test Program](#Compiling-the-Test-Program)
  - [Creating the Shared Library](#creating-the-shared-library)
  - [Running Commands with the Library](#Running-Library)


## Overview

The custom memory allocator uses a free list to keep track of available memory blocks, allowing efficient memory allocation, deallocation, and resizing operations. The allocator aligns memory to improve performance and minimize issues related to misaligned memory access.

## Features

- **Dynamic Memory Allocation:** Supports `malloc`, `calloc`, `realloc`, and `free` equivalents.
- **Free List Management:** Efficiently manages free memory blocks using a doubly linked list.
- **Memory Alignment:** Aligns memory blocks to the nearest multiple of 8 bytes for optimal performance.
- **Heap Expansion and Reduction:** Automatically adjusts the heap size as needed.

## Installation

Clone the repository to your local machine:

```bash
git clone git@github.com:gehadHassanin/Heap-Memory-Manager.git
cd Heap-Memory-Manager
```
## Building and Running
### Compiling the Test Program
To compile and run the provided test program (contained in main.c):
```
gcc main.c src/Hmm.c src/FreeList.c -o hmm
./hmm
```

## Creating the Shared Library
To create a shared library (libHmm.so) with the custom memory management functions:
```
gcc -g -fPIC -Wall -shared src/Hmm.c src/FreeList.c -o libhmm.so
export LD_PRELOAD=$(realpath libhmm.so)
```

## Running Commands with the Library
To run any command using the custom memory management library:
```
LD_PRELOAD=$(realpath libhmm.so) <command>
```
**NOTE:** Replace `<command>` with the desired command or application you want to run with the custom memory manager

