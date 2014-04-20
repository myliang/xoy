#ifndef _BENCODE_H_
#define _BENCODE_H_

typedef enum {
  B_INTEGER = 1,
  B_STRING,
  B_LIST,
  B_DICT
} b_type;

typedef long long b_size;
struct b_encode;

typedef struct b_list{
  struct b_encode* item;
  struct b_list* next;
} b_list;

typedef struct b_dict{
  char* key;
  struct b_encode* value;
  struct b_dict* next;
} b_dict;

typedef struct {
  b_type type;
  char* begin;
  int len;
  union {
    b_size iv;
    char* cpv;
    b_list* lpv;
    b_dict* dpv;
  } data;
} b_encode;

b_encode* b_encode_init(char* file_name);
void b_encode_free(b_encode* encode);

#endif /* end of include guard: _BENCODE_H_ */
