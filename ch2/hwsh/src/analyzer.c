#include <stdio.h>
#include <stdlib.h>
#include "hist.h"

/*
 * Generate token from char in input stream. Pass them to parser.
 *
 * @param   ch      a single char got from input stream
 * @return  1 if continue to read from input, else 0
 */
int scanner(char ch){
    static char quote = '\0';
    if(quote != '\0'){

    } else {
        switch (ch) {
            case '\t':
            case ' ':
            case '|':
                cur_newarg();
                cur_inschar(ch);
                break;
            case '\'':
            case '\"':
                quote = ch;
                break;
            default:
                cur_inschar(ch);
        }
    }
}

int parse(const char *line, size_t len, char *argv[], int *argc) {
    const char *start = line, *end = line;
    char quote = '\0';
    for (const char *ptr = line; ptr - line < len; ptr++) {
        switch (*ptr) {
            case ' ':
                start = ptr + 1;
                break;
            case '\'':
            case '\"':
                start = ptr;
                quote = *ptr;
                do{
                    ptr++;
                    if(ptr - line < len) {
                        fprintf(stderr, "Quote not match! ");
                        return 1;
                    }
                } while(*ptr != quote);
                end = ptr;
                (*argc)--;
                start = ptr + 1;
                break;
            case '\n':
            default:
                end = ptr;
        }
    }
    return 0;
}
