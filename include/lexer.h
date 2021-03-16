#include "definitions.h"

#ifndef LEXER_H
#define LEXER_H

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
    char* source_code;
    int   line_num;
    char* next_token;
    int   next_token_type;
    int   next_token_line_num;
} Lexer;

Lexer* NexLexer(char* source_code);

int GetLineNum(Lexer* lexer);

token NextTokenIs(Lexer* lexer, int token_type);

void LookAheadAndSkip(Lexer* lexer, int expected_type);

int LookAhead(Lexer* lexer);

bool nextSourceCodeIs(Lexer* lexer, char* s);

void skipSourceCode(Lexer* lexer, int n);

bool isIgnored(Lexer* lexer);

char* scan(Lexer* lexer); // TODO: regex

char* scanBeforeToken(Lexer* lexer, char* token);

char* scanName(Lexer* lexer);

token GetNextToken(Lexer* lexer);

token MatchToken(Lexer* lexer);

bool isLetter(char c);

#endif