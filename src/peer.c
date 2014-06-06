#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>

#include "../inc/peer.h"


xbt_peer* xbt_peer_init () {
  xbt_peer* p = malloc(sizeof(xbt_peer));
  p->next = NULL;
  return p;
}

int16_t xbt_peer_contain (xbt_peer* head, xbt_peer* cur) {
  while (NULL != head) {
    if (strcmp(head->ip, cur->ip) == 0 && head->port == cur->port) {
      return 1;
    }
    head = head->next;
  }
  return 0;
}

void xbt_peer_add_ip_port (xbt_peer* cur, const char* src) {
  int64_t ip = (src[0] << 24) + (src[1] << 16) + (src[2] << 8) + src[3];
  struct in_addr s;
  s.s_addr = ip;
  inet_ntop(AF_INET, (void*)&s, cur->ip, 16);
  cur->port = (src[4] << 8 & 0xffff) + src[5];
  printf("peer->ip = %s, peer->port = %d\n", cur->ip, cur->port);
}

void* xbt_peer_free(xbt_peer* p) {
  free(p);
}
