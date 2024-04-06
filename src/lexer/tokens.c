#include <lexer/tokens.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    char *out = malloc(1 + (token->lexeme ? strlen(token->lexeme) : 0) +
                       (token->literal ? strlen(token->literal) : 0) + 3);
    if (!out) {
        return NULL;
    }

    // +1 is for token type
    // +2 is for 2 spaces
    memset(out, 0,
           1 + (token->lexeme ? strlen(token->lexeme) : 0) +
               (token->literal ? strlen(token->literal) : 0) + 3);

    sprintf(out, "%d", token->type);
    if (token->lexeme) {
        strcat(out, " ");
        strcat(out, token->lexeme);
    }
    if (token->literal) {
        strcat(out, " ");
        strcat(out, token->literal);
    }

    out[1 + (token->lexeme ? strlen(token->lexeme) : 0) +
        (token->literal ? strlen(token->literal) : 0) + 2] = 0;
    return out;
}
