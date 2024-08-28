#ifndef HMM_H_
#define HMM_H_

#include "FreeList.h"

/**
 * @brief Defines the increment size for increasing the program break.
 *
 * This macro sets a constant value of 13,926,400 bytes (approximately 13.3 MB),
 * which is used to increment the program break when more heap memory is needed.
 * The chosen size balances the frequency of `sbrk` system calls and the memory overhead.
 */

#define INCREMENT (13926400)  


/**
 * @brief Aligns the given size to the nearest multiple of 8.
 *
 * This macro adjusts the provided size to ensure it is a multiple of 8 bytes,
 * which is a common alignment requirement for memory allocations to optimize performance
 * and avoid potential issues with misaligned access.
 *
 * @param size The size to be aligned. This value will be modified if it is not already
 *             a multiple of 8.
 */
#define ALIGNMENT_SIZE(size)\
    if (size % 8 != 0) {\
        size = ((size + 7) / 8) * 8;\
    }


/**
 * @brief Allocates memory of the specified size.
 *
 * @param size The size of memory to allocate.
 * @return A pointer to the allocated memory block. 
 *         Returns NULL if the allocation fails.
 */
void *HmmAlloc(size_t size);


/**
 * @brief Allocates memory for an array of elements and initializes it to zero.
 *
 * @param numitems The number of elements to allocate.
 * @param size The size of each element.
 * @return A pointer to the allocated and zero-initialized memory block.
 *         Returns NULL if the allocation fails or if either numitems or size is zero.
 */
void *HmmCalloc(size_t numitems, size_t size);


/**
 * @brief Reallocates memory, changing the size of the previously allocated block.
 *
 * @param ptr A pointer to the memory block previously allocated by HmmAlloc.
 * @param size The new size of the memory block.
 * @return A pointer to the newly allocated memory block, with content from the original block copied over.
 *         Returns NULL if the reallocation fails. If size is zero, the memory block is freed.
 */
void *HmmRealloc(void *ptr, size_t size);


/**
 * @brief Frees a previously allocated memory block.
 *
 * @param ptr A pointer to the memory block to be freed. If ptr is NULL, no operation is performed.
 */
void HmmFree(void *ptr);


/**
 * @brief Checks if a given block address is valid within the current heap memory.
 *
 * This function verifies whether the provided block address falls within the allocated
 * heap and is not marked as free in the free list.
 *
 * @param pList A pointer to the free list structure.
 * @param pBlock A pointer to the block to validate.
 * @return `true` if the block address is valid, `false` otherwise.
 */
static bool IsVaildAddress(FreeList_t *pList, Block_t *pBlock);

/**
 * @brief Decreases the program break if the specified block is at the end of the heap.
 *
 * This function checks if the memory block is at the end of the heap (program break).
 * If so, it reduces the program break, effectively releasing the memory back to the system.
 *
 * @param ptr A pointer to the memory block to check and potentially reduce the program break.
 */
static void DecreaseProgramBreak(void *ptr);


void *malloc(size_t size);

void *calloc(size_t numitems, size_t size);

void *realloc(void *ptr, size_t size);

void free(void *ptr);
#endif  // HMM_H_ 