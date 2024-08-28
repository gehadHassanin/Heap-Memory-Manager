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