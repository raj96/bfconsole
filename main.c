#include <stdio.h>
#include "vm/token.h"

int main() {
	if (!token_populate("test.bf")) { return 1; }
#ifdef DEBUG
	tokens_print();
#endif
	token_free();
	return 0;
}
