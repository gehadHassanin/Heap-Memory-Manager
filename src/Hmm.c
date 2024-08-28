#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>

#include "../inc/FreeList.h"
#include "../inc/Hmm.h"

static void *startOfHeap = NULL;
void *endOfHeap;

FreeList_t freeList;

void *HmmAlloc(size_t size) {
    uint32_t totalLength;
    void *pReturnAddress = NULL;
    size = size == 0 ? sizeof(Block_t) : size;
    totalLength = size + sizeof(Block_t);
    ALIGNMENT_SIZE(totalLength);
    // IN CASE OF THE FIRST TIME THE USER CALL THE HmmAlloc
    if (startOfHeap == NULL) {
        startOfHeap = sbrk(0);
        FreeList_Init(&freeList);
    }
    Block_t *pSuitableBlock;
    pSuitableBlock = FreeList_FindSuitableBlock(&freeList, totalLength);
    if (pSuitableBlock != NULL) {
        if (pSuitableBlock->length > totalLength) {
            FreeList_Split(&freeList, pSuitableBlock, totalLength);
        } else {
            FreeList_Delete(&freeList, pSuitableBlock);
        }
        pReturnAddress = (void *)pSuitableBlock;
    } else {
        while (pSuitableBlock == NULL) {
            // INCREMENT THE PROGRAM BREAK
            if ((pReturnAddress = sbrk(INCREMENT)) == (void *) -1) {
                pReturnAddress = NULL;
                break;
            }
            endOfHeap = sbrk(0);
            ((Block_t *)pReturnAddress)->length = (endOfHeap - pReturnAddress);
            FreeList_Insert(&freeList, ((Block_t *)pReturnAddress));
            pSuitableBlock = FreeList_FindSuitableBlock(&freeList, totalLength);
            if (pSuitableBlock->length > totalLength) {
                FreeList_Split(&freeList, pSuitableBlock, totalLength);
            } else {
                FreeList_Delete(&freeList, pSuitableBlock);
            }
            pReturnAddress = (void *)pSuitableBlock;
        }
    }
    pSuitableBlock->length = size;    
    
    return pReturnAddress = (pReturnAddress == NULL) ? NULL : (pReturnAddress + sizeof(Block_t));
}

void HmmFree(void *ptr) {
    // ENSURE THAT THE POINTER VALUE WAS OBTAINED BY A CALL HmmAlloc FUNCTION
    if (IsVaildAddress(&freeList, (Block_t *)(ptr - sizeof(Block_t)))) {
        // ENSURE THAT NOT FREE THE SAME PIECE OF ALLOCATED MEMORY MORE THAN ONCE
        if (!FreeList_IsFree(&freeList, (Block_t *)(ptr - sizeof(Block_t)))) {
            DecreaseProgramBreak(ptr);
            FreeList_Insert(&freeList, (Block_t *)(ptr - sizeof(Block_t)));
        }       
    }
}

void *HmmCalloc(size_t numitems, size_t size) {
    void *ptr = NULL;
    if (size != 0 && numitems != 0) {
        ptr = HmmAlloc(numitems * size);
        if (ptr != NULL) {
            ptr = memset(ptr, 0, size * numitems);
        }
    }
    return ptr;
}

void *HmmRealloc(void *ptr, size_t size) {
    void *pReturnAddress = ptr;
    Block_t *pTempBlock;
    size_t copy_size;
    // ENSURE THAT THE POINTER VALUE WAS OBTAINED BY A CALL HmmAlloc FUNCTION
    if (IsVaildAddress(&freeList, (Block_t *)((char *)ptr - sizeof(Block_t)))) {
        pTempBlock = (Block_t *)((char *)ptr - sizeof(Block_t));
        if (size == 0) {
            HmmFree(ptr);
            pReturnAddress =  NULL;
        } else {
            // ALLOCATE A NEW BLOCK WITH THE REQUESTED SIZE
            pReturnAddress = HmmAlloc(size);
            if (pReturnAddress != NULL) {
                // DETERMINE THE SIZE TO COPY (MIN OF NEW SIZE OR OLD SIZE)
                copy_size = size > pTempBlock->length ? pTempBlock->length : size;
                // COPY THE DATA FROM THE OLD BLOCK TO THE NEW BLOCK
                memcpy(pReturnAddress, ptr, copy_size);
                // FREE THE OLD BLOCK
                FreeList_Delete(&freeList, pTempBlock);
            }
        }
    }
    return pReturnAddress;
}


bool IsVaildAddress(FreeList_t *pList, Block_t *pBlock) {
    bool flag = kFalse;
    if (pBlock < (Block_t *)sbrk(0)) {
        if (!FreeList_IsFree(pList, pBlock)) {
          flag = kTrue;  
        }
    } 
    return flag;
}

void DecreaseProgramBreak(void *ptr) {
    uint32_t len = ((Block_t *)(ptr - sizeof(Block_t)))->length;
    Block_t *pEndBlock = ptr + len;
    if (pEndBlock != sbrk(0)) {
        pEndBlock = (void *)pEndBlock + pEndBlock->length + sizeof(Block_t);
        if (pEndBlock == sbrk(0)) {
            sbrk(-1 * (pEndBlock->length + sizeof(Block_t)));
            FreeList_Delete(&freeList, pEndBlock);
        }
    }
}

void *malloc(size_t size) {
    return HmmAlloc(size);
}

void free(void *ptr) {
    if (ptr != NULL) {
        HmmFree(ptr);
    }
}

void *calloc(size_t numitems, size_t size) {
    return HmmCalloc(numitems, size);
}

void *realloc(void *ptr, size_t size) {
    if (ptr == NULL) {
        return HmmAlloc(size);
    } else {
        return HmmRealloc(ptr, size);
    }
    
}