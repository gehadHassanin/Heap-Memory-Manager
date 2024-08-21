#ifndef FREE_LIST_H_
#define FREE_LIST_H_

#include <stdint.h>
#define IS_LIST_EMPTY(size)  !(size)

#define FIRST_FIT (0)
#define BEST_FIT  (1) 
/**
 *  OPTIONS:
 *          - FIRST_FIT
 *          - BEST_FIT
 */
#define SEARCHING_ALGORITHM FIRST_FIT

typedef struct block {
    uint32_t length;
    struct block *previous;
    struct block *next;
}Block_t;

typedef struct {
    uint32_t size;
    Block_t *head;
}FreeList_t;

typedef enum {
    kFalse,
    kTrue,
}bool;

void Print(FreeList_t *pList);

void FreeList_Init(FreeList_t *pList);

void FreeList_Insert(FreeList_t *pList, Block_t *pBlock);

void FreeList_Delete(FreeList_t *pList, Block_t *pBlock);

Block_t *FreeList_FindSuitableBlock(FreeList_t *pList, uint32_t requiredLength);

void FreeList_Split(FreeList_t *pList, Block_t *pBlock, uint32_t requiredLength);

bool FreeList_IsFree(FreeList_t *pList, Block_t *pBlock);

bool FreeList_IsContingous(Block_t *pFBlock, Block_t *pSBlock);

void FreeList_Merge(FreeList_t *pList, Block_t *pFBlock, Block_t *pSBlock);
#endif  // FREE_LIST_H_