#include <stdio.h>
#include "vm/token.h"

int main() {
	Token *tokens;
	int n;
	if (!token_populate("test.bf", &tokens, &n)) { 
		printf("Population failed\n");
		return 1; 
	}
#ifdef DEBUG
	tokens_print(tokens, n);
#endif
	token_free(tokens);
	return 0;
}

