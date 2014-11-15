#ifndef STDLIB_H_
#define STDLIB_H_

int abs(int i);
int atoi (const char *s);
long atol(const char *s);
long strtol (const char *nptr, char **ptr, int base);
long long strtoll(const char *nptr, char **endptr, int base);
unsigned long strtoul(const char *nptr, char **endptr, int base);
unsigned long long strtoull(const char *nptr, char **endptr, int base);

typedef struct {
    int quot; /* quotient */
    int rem; /* remainder */
}div_t;

typedef struct {
    long quot; /* quotient */
    long rem; /* remainder */
}ldiv_t;

div_t div (int num, int denom);
ldiv_t ldiv (long num, long denom);

void srand(unsigned int seed);
int rand(void);

#endif /* STDLIB_H_ */
