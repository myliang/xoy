#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../inc/sha1.h"
#include "../inc/torrent.h"

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

static torrent_tracker* malloc_tracker(char* src, int len);
static char* malloc_string(char* src, int len);
static void _torrent_init(torrent* tt, b_encode* bp);
static torrent_file* malloc_file(char* str, int strlen, torrent_size file_size);

torrent* torrent_init(b_encode* bp) {
  torrent* tt = malloc(sizeof(torrent));
  _torrent_init(tt, bp);
  return tt;
}

static void _torrent_init (torrent* tt, b_encode* bp) {
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
        tp = tp->next = malloc_tracker(bll->data.cpv, bll->len);
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

    if (strncmp("name", bd->key, max(key_len, 4)) == 0) {
      tt->name = malloc_string(bd->value->data.cpv, bd->value->len);
    } else if (strncmp("pieces", bd->key, max(key_len, 6)) == 0) {
      tt->pieces = malloc_string(bd->value->data.cpv, bd->value->len);
    } else if (strncmp("piece length", bd->key, max(key_len, 12)) == 0) {
      tt->piece_size = bd->value->data.iv;
    } else if (strncmp("files", bd->key, max(key_len, 5)) == 0) {
      b_list* list = bd->value->data.lpv;
      torrent_file head;
      torrent_file* tf = &head;
      while (list) {
        b_dict* ldict = list->item->data.dpv;
        torrent_size size = ldict->value->data.iv;
        b_encode* dnext = ldict->next->value;
        tf = tf->next = malloc_file(dnext->data.cpv, dnext->len, size);
        list = list->next;
      }
    } else if (strncmp("info", bd->key, max(key_len, 4)) == 0) {
      _torrent_init(tt, bd->value);
      SHA1_CTX context;
      SHA1Init(&context);
      SHA1Update(&context, bd->value->begin, bd->value->len);
      SHA1Final(tt->info_hash, &context);
    }
    bd = bd->next;
  }
}

static torrent_file* malloc_file(char* str, int strlen, torrent_size file_size) {
  torrent_file* tf = malloc(sizeof(torrent_file));
  tf->size = file_size;
  tf->name = malloc_string(str, strlen);
  return tf;
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
