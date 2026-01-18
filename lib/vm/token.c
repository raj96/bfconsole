#include "vm/token.h"

#include <stdio.h>
#include <stdlib.h>

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

void tokens_print(Token *_tokens, int _n_tokens) {
	int i = 0;
	for (; i < _n_tokens; i++) {
		if (_tokens[i].op != OP_LSQB && _tokens[i].op != OP_RSQB) {
			printf("token: %c\treps: %lu\n", op_to_char(_tokens[i].op), _tokens[i].meta.reps);
		} else {
			printf("token: %c\tjmp: %lu\n", op_to_char(_tokens[i].op), _tokens[i].meta.jmp);
		}
	}
}
#endif

// TODO: Fix non-nested brackets
void _token_calculate_jmp(Token *_tokens, int _n_tokens) {
	int64_t lp = 0, rp = _n_tokens - 1;

	while (lp < _n_tokens && _tokens[lp].op != OP_LSQB) lp++;
	while (rp >= 0 && _tokens[rp].op != OP_RSQB) rp--;
	
	while (lp < rp) {
		_tokens[lp].meta.jmp = rp;
		_tokens[rp].meta.jmp = lp;
		lp++; rp--;
		while (lp < _n_tokens && _tokens[lp].op != OP_LSQB) lp++;
		while (rp >= 0 && _tokens[rp].op != OP_RSQB) rp--;
	}
}

bool token_populate(char *file_path, Token **tokens, int *n_tokens) {
	int _n_tokens = 0;
	Token *_tokens = NULL;

	FILE *fp = fopen(file_path, "r");
	if (fp == NULL) {
		printf("Could not open file: %s\n", file_path);
		return false;
	}

	int bracket_match = 0;
	char c;
	Operator op;

	while (fread(&c, sizeof(char), 1, fp) >= 1) {
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


		if (bracket_match < 0) {
			goto unmatched_brackets_found;
		}

		if (op != OP_INVALID && op != OP_QUOTE) {
			if (_n_tokens > 0) {
				if (op == _tokens[_n_tokens-1].op && op != OP_LSQB && op != OP_RSQB) {
					_tokens[_n_tokens-1].meta.reps++;
#ifdef DEBUG
					printf("Increase token: %c [%lu]\n", op_to_char(op), _tokens[_n_tokens-1].meta.reps);
#endif
				} else {
					goto create_new_token;
				}
			} else {
create_new_token:
				_n_tokens++;
				_tokens = (Token *)realloc(_tokens, sizeof(Token) * _n_tokens);
#ifdef DEBUG
				printf("Created new token: %c\n", op_to_char(op));
#endif
				_tokens[_n_tokens-1] = (Token){
					.op = op,
					.meta.reps = 1,
				};
			}
		}
	}

	if (bracket_match != 0) {
unmatched_brackets_found:
		printf("Unmatched square brackets: %d\n", bracket_match);
		free(_tokens);
		fclose(fp);
		return false;
	}

	_token_calculate_jmp(_tokens, _n_tokens);	

	*n_tokens = _n_tokens;
	*tokens = _tokens;

	return true;
}

void token_free(Token *_tokens) {
	free(_tokens);
}


