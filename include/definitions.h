#ifndef DEFINITIONS_H
#define DEFINITIONS_H

typedef struct {
    int   line_num;
    int   token_type;
    char* token;
} token_t;

typedef struct {
    int   line_num;
    char* name;
} variable_t;

typedef struct {
    int         type;
    int         line_num;
    variable_t* var;
} statement_t;

typedef struct {
    statement_t* base;
} print_statement_t;

typedef struct {
    statement_t* base;
    char*        string;
} assignment_statement_t;

typedef struct {
    int           line_num;
    statement_t** statements;
} source_code_t;
    int         line_num;
    statement** statements;
} source_code;

#endif
