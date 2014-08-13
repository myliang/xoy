#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#include "../inc/rw.h"
#include "../inc/xoy_http.h"

static int tcp_connect(char *ip, short port);
static struct xoy_http_request *tohreq(char *url);
static char *hreqtoa(struct xoy_http_request *req);

struct xoy_http_response *http_get(char *url) {
  struct xoy_http_request *req = tohreq(url);
  int sockfd = tcp_connect(req->ip, req->port);
  if (sockfd < 0) {
    return NULL;
  }

  char *buff = hreqtoa(req);
  if (writen(sockfd, buff, sizeof(buff)) < 0) {
    fprintf(stderr, "write error\n");
    return NULL;
  }

  char recvline[2048];
  readline(sockfd, recvline, 2048);
}

static int tcp_connect(char *ip, short port) {
  int sockfd;
  struct sockaddr_in servaddr;

  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = inet_addr(ip);
  servaddr.sin_port = htons(port);

  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    fprintf(stderr, "socket error\n");
    return -1;
  }

  if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
    fprintf(stderr, "connect ip<%s> error\n", ip);
    return -1;
  }

  return sockfd;
}
