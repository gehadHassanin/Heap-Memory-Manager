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
    Block_t *pSuitableBlock;
    // IF [SIZE == 0] => ALLOCATE AT LEAST THE SIZE OF A @Block_t TO AVOID ALLOCATING NOTHING
    size = size == 0 ? sizeof(Block_t) : size;
    // CALCULATE TOTAL LENGTH NEEDED: [REQUESTED SIZE + SPACE FOR THE BLOCK METADATA]
    totalLength = size + sizeof(Block_t);
    // ALIGN THE TOTAL LENGTH TO MEET THE SYSTEM'S MEMORY ALIGNMENT REQUIREMENTS
    ALIGNMENT_SIZE(totalLength);
    // IF THIS THE FIRST TIME THE USER CALLS HmmAlloc, INITIALIZE THE HEAP
    if (startOfHeap == NULL) {
        startOfHeap = sbrk(0);  
        FreeList_Init(&freeList);  
    }
    // TRY TO FIND A SUITABLE BLOCK FROM THE FREE LIST
    pSuitableBlock = FreeList_FindSuitableBlock(&freeList, totalLength);
    if (pSuitableBlock != NULL) {
        // IF THE SUITABLE BLOCK IS LARGER THAN NEEDED, SPLIT IT
        if (pSuitableBlock->length > totalLength) {
            FreeList_Split(&freeList, pSuitableBlock, totalLength);
        } else {
            // OTHERWISE, REMOVE THE BLOCK FROM THE FREE LIST SINCE IT WILL BE USED ENTIRELY
            FreeList_Delete(&freeList, pSuitableBlock);
        }
        // SET THE RETURN ADDRESS TO THE START OF THE SUITABLE BLOCK
        pReturnAddress = (void *)pSuitableBlock;
    } else {
        // IF NO SUITABLE BLOCK WAS FOUND, EXTEND THE HEAP
        while (pSuitableBlock == NULL) {
            // INCREMENT THE PROGRAM BREAK [ALLOCATE MORE MEMORY]
            if ((pReturnAddress = sbrk(INCREMENT)) == (void *) -1) {
                // IF sbrk FAILS, SET RETURN ADDRESS TO NULL AND BREAK THE LOOP
                pReturnAddress = NULL;
                break;
            }
            // UPDATE THE END OF THE HEAP TO THE NEW PROGRAM BREAK
            endOfHeap = sbrk(0);
            // SET THE LENGTH OF THE NEW BLOCK TO THE AMOUNT OF MEMORY JUST ALLOCATED
            ((Block_t *)pReturnAddress)->length = (endOfHeap - pReturnAddress);
            // INSERT THE NEW BLOCK INTO THE FREE LIST
            FreeList_Insert(&freeList, ((Block_t *)pReturnAddress));
            // TRY FINDING A SUITABLE BLOCK AGAIN NOW THAT MORE MEMORY IS AVAILABLE
            pSuitableBlock = FreeList_FindSuitableBlock(&freeList, totalLength);
            // IF THE SUITABLE BLOCK IS LARGER THAN NEEDED, SPLIT IT
            if (pSuitableBlock->length > totalLength) {
                FreeList_Split(&freeList, pSuitableBlock, totalLength);
            } else {
                // OTHERWISE, REMOVE IT FROM THE FREE LIST FOR USE
                FreeList_Delete(&freeList, pSuitableBlock);
            }
            // SET THE RETURN ADDRESS TO THE START OF THE NEWLY ALLOCATED BLOCK
            pReturnAddress = (void *)pSuitableBlock;
        }
    }
    // SET THE ACTUAL SIZE OF THE BLOCK 
    pSuitableBlock->length = size;    
    // RETURN THE ADDRESS OF THE ALLOCATED MEMORY
    return pReturnAddress = (pReturnAddress == NULL) ? NULL : (pReturnAddress + sizeof(Block_t));
}

void HmmFree(void *ptr) {
    // ENSURE THAT THE POINTER VALUE WAS OBTAINED BY A CALL HmmAlloc FUNCTION
    if (IsVaildAddress(&freeList, FETCH_START_BLOCK(ptr))) {
        // ENSURE THAT NOT FREE THE SAME PIECE OF ALLOCATED MEMORY MORE THAN ONCE
        if (!FreeList_IsFree(&freeList, FETCH_START_BLOCK(ptr))) {
            DecreaseProgramBreak(ptr);
            FreeList_Insert(&freeList, FETCH_START_BLOCK(ptr));
        }       
    }
}

void *HmmCalloc(size_t numitems, size_t size) {
    void *ptr = NULL;
    // CHECK IF SIZE AND NUMBER OF ITEMS ARER BOTH NON-ZERO
    if (size != 0 && numitems != 0) {
        ptr = HmmAlloc(numitems * size);
        // IF MEMORY ALLOCATION WAS SUCCESSFUL, INITIALIZE THE ALLOCATED MEMORY TO ZERO
        if (ptr != NULL) {
            ptr = memset(ptr, 0, size * numitems);
        }
    }
    // RETURN THE POINTER TO THE ALLOCATED AND ZERO-INITIALIZED MEMORY
    return ptr;
}

void *HmmRealloc(void *ptr, size_t size) {
    void *pReturnAddress = ptr;
    size_t old_size;
    // ENSURE THAT THE POINTER VALUE WAS OBTAINED BY A CALL HmmAlloc FUNCTION
    if (IsVaildAddress(&freeList, FETCH_START_BLOCK(ptr))) {
        old_size = FETCH_START_BLOCK(ptr)->length;
        if (size == 0) {
            FreeList_Insert(&freeList, FETCH_START_BLOCK(ptr));
            pReturnAddress =  NULL;
        } else if (old_size >= size ) {
            pReturnAddress =  ptr;
            // CHECK IF THE LEFTOVER SPACE CAN FORM A VALID FREE BLOCK
            if (IS_VALID_FREE_BLOCK(size - old_size)) {
                ((Block_t *)(ptr + old_size))->length = (size - old_size - sizeof(Block_t));
                FreeList_Insert(&freeList, ptr + old_size);
            }
        } else {
            // ALLOCATE A NEW BLOCK WITH THE REQUESTED SIZE
            pReturnAddress = HmmAlloc(size);
            if (pReturnAddress != NULL) {
                // DETERMINE THE SIZE TO COPY (MIN OF NEW SIZE OR OLD SIZE)
                size = size > FETCH_START_BLOCK(ptr)->length ? FETCH_START_BLOCK(ptr)->length : size;
                // COPY THE DATA FROM THE OLD BLOCK TO THE NEW BLOCK
                memcpy(pReturnAddress, ptr, size);
            }
            // FREE THE OLD BLOCK
            FreeList_Insert(&freeList, FETCH_START_BLOCK(ptr));
        }
   }
    return pReturnAddress;
}

bool IsVaildAddress(FreeList_t *pList, Block_t *pBlock) {
    bool flag = kFalse;
    // CHECK IF THE BLOCK'S ADDRESS IS WITHIN THE CURRENT PROGRAM BREAK
    if (pBlock < (Block_t *)sbrk(0)) {
        // CHECK IF THE BLOCK IS NOT CURRENTLY MARKED AS FREE IN THE FREE LIST
        if (!FreeList_IsFree(pList, pBlock)) {
          flag = kTrue;  
        }
    } 
    return flag;
}

void DecreaseProgramBreak(void *ptr) {
    uint32_t len = FETCH_START_BLOCK(ptr)->length;
    // CALCULATE THE END OF THE BLOCK BASED ON ITS LENGTH
    Block_t *pEndBlock = ptr + len;
    // CHECK IF THIS BLOCK IS AT THE CURRENT PROGRAM BREAK [END OF THE HEAP]
    if (pEndBlock != sbrk(0)) {
        // MOVE TO THE NEXT BLOCK IF IT EXISTS
        pEndBlock = (void *)pEndBlock + pEndBlock->length + sizeof(Block_t);
        // IF THE NEXT BLOCK ENDS EXACTLY AT THE PROGRAM BREAK, WE CAN REDUCE THE PROGRAM BREAK
        if (pEndBlock == sbrk(0)) {
            // ADJUST THE PROGRAM BREAK TO RELEASE THE MEMORY BACK TO THE SYSTEM
            sbrk(-1 * (pEndBlock->length + sizeof(Block_t)));
            // REMOVE THE BLOCK FROM THE FREE LIST SINCE IT'S NO LONGER AVAILABLE
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