#ifndef _RW_H_
#define _RW_H_

ssize_t readn(int fd, void *vptr, size_t n);
ssize_t writen(int fd, const void *vptr, size_t n);
ssize_t readline(int fd, void *vptr, size_t maxlen);

#endif /* end of include guard: _RW_H_ */
