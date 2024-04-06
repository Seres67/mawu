#include "lexer/tokens.h"
#include <lexer/scanner.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

mawu_scanner *scanner_new(char *source)
{
    mawu_scanner *scanner = malloc(sizeof(mawu_scanner));
    if (!scanner)
        return NULL;
    scanner->source = source;
    scanner->_current = 0;
    scanner->_start = 0;
    scanner->_line = 0;
    scanner->size = strlen(source);
    scanner->tokens = malloc(sizeof(mawu_token *));
    scanner->tokens[0] = NULL;
    scanner->_token_count = 1;
    return scanner;
}

void scanner_delete(mawu_scanner *scanner)
{
    free(scanner->source);
    for (int i = 0; scanner->tokens[i]; ++i) {
        free(scanner->tokens[i]);
    }
    free(scanner->tokens);
}

char scanner_next(mawu_scanner *scanner)
{
    return scanner->source[scanner->_current++];
}

void scanner_add_token_literal(mawu_scanner *scanner,
                               enum mawu_token_type token_type, void *literal)
{
    char *text =
        malloc(sizeof(char) * (scanner->_current - scanner->_start + 1));
    memcpy(text, &scanner->source[scanner->_start],
           scanner->_current - scanner->_start);
    mawu_token *token = token_new(token_type, text, literal, scanner->_line);
    scanner->tokens = realloc(scanner->tokens, ++scanner->_token_count);
    if (!scanner->tokens)
        return;
    scanner->tokens[scanner->_token_count - 1] = token;
}

void scanner_add_token(mawu_scanner *scanner, enum mawu_token_type token_type)
{
    scanner_add_token_literal(scanner, token_type, NULL);
}

static void _scanner_scan_token(mawu_scanner *scanner)
{
    char c = scanner_next(scanner);
    switch (c) {
    case '(':
        scanner_add_token(scanner, LEFT_PAREN);
        break;
    default:
        fprintf(stderr, "unrecognized char line %d char %d\n", scanner->_line,
                scanner->_current);
    }
}

mawu_token *scanner_scan_tokens(mawu_scanner *scanner)
{
    while (!scanner_is_at_end(scanner)) {
        scanner->_start = scanner->_current;
        _scanner_scan_token(scanner);
    }
}

_Bool is_at_end(mawu_scanner *scanner)
{
    return scanner->_current >= scanner->size;
}
