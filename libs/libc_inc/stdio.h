#ifndef STDIO_H_
#define STDIO_H_

#include <stdarg.h>

extern int printf(const char *, ...),
           sprintf(char *, const char *, ...);

extern int vsprintf(char *buf, const char *fmt, va_list ap);

#define EOF (-1)

int putchar(int c);
int getche(void);
int puts (const char *s);

typedef void (*putc_func)(char, char**);

extern int _doprntx( const char *fmt0,
                     va_list     argp,
                     putc_func   putc,
                     char      **pca);

#endif /* STDIO_H_ */
