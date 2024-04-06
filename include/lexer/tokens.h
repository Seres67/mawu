#ifndef MAWU_TOKENS_H
#define MAWU_TOKENS_H

enum mawu_token_type
{
    MAWU_LEFT_PAREN,
    MAWU_RIGHT_PAREN,
    MAWU_EQUAL,
    MAWU_EQUAL_EQUAL,
    MAWU_EOF
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
