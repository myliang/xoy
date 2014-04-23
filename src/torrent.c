#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../inc/torrent.h"

#define min(a, b) ((a) < (b) ? (a) : (b))

torrent* torrent_init (b_encode* bp) {
  if(NULL == bp) return NULL;
  torrent* tt = malloc(sizeof(torrent));
  b_dict* bd = bp->data.dpv;
  printf("type=%d\n", bp->type);
  while(NULL != bd) {
    int key_len = strlen(bd->key);
    int min_len = min(key_len, 13);
    printf("key=%s", bd->key);
    if(strncmp("announce-list", bd->key, min_len)) {
      b_list* bl = bd->value->data.lpv;
      while(NULL != bl) {
        printf("%s\n", bl->item->data.cpv);
        bl = bl->next;
      }
    }
    bd = bd->next;
  }
  return tt;
}
