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
- [Example](#example)
- [Contact](#contact)


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
## Usage
### Compile and run the program:
To compile and run the provided program (contained in main.c):
```
make run TEST_SRC=<file_name>
```
NOTE: Replace `<file_name>` with the file you want to run. <br>
NOTE: by default file_name is main.c, so if you run this command without specifying the file name, the main.c will run

### Running Commands with the Library:
To run any command using the custom memory management library:
```
make run-shared-lib COMMAND=<command>
```
NOTE: Replace `<command>` with the command you want to run with libhmm.so 

### Clean the build files:
```
make clean
```

## Example
Here's an example usage of the Heap Memory Manager:
```
#include <stdio.h>
#include "inc/Hmm.h" // Include your custom header for the heap memory manager

int main() {
    // Allocate memory for an array of 3 integers
    int *arr = (int *)malloc(3 * sizeof(int));
    if (arr != NULL) {
        // Assign values to the array
        arr[0] = 10;
        arr[1] = 20;
        arr[2] = 30;
        printf("Array values: %d, %d, %d\n", arr[0], arr[1], arr[2]);

        // Reallocate memory to expand the array to 5 integers
        arr = (int *)realloc(arr, 5 * sizeof(int));
        if (arr != NULL) {
            arr[3] = 40;
            arr[4] = 50;
            printf("Updated array values: %d, %d, %d, %d, %d\n", arr[0], arr[1], arr[2], arr[3], arr[4]);

            // Free the allocated memory
            free(arr);
            printf("Memory freed successfully\n");
        } else {
            printf("Memory reallocation failed\n");
        }
    } else {
        printf("Memory allocation failed\n");
    }

    return 0;
}

```
## Contact
For questions or contributions, please contact **Gehad Hassanein** at gehadelmenshawy2@gmail.com