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
  printf("type=%d\n", bp->type);
  while(NULL != bd) {
    printf("key=%s\n", bd->key);
    unsigned int key_len = strlen(bd->key);
    int max_len = max(key_len, 13);
    if(strncmp("announce-list", bd->key, max_len) == 0) {
      b_list* bl = bd->value->data.lpv;
      while(NULL != bl) {
        b_list* bll = bl->item->data.lpv;
        printf("%s\n", bll->item->data.cpv);
        bl = bl->next;
      }
    }
    bd = bd->next;
  }
  return tt;
}
