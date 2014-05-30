#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>

#include "../inc/peer.h"


peer* peer_init () {
  peer* p = malloc(sizeof(peer));
  p->next = NULL;
  return p;
}

int peer_contain (peer* head, peer* cur) {
  while (NULL != head) {
    printf(";;;;head.ip=%s, cur.ip=%s\n", head->ip, cur->ip);
    printf("strcmp.ret=%d\n", strcmp(head->ip, cur->ip));
    if (strcmp(head->ip, cur->ip) == 0 && head->port == cur->port) {
      return 1;
    }
    head = head->next;
  }
  return 0;
}

void add_ip_port_topeer (peer* cur, const char* src) {
  int64_t ip = (src[0] << 24) + (src[1] << 16) + (src[2] << 8) + src[3];
  struct in_addr s;
  s.s_addr = ip;
  inet_ntop(AF_INET, (void*)&s, cur->ip, 16);
  cur->port = (src[4] << 8 & 0xffff) + src[5];
  printf("peer->ip = %s, peer->port = %d\n", cur->ip, cur->port);
}

void* peer_free(peer* p) {
  free(p);
}
