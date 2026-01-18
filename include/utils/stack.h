#ifndef STACK_H
#define STACK_H

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#ifdef STACK_IMPL
#define STACK_BLOCK_MEM	32

#define stack_implement(T)									\
typedef struct {											\
	uint64_t ptr;											\
	uint64_t cap;											\
	T *data;												\
} stack_##T;												\
															\
bool stack_##T##_init(stack_##T *stk) {						\
	stk = (stack_##T *)malloc(sizeof(stack_##T));			\
	stk->data = (T *)malloc(sizeof(T) * STACK_BLOCK_MEM);	\
	if (stk->data == NULL) {								\
		return false;										\
	}														\
															\
	stk->ptr = 0;											\
	stk->cap = STACK_BLOCK_MEM;								\
															\
	return true;											\
}															\
															\
bool stack_##T##_push(stack_##T *stk, T data) {				\
	if (stk->ptr + 1 >= stk->cap) {							\
		T *e_mem = (T *)realloc(stk->data,					\
				sizeof(T) * (stk->cap + STACK_BLOCK_MEM));	\
		if (e_mem == NULL) {								\
			return false;									\
		}													\
		stk->data = e_mem;									\
		stk->cap += STACK_BLOCK_MEM;						\
	}														\
															\
	stk->ptr += 1;											\
	stk->data[stk->ptr] = data;								\
															\
	return true;											\
}															\
															\
T stack_##T##_pop(stack_##T *stk) {							\
	if (stk->ptr == 0) {									\
		return 0;											\
	}														\
															\
	return stk->data[stk->ptr--];							\
}															\
															\
void stack_##T##_free(stack_##T *stk) {						\
	free(stk->data);										\
}															

#endif

#endif // STACK_H
