#ifndef _PEER_H_
#define _PEER_H_

typedef struct xbt_peer{

  char ip[20];
  unsigned int port;

  // int16_t am_choking = 1;
  // int16_t am_interested = 0;
  // int16_t peer_choking = 1;
  // int16_t peer_interested = 0;

  struct xbt_peer* next;

} xbt_peer;

xbt_peer* xbt_peer_init();
int16_t xbt_peer_contain(xbt_peer* head, xbt_peer* cur);
void xbt_peer_add_ip_port(xbt_peer* cur, const char* src);
void* xbt_peer_free(xbt_peer* p);

#endif /* end of include guard: _PEER_H_ */
