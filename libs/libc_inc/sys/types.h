#ifndef SYS_TYPES_H_
#define SYS_TYPES_H_

#ifndef NULL
#define NULL 0
#endif

typedef long            off_t;
typedef unsigned long   size_t;
typedef long            ssize_t;
typedef long            ptrdiff_t;
typedef unsigned long   mode_t;
typedef long            time_t;
typedef unsigned long   clock_t;
typedef unsigned long   useconds_t;
typedef long            suseconds_t;

struct timespec {
    time_t tv_sec;
    long   tv_nsec;
};

#define NFDSHIFT        5       /* 32 bit per mask */
#define NFDBITS         (1 << NFDSHIFT)
#define NFDMASK         ((1 << NFDSHIFT) - 1)
#ifndef FD_SETSIZE
#define FD_SETSIZE      256     /* default 256 files */
#endif

typedef unsigned long fd_mask;
typedef struct {
    fd_mask fds_bits [FD_SETSIZE >> NFDSHIFT];
}fd_set;

#define FD_SET(n, p)    ((p)->fds_bits[(n) >> NFDSHIFT] |= (1L << ((n) & NFDMASK)))
#define FD_CLR(n, p)    ((p)->fds_bits[(n) >> NFDSHIFT] &= ~(1L << ((n) & NFDMASK)))
#define FD_ISSET(n, p)  ((p)->fds_bits[(n) >> NFDSHIFT] & (1L << ((n) & NFDMASK)))
#define FD_ZERO(p)      do { \
                            unsigned int _i;   \
                            for( _i = 0; _i < (FD_SETSIZE >> NFDSHIFT); _i++ ) \
                                (p)->fds_bits[_i] = 0; \
                        }while (0)

#endif
