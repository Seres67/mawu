#ifndef MAWU_TOKENS_H
#define MAWU_TOKENS_H

enum mawu_token_type
{
    MAWU_LEFT_PAREN,
    MAWU_RIGHT_PAREN,
    MAWU_LEFT_BRACE,
    MAWU_RIGHT_BRACE,
    MAWU_COMMA,
    MAWU_DOT,
    MAWU_PLUS,
    MAWU_MINUS,
    MAWU_STAR,
    MAWU_SEMICOLON,
    MAWU_SLASH,
    MAWU_BANG_EQUAL,
    MAWU_BANG,
    MAWU_EQUAL,
    MAWU_EQUAL_EQUAL,
    MAWU_LESS,
    MAWU_LESS_EQUAL,
    MAWU_GREATER_EQUAL,
    MAWU_GREATER,
    MAWU_STRING,
    MAWU_NUMBER,
    MAWU_IDENTIFIER,
    MAWU_ELSE,
    MAWU_FALSE,
    MAWU_FOR,
    MAWU_FUN,
    MAWU_IF,
    MAWU_NULL,
    MAWU_RETURN,
    MAWU_TRUE,
    MAWU_VAR,
    MAWU_WHILE,
    MAWU_EOF,
    MAWU_NONE
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
