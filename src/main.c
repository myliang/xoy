#include <stdio.h>
#include <stdio.h>
#include <string.h>

#include <event2/event.h>
#include <event2/http.h>
#include <event2/buffer.h>
#include <event2/http_struct.h>

#include "../inc/torrent.h"

static void http_get(char* url, void(*func)(struct evhttp_request *req, void* arg));
static void http_get_callback(struct evhttp_request *req, void* arg);

int main (int argc, const char* argv[]) {
  int i ;
  for (i = 0; i < argc; i++) {
    printf("%s\n", argv[i]);
  }
  b_encode* bp = b_encode_init(argv[1]);
  b_encode_print(bp);

  torrent* tt = torrent_init(bp);

  http_get("http://www.baidu.com", http_get_callback);
  return 0;
}

static void http_get_callback(struct evhttp_request* req, void* arg) {
  size_t buffer_len = evbuffer_get_length(req->input_buffer);
  printf("buffer_len=%ld\n", buffer_len);
  char buf[buffer_len + 1];
  evbuffer_remove(req->input_buffer, &buf, sizeof(buf) - 1);
  buf[buffer_len] = '\0';
  printf("%s\n", buf);
}

static void http_get(char* url, void(*func)(struct evhttp_request* req, void* arg)) {
  struct evhttp_uri* uri = evhttp_uri_parse(url);
  const char* scheme = evhttp_uri_get_scheme(uri);

  const char* host = evhttp_uri_get_host(uri);
  int port = evhttp_uri_get_port(uri);
  if (port == -1) {
    port = (strcasecmp(scheme, "http") == 0) ? 80 : 443;
  }

  char str_uri[256];
  const char* path = evhttp_uri_get_path(uri);
  if(NULL == path || strcmp("", path) == 0) path = "/";

  const char* query = evhttp_uri_get_query(uri);
  if (NULL == query) {
    snprintf(str_uri, sizeof(str_uri) - 1, "%s", path);
  } else {
    snprintf(str_uri, sizeof(str_uri) - 1, "%s?%s", path, query);
  }
  str_uri[sizeof(str_uri) - 1] = '\0';

  struct event_base* base = event_base_new();
  struct evhttp_connection* conn = evhttp_connection_base_new(base,
      NULL, host, port);

  struct evhttp_request* req = evhttp_request_new(func, base);
  evhttp_add_header(req->output_headers, "Host", host);
  printf("scheme=%s, host=%s, port=%d, uri=%s, query=%s, path=%s\n", scheme, host, port, str_uri, query, path);
  evhttp_make_request(conn, req, EVHTTP_REQ_GET, str_uri);
  evhttp_connection_set_timeout(req->evcon, 600);
  event_base_dispatch(base);

  evhttp_connection_free(conn);
  event_base_free(base);
}
