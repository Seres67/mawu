#include "lexer/scanner.h"
#include "lexer/tokens.h"
#include <stdio.h>
#include <stdlib.h>

void run(char *source)
{
    mawu_scanner *scanner = scanner_new(source);
    if (!scanner)
        return;
    mawu_token **tokens = scanner_scan_tokens(scanner);
    for (size_t i = 0; tokens[i]; ++i)
        printf("%s %d\n", tokens[i]->lexeme, tokens[i]->type);
}

void mawu_repl() {}

void mawu_run_file(char *file)
{
    FILE *f = fopen(file, "r");
    if (!f) {
        fprintf(stderr, "couldn't open file");
        return;
    }
    // int read;
    // char *line = NULL;
    // size_t len = 0;
    //
    // while ((read = getline(&line, &len, f))) {
    // }
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET); /* same as rewind(f); */

    char *string = malloc(fsize + 1);
    fread(string, fsize, 1, f);
    fclose(f);

    string[fsize] = 0;
    run(string);
}

int main(int ac, char **av)
{
    printf("Hello world!\n");
    if (ac == 1) {
        mawu_repl();
    } else if (ac == 2) {
        mawu_run_file(av[1]);
    }
    return 0;
}
