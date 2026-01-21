#include <stdio.h>
#include "vm/token.h"
#include "vm/executor.h"

int main(int argc, char **argv) {
    if (argc <= 1) {
        printf("Usage: bfconsole <bf source file>\n");
        return 1;
    }
    Token *tokens;
    int n;
    if (!token_populate(argv[1], &tokens, &n)) { 
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

