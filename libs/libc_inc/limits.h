#ifndef LIMITS_H_
#define LIMITS_H_

#define CHAR_BIT 8
#define SCHAR_MIN (-128)
#define SCHAR_MAX 127
#define UCHAR_MAX 255
#define CHAR_MIN (-128)
#define CHAR_MAX 127

#define SHRT_MIN (-32767-1)
#define SHRT_MAX 32767
#define USHRT_MAX 65535

#define __INT_MAX__ 2147483647
#define INT_MIN (-INT_MAX-1)
#define INT_MAX __INT_MAX__
#define UINT_MAX (INT_MAX * 2U + 1)

#define __LONG_MAX__ 2147483647L
#define LONG_MIN (-LONG_MAX-1)
#define LONG_MAX __LONG_MAX__
#define ULONG_MAX (LONG_MAX * 2UL + 1)

#define __LONG_LONG_MAX__ 9223372036854775807LL
#define LLONG_MIN (-LLONG_MAX-1)
#define LLONG_MAX __LONG_LONG_MAX__
#define ULLONG_MAX (LLONG_MAX * 2ULL + 1)
#define LONG_LONG_MIN (-LONG_LONG_MAX-1)
#define LONG_LONG_MAX __LONG_LONG_MAX__
#define ULONG_LONG_MAX (LONG_LONG_MAX * 2ULL + 1)

#endif /* LIMITS_H_ */
