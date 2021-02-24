#ifndef DEFINITIONS_H
#define DEFINITIONS_H

typedef struct {
    int   line_num;
    int   token_type;
    char* token;
} token;

typedef struct {
    int   line_num;
    char* name;
} variable;

typedef struct {
    int       type;
    int       line_num;
    variable* var;
} statement;

typedef struct {
    statement* base;
} print_statement;

typedef struct {
    statement* base;
    char*      string;
} assignment_statement;

typedef struct {
    int         line_num;
    statement** statements;
} source_code;

#endif
