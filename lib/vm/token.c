#include "vm/token.h"

#include <stdio.h>
#include <stdlib.h>

static uint64_t _n_tokens;

#ifdef DEBUG
char op_to_char(Operator op) {
	switch (op) {
		case OP_PLUS: return '+';
		case OP_MINUS: return '-';
		case OP_LT: return '<';
		case OP_GT: return '>';
		case OP_LSQB: return '[';
		case OP_RSQB: return ']';
		case OP_DOT: return '.';
		case OP_COMMA: return ',';
		default: return 'X';
	}
}

void tokens_print(Token *_tokens, int *_n_tokens) {
	int i = 0;
	for (; i < _n_tokens-1; i++) {
		printf("%c{%lu} | ", op_to_char(_tokens[i].op), _tokens[i].reps);
	}
	printf("%c{%lu}\n", op_to_char(_tokens[i].op), _tokens[i].reps);
}
#endif

bool Token_populate(char *file_path, Token *_tokens, int *_n_tokens) {
	FILE *fp = fopen(file_path, "r");
	if (fp == NULL) {
			return false;
	}

	int bracket_match = 0;
	char c;
	Operator op;
	while (feof(fp) == 0) {
		fread(&c, sizeof(char), 1, fp);
		if (op == OP_QUOTE) {
				op = OP_INVALID;
		} else {
			switch(c) {
				case '+':
					op = OP_PLUS;
					break;
				case '-':
					op = OP_MINUS;
					break;
				case '<':
					op = OP_LT;
					break;
				case '>':
					op = OP_GT;
					break;
				case '[':
					op = OP_LSQB;
					bracket_match++;
					break;
				case ']':
					op = OP_RSQB;
					bracket_match--;
					break;
				case '.':
					op = OP_DOT;
					break;
				case ',':
					op = OP_COMMA;
					break;
				case '"':
					op = OP_QUOTE;
					break;
				default:
					op = OP_INVALID;
					break;
			}
		}
		
		if (bracket_match != 0) {
			printf("Unmatched square brackets\n");
			free(_tokens);
			fclose(fp);
			return false;
		}

		if (op != OP_INVALID && op != OP_QUOTE) {
			if (_n_tokens > 0) {
				if (op == _tokens[_n_tokens-1].op) {
					_tokens[_n_tokens-1].reps++;
				} else {
					goto create_new_token;
				}
			} else {
create_new_token:
				_n_tokens++;
				_tokens = (Token *)realloc(_tokens, sizeof(Token) * _n_tokens);
				_tokens[_n_tokens-1] = (Token){
					.op = op,
					.reps = 1,
				};
			}
		}
	}

	return true;
}

void token_free(Token *_tokens) {
	free(_tokens);
}


