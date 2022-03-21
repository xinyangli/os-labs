#include <stdio.h>
#include "global.h"

int exec(void) {
    printf("%s:\n", "argv");
#ifdef DEBUG
    for (int i = 0; i < current_line->argc; i++) {
        printf("\t%s\n", current_line->argv[i]);
    }
#endif
    return 0;
}
