#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <regex.h>
#include "lexer.h"
#include "definitions.h"

const char* RegexName = "^[A-Za-z0-9_]+";

regex_t* nameRegex;

Lexer* NewLexer(char* source_code)
{
    Lexer* lexer = (Lexer*)calloc(sizeof(Lexer), 1);
    if (lexer == NULL) {
        printf("calloc() for Lexer failed\n");
        exit(-1);
    }
    lexer->source_code = source_code;
    return lexer;
}

int GetLineNum(Lexer* lexer)
{
    return lexer->line_num;
}

void skipSourceCode(Lexer* lexer, int n)
{
    //
}

bool isIgnored(Lexer* lexer)
{
    // 
}

void scan(Lexer* lexer, regex_t* regex, char* result, int* length)
{
    regmatch_t pm[1];
    int status;

    status = regexec(regex, lexer->source_code, 1, pm, 0);
    if (! status) {
        *length = pm[0].rm_eo - pm[0].rm_so + 1;

        result = (char*)realloc(result, sizeof(char) * (*length));
        memcpy(result, lexer->source_code + pm[0].rm_so, *length);

        skipSourceCode(lexer, *length);
        return;
    } else {
        printf("unreachable\n");
        exit(-1);
    }
}

void scanName(Lexer* lexer, char* name, int* length)
{
    if (nameRegex == NULL) {
        nameRegex = (regex_t*)calloc(sizeof(regex_t), 1);
    }
    regcomp(nameRegex, RegexName, REG_EXTENDED);

    scan(lexer, nameRegex, name, length);

    regfree(nameRegex);
}

token_t GetNextToken(Lexer* lexer)
{
    if (lexer->next_token_line_num > 0) {
        token_t token;
        token.line_num   = lexer->next_token_line_num;
        token.token_type = lexer->next_token_type;
        token.token      = lexer->next_token;

        lexer->line_num = lexer->next_token_line_num;
        lexer->next_token_line_num = 0;

        return token;
    } else {
        return MatchToken(lexer);
    }
}

token_t MatchToken(Lexer* lexer)
{
    // 
}

bool isLetter(char c)
{
    return c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z';
}