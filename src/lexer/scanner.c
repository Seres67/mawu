#include "lexer/identifiers.h"
#include <ctype.h>
#include <errors.h>
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

char scanner_peek(mawu_scanner *scanner)
{
    if (scanner_is_at_end(scanner))
        return '\n';
    return scanner->source[scanner->_current];
}

char scanner_peek_next(mawu_scanner *scanner)
{
    if (scanner->_current + 1 >= scanner->size)
        return '\0';
    return scanner->source[scanner->_current + 1];
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

void _scanner_string(mawu_scanner *scanner)
{
    while (scanner_peek(scanner) != '"' && !scanner_is_at_end(scanner)) {
        if (scanner_peek(scanner) == '\n') {
            ++scanner->_line;
        }
        scanner_next(scanner);
    }
    if (scanner_is_at_end(scanner)) {
        error(scanner->_line, "Unterminated string\n");
        return;
    }
    scanner_next(scanner);
    char *value = malloc((scanner->_current - 1) - (scanner->_start + 1) + 1);
    if (!value)
        return;
    memcpy(value, &scanner->source[scanner->_start + 1], scanner->_current - 1);
    scanner_add_token_literal(scanner, MAWU_STRING, &value);
}

_Bool is_digit(char c) { return c >= '0' && c <= '9'; }

void _scanner_number(mawu_scanner *scanner)
{
    while (is_digit(scanner_peek(scanner)))
        scanner_next(scanner);
    if (scanner_peek(scanner) == '.' && is_digit(scanner_peek_next(scanner))) {
        scanner_next(scanner);
        while (is_digit(scanner_peek(scanner))) {
            scanner_next(scanner);
        }
    }
    char *value = malloc(scanner->_current - scanner->_start + 1);
    if (!value)
        return;
    memcpy(value, &scanner->source[scanner->_start], scanner->_current);
    value[scanner->_current - scanner->_start] = '\0';

    scanner_add_token_literal(scanner, MAWU_NUMBER, (void *)atoi(value));
}

void _scanner_identifier(mawu_scanner *scanner)
{
    while (isalnum(scanner_peek(scanner)))
        scanner_next(scanner);
    char *value = malloc(scanner->_current - scanner->_start + 1);
    if (!value)
        return;
    memcpy(value, &scanner->source[scanner->_start], scanner->_current);
    value[scanner->_current - scanner->_start] = '\0';
    enum mawu_token_type type = MAWU_NONE;
    for (int i = 0; IDEN_TO_TOKEN[i].identifier; ++i)
        if (!strcmp(IDEN_TO_TOKEN[i].identifier, value))
            type = IDEN_TO_TOKEN[i].token;
    scanner_add_token(scanner, type);
}

struct char_to_token_enum
{
    char c;
    enum mawu_token_type t;
};

static struct char_to_token_enum CHAR_TO_TOKEN[] = {{'(', MAWU_LEFT_PAREN},
                                                    {')', MAWU_RIGHT_PAREN},
                                                    {'=', MAWU_EQUAL},
                                                    {0, MAWU_NONE}};

static void _scanner_scan_token(mawu_scanner *scanner)
{
    char c = scanner_next(scanner);
    // for (int i = 0; CHAR_TO_TOKEN[i].c; ++i) {
    //     if (c == CHAR_TO_TOKEN[i].c) {
    //         scanner_add_token(scanner, CHAR_TO_TOKEN[i].t);
    //     }
    // }
    switch (c) {
    case '{':
        scanner_add_token(scanner, MAWU_LEFT_BRACE);
        break;
    case '}':
        scanner_add_token(scanner, MAWU_RIGHT_BRACE);
        break;
    case ',':
        scanner_add_token(scanner, MAWU_COMMA);
        break;
    case '.':
        scanner_add_token(scanner, MAWU_DOT);
        break;
    case '-':
        scanner_add_token(scanner, MAWU_MINUS);
        break;
    case '+':
        scanner_add_token(scanner, MAWU_PLUS);
        break;
    case '*':
        scanner_add_token(scanner, MAWU_STAR);
        break;
    case ';':
        scanner_add_token(scanner, MAWU_SEMICOLON);
        break;
    case '(':
        scanner_add_token(scanner, MAWU_LEFT_PAREN);
        break;
    case ')':
        scanner_add_token(scanner, MAWU_RIGHT_PAREN);
        break;
    case '!':
        scanner_add_token(scanner, _scanner_match(scanner, '=')
                                       ? MAWU_BANG_EQUAL
                                       : MAWU_BANG);
        break;
    case '=':
        scanner_add_token(scanner, _scanner_match(scanner, '=')
                                       ? MAWU_EQUAL_EQUAL
                                       : MAWU_EQUAL);
        break;
    case '<':
        scanner_add_token(scanner, _scanner_match(scanner, '=')
                                       ? MAWU_LESS_EQUAL
                                       : MAWU_LESS);
        break;
    case '>':
        scanner_add_token(scanner, _scanner_match(scanner, '=')
                                       ? MAWU_GREATER_EQUAL
                                       : MAWU_GREATER);
        break;
    case '/':
        if (_scanner_match(scanner, '/')) {
            while (scanner_peek(scanner) != '\n' && !scanner_is_at_end(scanner))
                scanner_next(scanner);
        } else {
            scanner_add_token(scanner, MAWU_SLASH);
        }
        break;
    case ' ':
    case '\r':
    case '\t':
        break;
    case '\n':
        ++scanner->_line;
        break;
    case '"':
        _scanner_string(scanner);
        break;
    default:
        if (is_digit(c)) {
            _scanner_number(scanner);
        } else if (isalpha(c) || c == '_') {
            _scanner_identifier(scanner);
        } else {
            fprintf(stderr, "unrecognized char at %d:%d\n", scanner->_line,
                    scanner->_current);
        }
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
