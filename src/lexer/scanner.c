#include <lexer/scanner.h>
#include <lexer/tokens.h>
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
    scanner->_token_count = 0;
    return scanner;
}

void scanner_delete(mawu_scanner *scanner)
{
    free(scanner->source);
    for (int i = 0; scanner->tokens[i]; ++i) {
        if (scanner->tokens[i]->lexeme)
            free(scanner->tokens[i]->lexeme);
        free(scanner->tokens[i]);
    }
    free(scanner->tokens);
    free(scanner);
}

char scanner_next(mawu_scanner *scanner)
{
    return scanner->source[scanner->_current++];
}

// returns 1 on error
_Bool scanner_add_token_literal(mawu_scanner *scanner,
                                enum mawu_token_type token_type, void *literal)
{
    char *text =
        malloc(sizeof(char) * (scanner->_current - scanner->_start + 1));
    memcpy(text, &scanner->source[scanner->_start],
           scanner->_current - scanner->_start);
    text[scanner->_current - scanner->_start] = 0;
    mawu_token *token = token_new(token_type, text, literal, scanner->_line);
    if (!token)
        return 1;
    scanner->tokens = realloc(scanner->tokens, (++scanner->_token_count + 1) *
                                                   sizeof(mawu_token *));
    if (!scanner->tokens)
        return 1;
    scanner->tokens[scanner->_token_count - 1] = token;
    scanner->tokens[scanner->_token_count] = NULL;
    return 0;
}

_Bool scanner_add_token(mawu_scanner *scanner, enum mawu_token_type token_type)
{
    return scanner_add_token_literal(scanner, token_type, NULL);
}

_Bool _scanner_match(mawu_scanner *scanner, char expected)
{
    if (scanner_is_at_end(scanner))
        return 0;
    if (scanner->source[scanner->_current] != expected)
        return 0;

    ++scanner->_current;
    return 1;
}

static void _scanner_scan_token(mawu_scanner *scanner)
{
    char c = scanner_next(scanner);
    switch (c) {
    case '(':
        scanner_add_token(scanner, MAWU_LEFT_PAREN);
        break;
    case ')':
        scanner_add_token(scanner, MAWU_RIGHT_PAREN);
        break;
    case '=':
        scanner_add_token(scanner, _scanner_match(scanner, '=')
                                       ? MAWU_EQUAL_EQUAL
                                       : MAWU_EQUAL);
        break;
    default:
        fprintf(stderr, "unrecognized char at %d:%d\n", scanner->_line,
                scanner->_current);
    }
}

mawu_token **scanner_scan_tokens(mawu_scanner *scanner)
{
    while (!scanner_is_at_end(scanner)) {
        scanner->_start = scanner->_current;
        _scanner_scan_token(scanner);
    }
    mawu_token *token = token_new(MAWU_EOF, NULL, NULL, scanner->_line);
    if (!token)
        return NULL;
    scanner->tokens = realloc(scanner->tokens, (++scanner->_token_count + 1) *
                                                   sizeof(mawu_token *));
    if (!scanner->tokens)
        return NULL;
    scanner->tokens[scanner->_token_count - 1] = token;
    scanner->tokens[scanner->_token_count] = NULL;

    return scanner->tokens;
}

_Bool scanner_is_at_end(mawu_scanner *scanner)
{
    return scanner->_current >= scanner->size;
}
