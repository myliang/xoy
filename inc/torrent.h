#ifndef _TORRENT_H_
#define _TORRENT_H_

#include "../inc/bencode.h"
#include "../inc/peer.h"

typedef long long torrent_size;

typedef struct torrent_tracker{
  char* url;
  struct torrent_tracker* next;
} torrent_tracker;

typedef struct torrent_file{
  torrent_size size;
  char* name;
  struct torrent_file* next;
} torrent_file;

typedef struct torrent{
  char* name;
  char* comment;
  char* created_by;
  char* encoding;
  char* pieces;
  torrent_size create_date;
  torrent_size piece_size;
  unsigned char info_hash[21];
  unsigned char peer_id[20];

  torrent_tracker* tracker;
  torrent_file* file;

  peer* peer;
} torrent;

torrent* torrent_init(b_encode* bp);

#endif /* end of include guard: _TORRENT_H_ */
