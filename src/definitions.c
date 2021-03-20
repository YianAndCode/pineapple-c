#include "definitions.h"
#include <stdlib.h>

token_t* NewToken(int line_num, int token_type, char* token)
{
    token_t* t = (token_t*)calloc(1, sizeof(token_t));
    if (t != NULL) {
        t->line_num   = line_num;
        t->token_type = token_type;
        t->token      = token;
    }

    return t;
}

void FreeToken(token_t* token)
{
    if (token != NULL) {
        free(token);
    }
}