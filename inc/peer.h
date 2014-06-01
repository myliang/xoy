#ifndef _PEER_H_
#define _PEER_H_

typedef struct peer{

  char ip[20];
  unsigned int port;

  int am_choking = 1;
  int am_interested = 0;
  int peer_choking = 1;
  int peer_interested = 0;

  struct peer* next;
} peer;

peer* peer_init();
int peer_contain(peer* head, peer* cur);
void add_ip_port_topeer(peer* cur, const char* src);
void* peer_free(peer* p);

#endif /* end of include guard: _PEER_H_ */
