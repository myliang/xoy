#include "../inc/xbt_tcp.h"

#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include <event2/event.h>
#include <event2/buffer.h>
#include <event2/bufferevent.h>

static void ev_tcp_request(char* host, unsigned int port, void(*cb_read)(struct bufferevent* be, void* arg),
    void(*cb_write)(struct bufferevent* be, void* arg),
    void(*cb_event)(struct bufferevent* be, short events, void* arg),
    void* arg);

static void xbt_tcp_handshake_read(struct bufferevent* be, void* arg);
static void xbt_tcp_handshake_write(struct bufferevent* be, void* arg);


void xbt_tcp_handshake(torrent* tp, xbt_peer* pp) {
  ev_tcp_request(pp->ip, pp->port, xbt_tcp_handshake_read, xbt_tcp_handshake_write, NULL, tp);
}
static void xbt_tcp_handshake_read(struct bufferevent* be, void* arg) {

}
static void xbt_tcp_handshake_write(struct bufferevent* be, void* arg) {
  torrent* tp = arg;
  char handshake[68];
  memset(handshake, 0, 68);
  char pstr[] = "BitTorrent protocol";
  handshake[0] = sizeof(pstr);
  memcpy(handshake + 1, pstr, handshake[0]);
  memcpy(handshake + 28, tp->info_hash, 20);
  memcpy(handshake + 48, tp->peer_id, 20);

  struct evbuffer* output = bufferevent_get_output(be);
  evbuffer_add(output, handshake, sizeof(handshake));
}

static void ev_tcp_request(char* host, unsigned int port, void(*cb_read)(struct bufferevent* be, void* arg),
    void(*cb_write)(struct bufferevent* be, void* arg),
    void(*cb_event)(struct bufferevent* be, short events, void* arg),
    void* arg) {
  struct event_base* ev_base_tcp = event_base_new();
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = inet_addr(host);
  addr.sin_port = htons(port);
  struct bufferevent* be = bufferevent_socket_new(ev_base_tcp, -1, BEV_OPT_CLOSE_ON_FREE);
  bufferevent_setcb(be, cb_read, cb_write, cb_event, arg);
  bufferevent_socket_connect(be, (struct sockaddr*)&addr, sizeof(struct sockaddr_in));
  event_base_dispatch(ev_base_tcp);
}
