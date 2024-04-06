#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <lexer/tokens.h>

mawu_token *token_new(enum mawu_token_type type, char *text, char *literal,
                      int line)
{
    mawu_token *token = malloc(sizeof(mawu_token));
    if (!token) {
        return NULL;
    }
    token->type = type;
    token->lexeme = text;
    token->literal = literal;
    token->line = line;
    return token;
}

char *mawu_token_to_string(mawu_token *token)
{
    char str[2];
    sprintf(str, "%d", token->type);

    char *out = malloc(strlen(str) + strlen(token->lexeme) +
                       strlen(token->literal) + 3);
    if (!out) {
        return NULL;
    }

    strcat(out, str);
    strcat(out, token->lexeme);
    strcat(out, token->literal);
    return out;
}
