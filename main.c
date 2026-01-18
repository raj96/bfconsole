#include <stdio.h>
#include "vm/token.h"
#include "vm/executor.h"

int main(void) {
	Token *tokens;
	int n;
	if (!token_populate("test.bf", &tokens, &n)) { 
		printf("Population failed\n");
		return 1; 
	}
#ifdef DEBUG
	tokens_print(tokens, n);
#endif
	executor_init(tokens, n);
	executor_run();
	executor_free();
	return 0;
}

