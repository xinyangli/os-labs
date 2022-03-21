#include <stdlib.h>
#include "hist.h"

CurrentLine *current_line;
int cur_pos, cur_ipos;
/*
 * Enlarge string argv[cur_pos]
 *
 * @return  Return 0 if succeeded. If not, return 1.
 */
int cur_resize(void) {

    realloc(current_line->argv[cur_pos], 2 * current_line->str_size[cur_pos]);
}

/*
 * Insert character to cursor position at current line. Move cursor to new position.
 *
 * @param   ch      character to add
 * @return  If succeeded, return 0. If not, return -1.
 */
int cur_inschar(char ch) {
    char * new_buf = NULL;
    if (current_line->str_size[cur_pos] <= current_line->str_len[cur_pos] + 1) {
        /* Enlarge string */
        new_buf = realloc(current_line->argv[cur_pos], 2 * current_line->str_size[cur_pos]);
        if(new_buf == NULL) {
            return -1;
        } else {
            current_line->argv[cur_pos] = new_buf;
        }
    }
    for(int i = current_line->str_len[cur_pos]; i > cur_ipos; i--){
        current_line->argv[i] = current_line->argv[i - 1];
    }
    return 0;
}

/*
 * Remove the character before current cursor position. Move cursor to new position.
 *
 * @return  If succeeded, return 0. If already at the start, return 1. For unknown reason, return -1
 */
int cur_rmchar(void) {
    if (cur_pos == 0 && cur_ipos == 0)
        return 1;
    if (cur_ipos == 0) {
      ;
    }
    return 0;
}

/*
 * Create a new argument.
 */
int cur_newarg(void) {
    return 0;
}

/*
 * Archive current line to history.
 */
int archive_line(void) {
    return 0;
}