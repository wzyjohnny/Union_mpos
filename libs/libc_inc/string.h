#ifndef STRING_H_
#define STRING_H_

#include <sys/types.h>

extern char
        *strcpy(char *, const char *),
        *strncpy(char *, const char *, size_t),
        *strcat(char *, const char *),
        *strncat(char *, const char *, size_t),
        *strchr(const char *, int),
        *strstr(const char *, const char *),
        *strrchr(const char *, int),
        *strpbrk(char *, char *),
        *strtok(char *, char *),
        *index (const char *s, int c);

extern int
        strcmp(const char *, const char *),
        strncmp(const char *, const char *, size_t),
        strcasecmp(const char *, const char *),
        strncasecmp(const char *, const char *, size_t);

extern size_t
        strspn(const char *, const char *),
        strcspn(const char *, const char *),
        strlen(const char *),
        strnlen(const char *str, size_t n);

extern void
        *memccpy(void *, const void *, int, size_t),
        *memchr(const void *, int, size_t),
        *memcpy(void *, const void *, size_t),
        *memset(void *, int, size_t),
        *memmove(void *, const void *, size_t);
extern int
        memcmp(const void *, const void *, size_t);

#endif /* STRING_H_ */
