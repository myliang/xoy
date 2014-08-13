#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <event2/event.h>
#include <event2/http.h>
#include <event2/buffer.h>
#include <event2/http_struct.h>

#include "../inc/torrent.h"
#include "../inc/xbt_tcp.h"


static void http_get(char* url, torrent* tt,
    void(*func)(struct evhttp_request *req, void* arg));
static void http_get_callback(struct evhttp_request *req, void* arg);

static torrent* tt;

int main (int argc, const char* argv[]) {
  int i ;
  for (i = 0; i < argc; i++) {
    printf("%s\n", argv[i]);
  }
  b_encode* bp = b_encode_init(argv[1]);
  // b_encode_print(bp);

  tt = torrent_init(bp);

  torrent_tracker* tracker = tt->tracker;
  while(NULL != tracker) {
    char url[1024];
    snprintf(url, sizeof(url) - 1,
        "%s?info_hash=%s&peer_id=%s&port=%d&uploaded=%d&downloaded=%d&left=%d&event=%s&numwant=%d",
        tt->tracker->url, evhttp_encode_uri((char*)tt->info_hash), evhttp_encode_uri((char*)tt->peer_id),
        6881, 0, 0, 10240000, "started", 200);
    url[sizeof(url) - 1] = '\0';
    printf("url=%s\n", url);
    http_get(url, tt, http_get_callback);
    tracker = tracker->next;
  }

  // send message with peer
  xbt_peer* pp = tt->peer;
  while (pp) {
    sleep(5);
    fprintf(stderr, "send message start....");
    xbt_tcp_handshake(tt, pp);
    pp = pp->next;
  }
}

static void http_get_callback(struct evhttp_request* req, void* arg) {
  if (NULL == req) return ;
  size_t buffer_len = evbuffer_get_length(req->input_buffer);
  printf("buffer_len=%ld\n", buffer_len);
  if(buffer_len <= 0) return ;

  char buf[buffer_len + 1];
  evbuffer_remove(req->input_buffer, &buf, sizeof(buf) - 1);
  buf[buffer_len] = '\0';
  // printf("%ld, %s\n", sizeof(buf), buf);

  torrent* tt = (torrent*) arg;
  xbt_peer* p = tt->peer;
  xbt_peer head;
  if (NULL == tt->peer) {
    head.next = NULL;
    p = &head;
  }

  b_encode* bp = b_encode_init_with_string(buf, buffer_len);
  b_dict* dict = bp->data.dpv;
  while(NULL != dict) {
    if (strcmp(dict->key,"peers") == 0) {
      // peer_id have 6 bytes, ip have 4 bytes, port have 2 bytes
      char* peers = dict->value->data.cpv;
      int i;
      printf("%d\n", dict->value->len);
      for (i = 0; i < dict->value->len; i += 6) {
        xbt_peer* cur = xbt_peer_init();
        xbt_peer_add_ip_port(cur, &peers[i]);
        if (xbt_peer_contain(tt->peer, cur) == 0) {
          p = p->next = cur;
          if (NULL == tt->peer) {
            tt->peer = head.next;
          }
        } else xbt_peer_free(cur);
      }
      break;
    }
    dict = dict->next;
  }

}

static void http_get(char* url, torrent* tt, void(*func)(struct evhttp_request* req, void* arg)) {
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

  struct evhttp_request* req = evhttp_request_new(func, tt);
  evhttp_add_header(req->output_headers, "Host", host);
  // printf("scheme=%s, host=%s, port=%d, uri=%s, query=%s, path=%s\n", scheme, host, port, str_uri, query, path);
  evhttp_make_request(conn, req, EVHTTP_REQ_GET, str_uri);
  evhttp_connection_set_timeout(req->evcon, 600);
  event_base_dispatch(base);

  evhttp_connection_free(conn);
  event_base_free(base);
}
