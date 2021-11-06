#include "cHashMap/cHashMap.h"
#include "definitions.h"

#ifndef BACKEND_H
#define BACKEND_H

typedef struct {
    HashMap vars;
} GlobalVariables;

GlobalVariables* NewGlobalVariables();

void Execute(char* string);

bool resolveAST(GlobalVariables* g, source_code_t* ast);

bool resolveStatement(GlobalVariables* g, statement_t* statement);

bool resolveAssignment(GlobalVariables* g, assignment_statement_t* assignment);

bool resolvePrint(GlobalVariables* g, print_statement_t* print);

#endif