#ifndef _PEER_H_
#define _PEER_H_

typedef struct peer{

  char ip[20];
  unsigned int port;

  struct peer* next;
} peer;

peer* peer_init();
void add_ip_port_topeer(peer* ppt, const char* src);

#endif /* end of include guard: _PEER_H_ */
