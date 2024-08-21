#ifndef HMM_H_
#define HMM_H_


#include "FreeList.h"

#define HEAP_SIZE (1073741824) // one Gega byte 
#define INCREMENT (139264)     // 136K byte
#define INC_PROGRAM_BREAK(programBreak)  (programBreak = programBreak + INCREMENT)
#define DEC_PROGRAM_BREAK(decrement)     (programBreak = programBreak - decrement)
#define IS_HEAP_END(programBreak)        (programBreak == (Hmm + HEAP_SIZE))

void *HmmAlloc(size_t size);
void HmmFree(void *ptr);
static bool IsVaildAddress(FreeList_t *pList, Block_t *pBlock);
static void DecreaseProgramBreak(void *ptr);
#endif  // HMM_H_ 