#ifndef TOKEN_H
#define TOKEN_H

#include <stdint.h>
#include <stdbool.h>

typedef enum {
	OP_PLUS = 0,
	OP_MINUS,
	OP_LT,
	OP_GT,
	OP_LSQB,
	OP_RSQB,
	OP_DOT,
	OP_COMMA,
	OP_QUOTE,
	OP_INVALID,
	OP_COUNT, // Total number of executors defined (see include/vm/executor.h)
} Operator;

typedef union {
	uint64_t reps;
	uint64_t jmp;
} TokenMeta;

typedef struct {
	Operator op;
	TokenMeta meta;
} Token;

bool token_populate(char *file_path, Token **tokens, int *n_token);
void token_free(Token *tokens);

#ifdef DEBUG
void tokens_print(Token *tokens, int n_token);
#endif

#endif
