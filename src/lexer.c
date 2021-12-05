#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <regex.h>
#include "lexer.h"
#include "definitions.h"

const char* RegexName = "^[A-Za-z0-9_]+";

regex_t* nameRegex;


token_t pack_token(int line_num, int token_type, char* token)
{
    token_t packed = {
        .line_num   = line_num,
        .token_type = token_type,
        .token      = token
    };
    return packed;
}

void initTokenMap(Lexer* lexer)
{
    lexer->TokenMap[TOKEN_EOF]         = "EOF";
    lexer->TokenMap[TOKEN_VAR_PREFIX]  = "$";
    lexer->TokenMap[TOKEN_LEFT_PAREN]  = "(";
    lexer->TokenMap[TOKEN_RIGHT_PAREN] = ")";
    lexer->TokenMap[TOKEN_EQUAL]       = "=";
    lexer->TokenMap[TOKEN_QUOTE]       = "\"";
    lexer->TokenMap[TOKEN_DUOQUOTE]    = "\"\"";
    lexer->TokenMap[TOKEN_NAME]        = "Name";
    lexer->TokenMap[TOKEN_PRINT]       = "print";
    lexer->TokenMap[TOKEN_IGNORED]     = "Ignored";
}

void initKeywordMap(Lexer* lexer)
{
    HashMap map = createHashMap();
    map->put(map, "print", (void*)TOKEN_PRINT);

    lexer->KeywordMap = map;
}

Lexer* NewLexer(char* source_code)
{
    Lexer* lexer = (Lexer*)calloc(sizeof(Lexer), 1);
    if (lexer == NULL) {
        printf("calloc() for Lexer failed\n");
        exit(-1);
    }
    lexer->source_code = source_code;
    lexer->line_num = 1;
    initTokenMap(lexer);
    initKeywordMap(lexer);
    return lexer;
}

int GetLineNum(Lexer* lexer)
{
    return lexer->line_num;
}

token_t NextTokenIs(Lexer* lexer, int token_type)
{
    token_t nextToken = GetNextToken(lexer);
    if (nextToken.token_type != token_type) {
        printf(
            "NextTokenIs(): syntax error nerar '%s', expected token: {%s} but got {%s}.\n",
            nextToken.token, lexer->TokenMap[token_type], lexer->TokenMap[nextToken.token_type]
        );
        exit(-1);
    }

    return nextToken;
}

void LookAheadAndSkip(Lexer* lexer, int token_type)
{
    int now_line_num = lexer->line_num;
    token_t nextToken = GetNextToken(lexer);
    // not expected type, reverser cursor
    if (nextToken.token_type != token_type) {
        lexer->line_num            = now_line_num;
        lexer->next_token          = nextToken.token;
        lexer->next_token_line_num = nextToken.line_num;
        lexer->next_token_type     = nextToken.token_type;
    }
}

int LookAhead(Lexer* lexer)
{
    if (lexer->next_token_line_num > 0) {
        return lexer->next_token_type;
    }

    int now_line_num = lexer->line_num;
    token_t nextToken = GetNextToken(lexer);
    lexer->line_num            = now_line_num;
    lexer->next_token          = nextToken.token;
    lexer->next_token_line_num = nextToken.line_num;
    lexer->next_token_type     = nextToken.token_type;

    return lexer->next_token_type;
}

bool nextSourceCodeIs(Lexer* lexer, char* s)
{
    return strncmp(lexer->source_code, s, strlen(s)) == 0;
}

void skipSourceCode(Lexer* lexer, int n)
{
    lexer->source_code += n;
}

static bool isNewLine(char c)
{
    return c == '\n' || c == '\r';
}

static bool isWhiteSpace(char c)
{
    switch (c) {
        case '\t':
        case '\n':
        case '\v':
        case '\f':
        case '\r':
        case ' ':
            return true;
        default:
            return false;
    }
}

bool isIgnored(Lexer* lexer)
{
    bool is_ignore = false;

    while (strlen(lexer->source_code) > 0) {
        if (nextSourceCodeIs(lexer, "\r\n") || nextSourceCodeIs(lexer, "\n\r")) {
            skipSourceCode(lexer, 2);
            lexer->line_num++;
            is_ignore = true;
        } else if (isNewLine(*lexer->source_code)) {
            skipSourceCode(lexer, 1);
            lexer->line_num++;
            is_ignore = true;
        } else if (isWhiteSpace(*lexer->source_code)) {
            skipSourceCode(lexer, 1);
            is_ignore = true;
        } else {
            break;
        }
    }

    return is_ignore;
}

void scan(Lexer* lexer, regex_t* regex, char* result, int* length)
{
    regmatch_t pm[1];
    int status;

    status = regexec(regex, lexer->source_code, 1, pm, 0);
    if (! status) {
        *length = pm[0].rm_eo - pm[0].rm_so;

        result = (char*)realloc(result, sizeof(char) * (*length + 1));
        memcpy(result, lexer->source_code + pm[0].rm_so, *length);
        result[*length] = '\0';

        skipSourceCode(lexer, *length);
        return;
    } else {
        printf("unreachable\n");
        exit(-1);
    }
}

char* scanBeforeToken(Lexer* lexer, char* token)
{
    for (int i = 0; i <= strlen(lexer->source_code) - strlen(token); i++) {
        if (strncmp(lexer->source_code + i, token, strlen(token)) == 0) {
            char* result = (char*)calloc(sizeof(char), i - 1);
            memcpy(result, lexer->source_code, i - 1);
            skipSourceCode(lexer, i);
            return result;
        }
    }

    printf("Unreachable!\n");
    exit(-1);
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
    token_t token;

    if (isIgnored(lexer)) {
        token = pack_token(
            lexer->line_num,
            TOKEN_IGNORED,
            lexer->TokenMap[TOKEN_IGNORED]
        );
        return token;
    }

    if (lexer->source_code[0] == '\0') {
        token = pack_token(
            lexer->line_num,
            TOKEN_EOF,
            lexer->TokenMap[TOKEN_EOF]
        );
        return token;
    }

    switch (lexer->source_code[0]) {
        case '$':
            skipSourceCode(lexer, 1);
            token = pack_token(
                lexer->line_num,
                TOKEN_VAR_PREFIX,
                lexer->TokenMap[TOKEN_VAR_PREFIX]
            );
            return token;

        case '(':
            skipSourceCode(lexer, 1);
            token = pack_token(
                lexer->line_num,
                TOKEN_LEFT_PAREN,
                lexer->TokenMap[TOKEN_LEFT_PAREN]
            );
            return token;

        case ')':
            skipSourceCode(lexer, 1);
            token = pack_token(
                lexer->line_num,
                TOKEN_RIGHT_PAREN,
                lexer->TokenMap[TOKEN_RIGHT_PAREN]
            );
            return token;

        case '=':
            skipSourceCode(lexer, 1);
            token = pack_token(
                lexer->line_num,
                TOKEN_EQUAL,
                lexer->TokenMap[TOKEN_EQUAL]
            );
            return token;

        case '"':
            if (nextSourceCodeIs(lexer, "\"\"")) {
                skipSourceCode(lexer, 2);
                token = pack_token(
                    lexer->line_num,
                    TOKEN_DUOQUOTE,
                    lexer->TokenMap[TOKEN_DUOQUOTE]
                );
                return token;
            }
            skipSourceCode(lexer, 1);
            token = pack_token(
                lexer->line_num,
                TOKEN_QUOTE,
                lexer->TokenMap[TOKEN_QUOTE]
            );
            return token;
    } // switch

    if (lexer->source_code[0] == '_' || isLetter(lexer->source_code[0])) {
        char* name = malloc(sizeof(char) * 1);
        int length;
        scanName(lexer, name, &length);

        void* keyword = lexer->KeywordMap->get(lexer->KeywordMap, name);
        if (keyword != NULL) {
            token = pack_token(
                lexer->line_num,
                (int)keyword,
                name
            );
            return token;
        } else {
            token = pack_token(
                lexer->line_num,
                TOKEN_NAME,
                name
            );
            return token;
        } // else
    }

    printf("matchToken(): unexpected symbol near '%c'.\n", lexer->source_code[0]);
    exit(-1);
}

bool isLetter(char c)
{
    return c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z';
}
