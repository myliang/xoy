#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../inc/torrent.h"

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

torrent* torrent_init (b_encode* bp) {
  if(NULL == bp) return NULL;
  torrent* tt = malloc(sizeof(torrent));
  b_dict* bd = bp->data.dpv;
  // printf("type=%d\n", bp->type);
  while(NULL != bd) {
    printf("key=%s\n", bd->key);
    unsigned int key_len = strlen(bd->key);
    int max_len = max(key_len, 13);
    if(strncmp("announce-list", bd->key, max_len) == 0) {
      b_list* bl = bd->value->data.lpv;
      torrent_tracker head;
      torrent_tracker* tp = &head;
      while(NULL != bl) {
        b_encode* bll = bl->item->data.lpv->item;
        tp->next = malloc(sizeof(torrent_tracker));
        tp = tp->next;
        tp->url = malloc(sizeof(bll->len));
        memcpy(tp->url, bll->data.cpv, bll->len);
        tp->next = NULL;
        printf("%s\n", bll->data.cpv);
        bl = bl->next;
      }
      tt->tracker = head.next;
    }
    bd = bd->next;
  }
  return tt;
}
