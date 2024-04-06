#include <stdio.h>

void report(int line, char *where, char *message)
{
    fprintf(stderr, "[error] %s: %s at line %d\n", where, message, line);
}

void error(int line, char *message) { report(line, "", message); }
