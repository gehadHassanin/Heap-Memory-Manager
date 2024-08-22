#include <stdint.h>
#include <stdio.h>

#include "../inc/FreeList.h"

void FreeList_Init(FreeList_t *pList) {
    pList->size = 0;
    pList->head = NULL;
}

void FreeList_Insert(FreeList_t *pList, Block_t *pBlock) {
    // IN CASE OF THE LIST IS EMPTY OR IF IT WILL BE INSERTED BRFORE STARTING THE LIST
    if (IS_LIST_EMPTY(pList->size) || pList->head > pBlock) {
        // IN CASE OF THE LIST IS EMPTY
        if (IS_LIST_EMPTY(pList->size)) {
            pBlock->next = NULL; 
        // IN CASE OF IT WILL BE INSERTED BEFORE STARTING THE LIST
        } else if (pList->head > pBlock) {
            pBlock->next = pList->head;
            pBlock->next->previous = pBlock;
        }
        pBlock->previous = NULL;
        pList->head = pBlock;
    } else {
        Block_t *pTemp = pList->head;
        while (pTemp->next != NULL) {
            if (pTemp->next > pBlock) {
                break;
            }
            pTemp = pTemp->next;
        }

        pBlock->next = pTemp->next;
        pBlock->previous = pTemp;
        pTemp->next = pBlock;
        // IN CASE OF NOT INSERTING THE BLOCK AT THE END OF THE FREE LIST
        if (pBlock->next != NULL) {
            pBlock->next->previous = pBlock; 
        }
    }
    pList->size++;
}


void FreeList_Delete(FreeList_t *pList, Block_t *pBlock) {
    // IN CASE OF ONLY ONE BLOCK AT THE FREE LIST
    if (pList->size == 1) {
        pList->head = NULL;
    // IN CASE OF THE FIRST BLOCK AT THE FREE LIST
    } else if (pList->size != 1 && pBlock->previous == NULL) {
        pList->head = pBlock->next;
        pBlock->next->previous = NULL;
    } else {
        pBlock->previous->next = pBlock->next;
        // IN CASE OF BLOCK IS NOT THE LAST AT THE FREE LIST
        if (pBlock->next != NULL) {
             pBlock->next->previous = pBlock->previous;
        }   
    }
    pList->size--;
}

Block_t *FreeList_FindSuitableBlock(FreeList_t *pList, uint32_t requiredLength) {
    Block_t *pSuitableBlock = NULL;
    if (!IS_LIST_EMPTY(pList->size)) {
        Block_t *pTemp = pList->head;
        while (pTemp != NULL) {
            if (pTemp->length >= requiredLength) {
                pSuitableBlock = pTemp;
                break;
            } else {
                while (pTemp != NULL && pTemp->next != NULL && FreeList_IsContingous(pTemp, pTemp->next)) {
                    FreeList_Merge(pList, pTemp, pTemp->next);
                    if (pTemp->length >= requiredLength) {
                        pSuitableBlock = pTemp;
                        break;                           
                    } else {
                        pTemp = pTemp->next;
                    }
                }
                if (pTemp != NULL && pTemp->next != NULL && FreeList_IsContingous(pTemp, pTemp->next)) {
                    break;
                }
            }
            
            if (pTemp != NULL) {
                pTemp = pTemp->next;
            }
        }
    } 
    return pSuitableBlock;
}

void FreeList_Split(FreeList_t *pList, Block_t *pBlock, uint32_t requiredLength) {
    // DELETE THE OLD BLOCK
    FreeList_Delete(pList, pBlock);
    // CHECK IF THERE IS ENOUGH SPACE TO INSERT META DATA
    if ((pBlock->length - requiredLength) > sizeof(Block_t)) {
        Block_t *splitBlock = (void *)pBlock + requiredLength;
        splitBlock->length = (pBlock->length - requiredLength);
        FreeList_Insert(pList, splitBlock);
    } else {
        // NO ENOUGH SPACE TO INSERT META DATA
    }
}

bool FreeList_IsFree(FreeList_t *pList, Block_t *pBlock) {
    bool flag = kFalse;
    Block_t *pHelpBlock = pList->head;
    uint32_t numOfBlocks = 0;
    while (pHelpBlock != NULL) {
        if(pHelpBlock == pBlock) {
           flag = kTrue; 
           break;
        }
        pHelpBlock = pHelpBlock->next;
    }
    return flag;
}

bool FreeList_IsContingous(Block_t *pFBlock, Block_t *pSBlock) {
    bool flag = kFalse;
    Block_t *pEndOfFBlock = (void *)pFBlock + pFBlock->length + sizeof(Block_t);
    if (pEndOfFBlock == pSBlock) {
        flag = kTrue;
    }
    return flag;
}

void FreeList_Merge(FreeList_t *pList, Block_t *pFBlock, Block_t *pSBlock) {
    // DELETE THE FIRST BLOCK FROM THE FREE LIST
    FreeList_Delete(pList, pFBlock);
    // DELETE THE SECOND BLOCK FROM THE FREE LIST
    FreeList_Delete(pList, pSBlock);
    // CALCULATE LENGTH OF THE NEW BLOCK
    pFBlock->length += ((pSBlock->length) + sizeof(Block_t));
    // INSERT NEW BLOCK INTO THE FREE LIST
    FreeList_Insert(pList, pFBlock);
}

void Print(FreeList_t *pList) {
    Block_t *pTemp = pList->head;
    
    while (pTemp != NULL) {
        printf("start = %p\n", pTemp);
        printf("length = %u\n", pTemp->length);
        printf("prev = %p\n", pTemp->previous);
        printf("next = %p\n", pTemp->next);
        pTemp = pTemp->next;
    }
    printf("---------------------------------------------\n");
}

/*
Block_t *FreeList_FindSuitableBlock(FreeList_t *pList, uint32_t requiredLength) {
    Block_t *pSuitableBlock = NULL;
    if (!IS_LIST_EMPTY(pList->size)) {
        Block_t *pTemp = pList->head;
        #if SEARCHING_ALGORITHM == FIRST_FIT
            while (pTemp != NULL) {
                if (pTemp->length >= requiredLength) {
                    pSuitableBlock = pTemp;
                    break;
                } else {
                    while (pTemp->next != NULL && FreeList_IsContingous(pTemp, pTemp->next)) {
                        FreeList_Merge(pList, pTemp, pTemp->next);
                        if (pTemp->length >= requiredLength) {
                            pSuitableBlock = pTemp;
                            break;                           
                        } else {
                            pTemp = pTemp->next;
                        }
                    }

                    if (pTemp->next != NULL && FreeList_IsContingous(pTemp, pTemp->next)) {
                        break;
                    }
                }
                pTemp = pTemp->next;
            }
        #elif SEARCHING_ALGORITHM == BEST_FIT
            int32_t suitableLength = pTemp->length;
            while (pTemp != NULL) {
                if (pTemp->length == requiredLength) {
                   pSuitableBlock = pTemp;
                   break; 
                } else if (pTemp->length > requiredLength && pTemp->length <= suitableLength) {
                    suitableLength = pTemp->length;
                    pSuitableBlock = pTemp;
                }
            }
        #endif  // SEARCHING_ALGORITHM
    } 
    return pSuitableBlock;
}
*/