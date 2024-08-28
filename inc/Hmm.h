#ifndef HMM_H_
#define HMM_H_


#include "FreeList.h"

#define INCREMENT (13926400)  
#define ALIGNMENT_SIZE(size)\
    if (size % 8 != 0) {\
        size = ((size + 7) / 8) * 8;\
    }


void *HmmAlloc(size_t size);
void *HmmCalloc(size_t numitems, size_t size);
void *HmmRealloc(void *ptr, size_t size);
void HmmFree(void *ptr);

void *malloc(size_t size);
void *calloc(size_t numitems, size_t size);
void *realloc(void *ptr, size_t size);
void free(void *ptr);

static bool IsVaildAddress(FreeList_t *pList, Block_t *pBlock);
static void DecreaseProgramBreak(void *ptr);
#endif  // HMM_H_ 