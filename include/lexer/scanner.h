#ifndef MAWU_SCANNER_H
#define MAWU_SCANNER_H

#include <lexer/tokens.h>

typedef struct mawu_scanner_s
{
    char *source;
    mawu_token **tokens;
    int size;
    int _start;
    int _current;
    int _line;
    int _token_count;
} mawu_scanner;

mawu_scanner *scanner_new(char *source);
void scanner_delete(mawu_scanner *scanner);
mawu_token **scanner_scan_tokens(mawu_scanner *scanner);
_Bool scanner_is_at_end(mawu_scanner *scanner);

#endif // !MAWU_SCANNER_H
