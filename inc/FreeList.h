#ifndef FREE_LIST_H_
#define FREE_LIST_H_

#include <stdint.h>

/**
 * @brief Checks if the free list is empty.
 *
 * This macro evaluates whether the free list contains any blocks. It returns true
 * if the size of the list is zero, indicating that the list is empty.
 *
 * @param size The size of the free list. This represents the number of blocks currently
 *             in the list.
 * @return `true` if the list is empty (size is zero), `false` otherwise.
 */
#define IS_LIST_EMPTY(size)  !(size)


/**
 * @brief: Represents a block of memory in the heap.
 *
 * This structure holds the metadata for a block of memory, including its length and
 * pointers to the previous and next blocks in the free list.
 */
typedef struct block {
    uint32_t length;           /**< The size of the memory block. */
    struct block *previous;    /**< Pointer to the previous block in the free list. */
    struct block *next;        /**< Pointer to the next block in the free list. */
} Block_t;


/**
 * @brief Represents a free list of memory blocks.
 *
 * This structure holds information about the free list, including its size (number of blocks)
 * and a pointer to the head of the list.
 */
typedef struct {
    uint32_t size;             /**< The number of blocks in the free list. */
    Block_t *head;             /**< Pointer to the first block in the free list. */
} FreeList_t;


/**
 * @brief Boolean values for use in conditional checks.
 *
 * This enum defines two values: `kFalse` and `kTrue`, representing boolean `false` and `true`, respectively.
 */
typedef enum {
    kFalse,  /**< Represents `false` in boolean logic. */
    kTrue,   /**< Represents `true` in boolean logic. */
} bool;


/**
 * @brief Initializes the free list.
 *
 * This function sets up the initial state of the free list, preparing it for use in the memory allocator.
 *
 * @param pList A pointer to the free list structure to initialize.
 */
void FreeList_Init(FreeList_t *pList);


/**
 * @brief Inserts a memory block into the free list.
 *
 * This function adds a block to the free list, maintaining the correct order
 * and linking the blocks appropriately.
 *
 * @param pList A pointer to the free list structure.
 * @param pBlock A pointer to the block to insert into the free list.
 */
void FreeList_Insert(FreeList_t *pList, Block_t *pBlock);


/**
 * @brief Removes a memory block from the free list.
 *
 * This function unlinks and removes a specified block from the free list.
 *
 * @param pList A pointer to the free list structure.
 * @param pBlock A pointer to the block to remove from the free list.
 */
void FreeList_Delete(FreeList_t *pList, Block_t *pBlock);


/**
 * @brief Finds a suitable memory block in the free list for allocation.
 *
 * This function searches the free list for a block of sufficient size to satisfy an allocation request.
 *
 * @param pList A pointer to the free list structure.
 * @param requiredLength The size of the memory block required.
 * @return A pointer to a suitable memory block if found, or NULL if no suitable block is available.
 */
Block_t *FreeList_FindSuitableBlock(FreeList_t *pList, uint32_t requiredLength);


/**
 * @brief Splits a memory block in the free list into two smaller blocks.
 *
 * This function divides a larger block into two parts, adjusting the free list accordingly.
 *
 * @param pList A pointer to the free list structure.
 * @param pBlock A pointer to the block to split.
 * @param requiredLength The size of the first block after splitting.
 */
void FreeList_Split(FreeList_t *pList, Block_t *pBlock, uint32_t requiredLength);


/**
 * @brief Checks if a memory block is free.
 *
 * This function determines whether a given block is marked as free in the free list.
 *
 * @param pList A pointer to the free list structure.
 * @param pBlock A pointer to the block to check.
 * @return `true` if the block is free, `false` otherwise.
 */
bool FreeList_IsFree(FreeList_t *pList, Block_t *pBlock);


/**
 * @brief Checks if two memory blocks are contiguous in memory.
 *
 * This function verifies whether two blocks are adjacent in memory, which may allow them to be merged.
 *
 * @param pFBlock A pointer to the first block.
 * @param pSBlock A pointer to the second block.
 * @return `true` if the blocks are contiguous, `false` otherwise.
 */
bool FreeList_IsContingous(Block_t *pFBlock, Block_t *pSBlock);


/**
 * @brief Merges two contiguous memory blocks in the free list.
 *
 * This function combines two adjacent blocks into a single larger block, adjusting the free list accordingly.
 *
 * @param pList A pointer to the free list structure.
 * @param pFBlock A pointer to the first block.
 * @param pSBlock A pointer to the second block.
 */
void FreeList_Merge(FreeList_t *pList, Block_t *pFBlock, Block_t *pSBlock);
#endif  // FREE_LIST_H_
