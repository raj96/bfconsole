#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "vm/token.h"

// Since brainf*ck theoritically allows for infinite memory
// and doing that practically is not possible, the allocation
// is handled in blocks of `EXECUTOR_MEM_BLOCK`, that is,
// everytime new memory is needed the heap memory is increased
// by EXECUTOR_MEM_BLOCK * sizeof(unit8_t) bytes
#define EXECUTOR_MEM_BLOCK 8

void executor_init(Token *, uint64_t);
void executor_run(void);
void executor_free(void);

#endif // EXECUTOR_H

