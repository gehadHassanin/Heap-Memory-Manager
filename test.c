#include <stdio.h>
#include "inc/Hmm.h"
#include "inc/Hmm.h"
#include "inc/FreeList.h"

extern FreeList_t freeList;

int main () {
    void* ptr = HmmAlloc(100);
    void* ptr1 = HmmAlloc(100);
    Print(&freeList);
    HmmFree(ptr1);
    Print(&freeList);
    ptr1 = HmmAlloc(200);
    Print(&freeList);
    HmmFree(ptr1);
    Print(&freeList);
}