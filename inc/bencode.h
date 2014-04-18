#ifndef _BENCODE_H_
#define _BENCODE_H_

typedef enum {
  B_INTEGER = 1,
  B_STRING,
  B_LIST,
  B_DICT
} b_type;

struct b_encode;

typedef struct b_list{
  struct b_encode* item;
  struct b_list* next;
} b_list;

typedef struct b_dict{
  char *key;
  struct b_encode* value;
  struct b_dict* next;
} b_dict;

typedef struct {
  b_type type;
  union {
    unsigned long long iv;
    char* cpv;
    b_list* lpv;
    b_dict* dpv;
  } data;
} b_encode;

b_encode* b_encode_init(char *content);

#endif /* end of include guard: _BENCODE_H_ */
