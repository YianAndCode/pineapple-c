#include "lexer.h"
#include "definitions.h"

#ifndef PARSER_H
#define PARSER_H

// Name ::= [_A-Za-z][_0-9A-Za-z]*
bool parseName(Lexer *lexer, char *name);

// String ::= '"' '"' Ignored | '"' StringCharacter '"' Ignored
bool parseString(Lexer *lexer, char *string);

// Variable ::= "$" Name Ignored
bool parseVariable(Lexer *lexer, variable_t* variable);

// Assignment  ::= Variable Ignored "=" Ignored String Ignored
bool parseAssignment(Lexer *lexer, assignment_statement_t* assignment);

// Print ::= "print" "(" Ignored Variable Ignored ")" Ignored
bool parsePrint(Lexer *lexer, print_statement_t* print);

bool parseStatement(Lexer *lexer, statement_t* statement);

// Statement ::= Print | Assignment
bool parseStatements(Lexer *lexer, statement_t** statements, int* count);

// SourceCode ::= Statement+ 
bool parseSourceCode(Lexer *lexer, source_code_t* source_code);

bool isSourceCodeEnd(int token);

bool parse(char* code, source_code_t* source_code);

#endif // PARSER_H