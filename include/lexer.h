#include <stdbool.h>
#include <regex.h>
#include "definitions.h"
#include "cHashMap/cHashMap.h"

#ifndef LEXER_H
#define LEXER_H

#define COUNT_TOKENS 10

enum TOKEN
{
    TOKEN_EOF,         // end-of-file
    TOKEN_VAR_PREFIX,  // $
    TOKEN_LEFT_PAREN,  // (
    TOKEN_RIGHT_PAREN, // )
    TOKEN_EQUAL,       // =
    TOKEN_QUOTE,       // "
    TOKEN_DUOQUOTE,    // ""
    TOKEN_NAME,        // Name ::= [_A-Za-z][_0-9A-Za-z]*
    TOKEN_PRINT,       // print
    TOKEN_IGNORED      // Ignored
};

typedef struct {
    char*   source_code;
    int     line_num;
    char*   next_token;
    int     next_token_type;
    int     next_token_line_num;
    char*   TokenMap[COUNT_TOKENS];
    HashMap KeywordMap;
} Lexer;

Lexer* NewLexer(char* source_code);

int GetLineNum(Lexer* lexer);

token_t NextTokenIs(Lexer* lexer, int token_type);

void LookAheadAndSkip(Lexer* lexer, int expected_type);

int LookAhead(Lexer* lexer);

bool nextSourceCodeIs(Lexer* lexer, char* s);

void skipSourceCode(Lexer* lexer, int n);

bool isIgnored(Lexer* lexer);

void scan(Lexer* lexer, regex_t* regex, char* result, int* length);

char* scanBeforeToken(Lexer* lexer, char* token);

void scanName(Lexer* lexer, char* name, int* length);

token_t GetNextToken(Lexer* lexer);

token_t MatchToken(Lexer* lexer);

bool isLetter(char c);

#endif