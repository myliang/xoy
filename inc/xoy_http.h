#ifndef _XOY_HTTP_H_
#define _XOY_HTTP_H_

struct xoy_http_request {
  char *ip;
  short port;
} /* optional variable list */;

struct xoy_http_response {
  int status;
  char *content;
} /* optional variable list */;

struct xoy_http_response *http_get(char *url);

#endif /* end of include guard: _XOY_HTTP_H_ */
