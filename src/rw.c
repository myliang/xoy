#include "../inc/rw.h"

#include <unistd.h>
#include <errno.h>

ssize_t readn(int fd, void *vptr, size_t n) {
  size_t nleft;
  ssize_t nread;
  char *ptr;

  ptr = vptr;
  nleft = n;

  while(nleft > 0) {
    if((nread = read(fd, ptr, nleft)) < 0) {
      if(errno == EINTR)
        nread = 0; /* and call read() again */
      else
        return -1;
    }else if(nread == 0)
      break; /* EOF */

    nleft -= nread;
    ptr += nread;
  }

  return n - nleft; /* return >= 0 */
}

ssize_t writen(int fd, const void *vptr, size_t n) {
  size_t nleft;
  ssize_t nwritten;
  const char *ptr;

  ptr = vptr;
  nleft = n;
  while(nleft > 0) {
    if((nwritten = write(fd, ptr, nleft)) < 0) {
      if(nwritten < 0 && errno == EINTR)
        nwritten = 0; /* and call write() again */
      else
        return -1;
    }

    nleft -= nwritten;
    ptr += nwritten;
  }

  return n;
}

ssize_t readline(int fd, void *vptr, size_t maxlen) {
  ssize_t n, rc;
  char c, *ptr;

  ptr = vptr;
  for(n = 1; n < maxlen; n++) {
    again:
      if((rc = read(fd, &c, 1)) == 1) {
        *ptr++ = c;
        if(c == '\n')
          break;  /* newline is stored, like fgets() */
      } else if (rc == 0) {
        *ptr = 0;
        return n - 1; /* EOF, n -1 bytes were read */
      } else {
        if (errno == EINTR)
          goto again;

        return -1;  /* error, errno set by read() */
      }
  }

  *ptr = 0;
  return n;
}


