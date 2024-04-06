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
    char *out;
    for (size_t i = 0; tokens[i]; ++i) {
        out = mawu_token_to_string(tokens[i]);
        printf("%s\n", out);
        free(out);
    }
    scanner_delete(scanner);
}

void mawu_repl() {}

void mawu_run_file(char *file)
{
    FILE *f = fopen(file, "r");
    if (!f) {
        fprintf(stderr, "couldn't open file");
        return;
    }
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);
    char *string = malloc(fsize + 1);
    fread(string, fsize, 1, f);
    fclose(f);

    string[fsize] = 0;
    run(string);
}

int main(int ac, char **av)
{
    if (ac == 1) {
        mawu_repl();
    } else if (ac == 2) {
        mawu_run_file(av[1]);
    }
    return 0;
}
