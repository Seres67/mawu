#ifndef MAWU_TOKENS_H
#define MAWU_TOKENS_H

enum mawu_token_type
{
    LEFT_PAREN,
    RIGHT_PAREN
};

typedef struct mawu_token_s
{
    enum mawu_token_type type;
    char *lexeme;
    char *literal;
    int line;
} mawu_token;

mawu_token *token_new(enum mawu_token_type type, char *text, char *literal,
                      int line);
char *mawu_token_to_string(mawu_token *token);

#endif // !MAWU_TOKENS_H
