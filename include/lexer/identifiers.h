#ifndef MAWU_IDENTIFIERS_H
#define MAWU_IDENTIFIERS_H

#include <lexer/tokens.h>
#include <unistd.h>

struct identifier_token_s
{
    char *identifier;
    enum mawu_token_type token;
};

static struct identifier_token_s IDEN_TO_TOKEN[] = {
    {"else", MAWU_ELSE},     {"false", MAWU_FALSE}, {"for", MAWU_FOR},
    {"fun", MAWU_FUN},       {"if", MAWU_IF},       {"null", MAWU_NULL},
    {"return", MAWU_RETURN}, {"true", MAWU_TRUE},   {"var", MAWU_VAR},
    {"while", MAWU_WHILE},   {NULL, MAWU_NONE}};

#endif // !MAWU_IDENTIFIERS_H
