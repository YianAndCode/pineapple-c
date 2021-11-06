#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "backend.h"

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s <filename>\n", argv[0]);
        return EXIT_FAILURE;
    }
    char* filename = argv[1];
    FILE* fp = fopen(filename, "r");
    if (fp == NULL)
    {
        printf("Error: Could not open file %s\n", filename);
        return EXIT_FAILURE;
    }

    fseek(fp, 0, SEEK_END);
    int filesize = ftell(fp);
    rewind(fp);

    char* source_code = (char*)calloc(sizeof(char), filesize + 1);
    char* tmp = (char*)calloc(sizeof(char), 1024);
    while((fgets(tmp, 1024, fp)) != NULL){
        strcat(source_code, tmp);
    }
    fclose(fp);
    free(tmp);

    Execute(source_code);

    return 0;
}