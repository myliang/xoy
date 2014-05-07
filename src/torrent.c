#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../inc/torrent.h"

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

static torrent_tracker* malloc_tracker(char* src, int len);
static char* malloc_string(char* src, int len);

torrent* torrent_init (b_encode* bp) {
  if(NULL == bp) return NULL;
  torrent* tt = malloc(sizeof(torrent));
  b_dict* bd = bp->data.dpv;
  // printf("type=%d\n", bp->type);
  while(NULL != bd) {
    printf("key=%s\n", bd->key);
    unsigned int key_len = strlen(bd->key);
    if(strncmp("announce-list", bd->key, max(key_len, 13)) == 0) {
      b_list* bl = bd->value->data.lpv;
      torrent_tracker head;
      torrent_tracker* tp = &head;
      while(NULL != bl) {
        b_encode* bll = bl->item->data.lpv->item;
        tp->next = malloc_tracker(bll->data.cpv, bll->len);
        tp = tp->next;
        // printf("%s\n", tp->url);
        bl = bl->next;
      }
      tt->tracker = head.next;
    }
    if(strncmp("announce", bd->key, max(key_len, 8)) == 0) {
      if (NULL != tt->tracker) {
        b_encode* bp = bd->value;
        tt->tracker = malloc_tracker(bp->data.cpv, bp->len);
      }
    }

    if(strncmp("comment", bd->key, max(key_len, 7)) == 0) {
      tt->comment = malloc_string(bd->value->data.cpv, bd->value->len);
    }
    if(strncmp("encoding", bd->key, max(key_len, 8)) == 0) {
      tt->encoding = malloc_string(bd->value->data.cpv, bd->value->len);
    }
    if(strncmp("created by", bd->key, max(key_len, 10)) == 0) {
      tt->created_by = malloc_string(bd->value->data.cpv, bd->value->len);
    }

    if(strncmp("creation date", bd->key, max(key_len, 13)) == 0) {
      tt->create_date = bd->value->data.iv;
    }
    if(strncmp("piece length", bd->key, max(key_len, 12)) == 0) {
      tt->piece_size = bd->value->data.iv;
    }

    if (strncmp("info", bd->key, max(key_len, 4)) == 0) {
      b_dict* file = bd->value->data.dpv;
      while (NULL != file) {
        // file
      }

    }
    bd = bd->next;
  }
  return tt;
}

static torrent_tracker* malloc_tracker(char* src, int len) {
  torrent_tracker* ttk = malloc(sizeof(torrent_tracker));
  ttk->next = NULL;
  ttk->url = malloc_string(src, len);
  return ttk;
}
static char* malloc_string (char* src, int len) {
  char* dst = malloc(len + 1);
  memcpy(dst, src, len);
  dst[len] = '\0';
  return dst;
}
