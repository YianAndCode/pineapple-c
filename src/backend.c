#include "backend.h"
#include "parser.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

GlobalVariables* NewGlobalVariables()
{
    GlobalVariables* g = (GlobalVariables*)malloc(sizeof(GlobalVariables));
    g->vars = createHashMap();

    return g;
}

void Execute(char* string)
{
    source_code_t ast;
    ast.statements = calloc(1, sizeof(statement_t*));
    GlobalVariables* g = NewGlobalVariables();

    if (!parse(string, &ast)) {
        exit(-1);
    }

    resolveAST(g, &ast);

    free(g);
}

bool resolveAST(GlobalVariables* g, source_code_t* ast)
{
    if (ast->statements_count == 0) {
        printf("resolveAST(): no code to execute, please check your input.\n");
        return false;
    }
    for (int i = 0; i < ast->statements_count; i++) {
        if (!resolveStatement(g, ast->statements[i])) {
            return false;
        }
    }

    return true;
}

bool resolveStatement(GlobalVariables* g, statement_t* statement)
{
    switch (statement->type) {
        case STATEMENT_ASSIGNMENT:
            return resolveAssignment(g, (assignment_statement_t*)statement);
        case STATEMENT_PRINT:
            return resolvePrint(g, (print_statement_t*)statement);
        default:
            printf("resolveStatement(): undefined statement type.\n");
            return false;
    }
}

bool resolveAssignment(GlobalVariables* g, assignment_statement_t* assignment)
{
    char* var_name = assignment->base.var->name;
    if (strcmp(var_name, "") == 0) {
        printf("resolveAssignment(): variable name can NOT be empty.\n");
        return false;
    }

    g->vars->put(g->vars, var_name, assignment->string);
    return true;
}

bool resolvePrint(GlobalVariables* g, print_statement_t* print)
{
    char* var_name = print->base.var->name;
    if (strcmp(var_name, "") == 0) {
        printf("resolvePrint(): variable name can NOT be empty.\n");
        return false;
    }

    char* value = g->vars->get(g->vars, var_name);
    if (value == NULL) {
        printf("resolvePrint(): variable %s is not defined.\n", var_name);
        return false;
    }

    printf("%s\n", value);
    return true;
}
