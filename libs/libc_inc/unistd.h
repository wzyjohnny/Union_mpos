#ifndef UNISTD_H_
#define UNISTD_H_

#include <sys/types.h>
#include <sys/time.h>

# define	SEEK_SET	0
# define	SEEK_CUR	1
# define	SEEK_END	2

int close(int fd);
ssize_t read(int fd, void * buffer, size_t len);
ssize_t write(int fd, const void * buffer, size_t len);
int select(int nfds, fd_set *readfds, fd_set *writefds,
           fd_set *exceptfds, struct timeval *timeout);
int unlink(const char * pathname);
off_t lseek(int fd, off_t offset, int whence);

#endif
