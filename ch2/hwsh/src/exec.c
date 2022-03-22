#include <stdio.h>
#include "exec.h"

// Execute a given series of Tasks

int exec(Task *task, size_t len) {
    printf("%s:\n", "argv");
#ifdef DEBUG
    for (int i = 0; i < current_line->argc; i++) {
        printf("\t%s\n", current_line->argv[i]);
    }
#endif
    return 0;
}
