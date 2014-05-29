#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>

#include "../inc/peer.h"

peer* peer_init () {
  peer* peer = malloc(sizeof(peer));
  peer->next = NULL;
  return peer;
}

void add_ip_port_topeer (peer* ppt, const char* src) {
  int64_t ip = (src[0] << 24) + (src[1] << 16) + (src[2] << 8) + src[3];
  struct in_addr s;
  s.s_addr = ip;
  inet_ntop(AF_INET, (void*)&s, ppt->ip, 16);
  ppt->port = (src[4] << 8) + src[5];
  printf("peer->ip = %s, peer->port = %d\n", ppt->ip, ppt->port);
}
