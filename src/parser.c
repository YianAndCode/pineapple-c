#include "parser.h"
#include <stdlib.h>

bool parseName(Lexer *lexer, char *name)
{
    token_t token = NextTokenIs(lexer, TOKEN_NAME);

    name = token.token;

    return true;
}

bool parseString(Lexer *lexer, char *string)
{
    switch (LookAhead(lexer))
    {
        case TOKEN_DUOQUOTE:
            NextTokenIs(lexer, TOKEN_DUOQUOTE);
            LookAheadAndSkip(lexer, TOKEN_IGNORED);
            string = "";
            return true;

        case TOKEN_QUOTE:
            NextTokenIs(lexer, TOKEN_QUOTE);
            string = scanBeforeToken(lexer, lexer->TokenMap[TOKEN_QUOTE]);
            NextTokenIs(lexer, TOKEN_QUOTE);
            LookAheadAndSkip(lexer, TOKEN_IGNORED);
            return true;

        default:
            return false;
    }
}

bool parseVariable(Lexer *lexer, variable_t* variable)
{
    variable->line_num = GetLineNum(lexer);
    NextTokenIs(lexer, TOKEN_VAR_PREFIX);
    if (!parseName(lexer, variable->name))
    {
        return false;
    }

    LookAheadAndSkip(lexer, TOKEN_IGNORED);

    return true;
}

bool parseAssignment(Lexer *lexer, assignment_statement_t* assignment)
{
    assignment->base.line_num = GetLineNum(lexer);
    if (!parseVariable(lexer, assignment->base.var))
    {
        return false;
    }
    LookAheadAndSkip(lexer, TOKEN_IGNORED);
    NextTokenIs(lexer, TOKEN_EQUAL);
    LookAheadAndSkip(lexer, TOKEN_IGNORED);
    if (!parseString(lexer, assignment->string))
    {
        return false;
    }
    LookAheadAndSkip(lexer, TOKEN_IGNORED);
    return true;
}

bool parsePrint(Lexer *lexer, print_statement_t* print)
{
    print->base.line_num = GetLineNum(lexer);
    NextTokenIs(lexer, TOKEN_PRINT);
    NextTokenIs(lexer, TOKEN_LEFT_PAREN);
    LookAheadAndSkip(lexer, TOKEN_IGNORED);
    if (!parseVariable(lexer, print->base.var))
    {
        return false;
    }
    LookAheadAndSkip(lexer, TOKEN_IGNORED);
    NextTokenIs(lexer, TOKEN_RIGHT_PAREN);
    LookAheadAndSkip(lexer, TOKEN_IGNORED);
    return true;
}

bool parseStatement(Lexer *lexer, statement_t* statement)
{
    LookAheadAndSkip(lexer, TOKEN_IGNORED);

    switch (LookAhead(lexer))
    {
        case TOKEN_PRINT:
            statement->type = STATEMENT_PRINT;
            return parsePrint(lexer, (print_statement_t*)statement);

        case TOKEN_VAR_PREFIX:
            statement = realloc(statement, sizeof(assignment_statement_t));
            statement->type = STATEMENT_ASSIGNMENT;
            return parseAssignment(lexer, (assignment_statement_t*)statement);
    }

    return true;
}

bool parseStatements(Lexer *lexer, statement_t** statements, int* count)
{
    *count = 0;
    while (!isSourceCodeEnd(LookAhead(lexer)))
    {
        statement_t* statement = malloc(sizeof(statement_t));
        if (!parseStatement(lexer, statement))
        {
            return false;
        }
        statements[*count] = statement;
        (*count)++;
    }

    return true;
}

bool parseSourceCode(Lexer *lexer, source_code_t* source_code)
{
    source_code->line_num = GetLineNum(lexer);
    return parseStatements(
        lexer,
        source_code->statements,
        &source_code->statements_count
    );
}

bool isSourceCodeEnd(int token)
{
    return token == TOKEN_EOF;
}

bool parse(char* code, source_code_t* source_code)
{
    Lexer* lexer = NewLexer(code);
    bool result = parseSourceCode(lexer, source_code);
    if (!result)
    {
        return false;
    }
    NextTokenIs(lexer, TOKEN_EOF);
    return true;
}