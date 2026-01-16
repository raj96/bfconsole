#include "vm/executor.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Memory used to store the running programs operands
typedef struct {
	uint64_t cap;
	uint8_t *mem;
} executor_heap;

// VM for the console
typedef struct {
	Token *tokens;
	uint64_t token_len;

	uint64_t inst_ptr;
	uint64_t mem_ptr;
	executor_heap heap;
} executor;

static executor vm = {NULL, 0, 0, 0, {0, NULL}};

void _executor_add_mem(void);
void ex_plus(void) {
	vm.heap.mem[vm.mem_ptr] += vm.tokens[vm.inst_ptr].meta.reps;
}

void ex_minus(void) {
	vm.heap.mem[vm.mem_ptr] -= vm.tokens[vm.inst_ptr].meta.reps;
}

void ex_lt(void) {
	int64_t mp = vm.mem_ptr - vm.tokens[vm.inst_ptr].meta.reps;
	if (mp >= 0) {
		vm.mem_ptr = (uint64_t)mp;
	}
}

void ex_gt(void) {
	vm.mem_ptr += vm.tokens[vm.inst_ptr].meta.reps;
	while (vm.heap.cap <= vm.mem_ptr) _executor_add_mem();
}

void ex_lsqb(void) {
	if (vm.heap.mem[vm.mem_ptr] == 0) {
		vm.inst_ptr = vm.tokens[vm.inst_ptr].meta.jmp;
	} 
}

void ex_rsqb(void) {
	if (vm.heap.mem[vm.mem_ptr] != 0) {
		vm.inst_ptr = vm.tokens[vm.inst_ptr].meta.jmp;
	} 
}

void ex_dot(void) {
	printf("%c", vm.heap.mem[vm.mem_ptr]);
}

static void (*executors[OP_COUNT])(void) = {
	&ex_plus,
	&ex_minus,
	&ex_lt,
	&ex_gt,
	&ex_lsqb,
	&ex_rsqb,
	&ex_dot,
};

#ifdef DEBUG
char op_to_char(Token);
void _executor_print(void) {
	printf("mem: [ ");
	for (uint64_t i = 0; i < vm.heap.cap; i++) {
		printf("%u ", vm.heap.mem[i]);
	}
	printf("]\n");
	printf("operation: %c\ta: %d\t", op_to_char(vm.tokens[vm.inst_ptr]), vm.heap.mem[vm.mem_ptr]);
	printf("mem_ptr: %lu\tinst_ptr: %lu {%lu}\n", vm.mem_ptr, vm.inst_ptr, vm.tokens[vm.inst_ptr].meta.reps);
}
#endif

void _executor_add_mem(void) {
#ifdef DEBUG
	printf("Adding memory\n");
#endif
	uint8_t *new_mem = (uint8_t *)realloc(vm.heap.mem, sizeof(uint8_t) * (vm.heap.cap + EXECUTOR_MEM_BLOCK));
	if (new_mem == NULL) {
		// TODO: Handle and return
	}
	memset(vm.heap.mem == NULL ? new_mem : vm.heap.mem + vm.heap.cap, 0, EXECUTOR_MEM_BLOCK);	
	vm.heap.cap += EXECUTOR_MEM_BLOCK;
	vm.heap.mem = new_mem;
}

void executor_init(Token *tokens, uint64_t token_len) {
	vm.tokens = tokens;
	vm.token_len = token_len;

	_executor_add_mem();
}

void executor_run(void) {
#ifdef DEBUG
	_executor_print();
#endif
	while (vm.inst_ptr < vm.token_len) {
		executors[vm.tokens[vm.inst_ptr].op]();
		vm.inst_ptr++;
#ifdef DEBUG
	_executor_print();
#endif
	}
}

void executor_free(void) {
	free(vm.heap.mem);
	token_free(vm.tokens);
}

