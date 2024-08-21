#include <stdio.h>
#include <stdint.h>

#include "../inc/FreeList.h"
#include "../inc/Hmm.h"

int8_t Hmm[HEAP_SIZE];
void *programBreak = Hmm;

FreeList_t freeList;

void *HmmAlloc(size_t size) {
    uint32_t totalLength;
    void *pReturnAddress = NULL;
    totalLength = size + sizeof(Block_t);
    // IN CASE OF THE HEAP IS ENDED
    if (programBreak > ((void *)Hmm + HEAP_SIZE)) {
        pReturnAddress = NULL;
    } else {
        // IN CASE OF THE FIRST TIME THE USER CALL THE HmmAlloc
        if (programBreak == Hmm) {
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
                pReturnAddress = programBreak;
                INC_PROGRAM_BREAK(programBreak);
                if (programBreak > (void *)((void *)Hmm + HEAP_SIZE)) {
                    uint32_t len = programBreak - (void *)((void *)Hmm + HEAP_SIZE); 
                    programBreak -= len;
                    ((Block_t *)pReturnAddress)->length = INCREMENT - len;
                } else {
                    ((Block_t *)pReturnAddress)->length = INCREMENT;
                }
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
    }
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

bool IsVaildAddress(FreeList_t *pList, Block_t *pBlock) {
    bool flag = kFalse;
    if (pBlock < (Block_t *)(Hmm + HEAP_SIZE) && pBlock < (Block_t *)programBreak) {
        if (!FreeList_IsFree(pList, pBlock)) {
          flag = kTrue;  
        }
    } 
    return flag;
}

void DecreaseProgramBreak(void *ptr) {
    uint32_t len = ((Block_t *)(ptr - 24))->length;
    Block_t *pEndBlock = ptr + len;
    if (pEndBlock != programBreak) {
        pEndBlock = (void *)pEndBlock + pEndBlock->length + sizeof(Block_t);
        if (pEndBlock == programBreak) {
            DEC_PROGRAM_BREAK((pEndBlock->length + sizeof(Block_t)));
        }
    }
}