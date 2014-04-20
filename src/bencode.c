#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "../inc/bencode.h"

typedef struct {
  char* head;
  char* tail;
  int len;
  char* index;
} b_buffer;


// b_buffer methods
static b_buffer* b_buffer_init(const char* file_name);
static b_size b_buffer_read_int(b_buffer* buf);
static void b_buffer_free(b_buffer* buf);

// b_encode methods
static b_encode* parse_int(b_buffer* buf);
static b_encode* parse_string(b_buffer* buf);
static b_encode* parse_list(b_buffer* buf);
static b_encode* parse_dict(b_buffer* buf);
static b_encode* parse(b_buffer* buf);
static b_encode* b_encode_malloc();

b_encode* b_encode_init (char* file_name) {
  b_buffer* buf = b_buffer_init(file_name);
  if(NULL == buf) {
    return NULL;
  }
  b_buffer_free(buf);
}

void b_encode_free (b_encode* encode) {
  // if (NULL != encode) {
  //   free(encode);
  // }
}

/****** the methods of b_encode int string list dict ******************/
static b_encode* parse_int(b_buffer* buf) {
  char* begin = buf->index++;
  b_size value = b_buffer_read_int(buf);
  b_encode* be = b_encode_malloc(B_INTEGER, begin, buf->index++);
  be->data.iv = value;
  return be;
}
static b_encode* parse_string(b_buffer* buf) {
  b_size len = b_buffer_read_int(buf);
  b_encode* be = b_encode_malloc(B_STRING, buf->index++, len);
  be->data.cpv = malloc(len + 1);
  memcpy(be->data.cpv, buf->index, len);
  be->data.cpv[len] = '\0';
  return be;
}
static b_encode* parse_list(b_buffer* buf) {
  char* begin = buf->index++;
  b_list head;
  b_list* bl = &head;
  while ('e' != *buf->index) {
    bl->next = malloc(sizeof(b_list));
    bl = bl->next;
    bl->item = parse(buf);
    bl->next = NULL;
  }
  buf->index++;
  b_encode* be = b_encode_malloc(B_LIST, begin, buf->index - begin);
  be->data.lpv = head.next;
  return be;
}
static b_encode* parse_dict(b_buffer* buf) {
  char* begin = buf->index++;
  b_dict head;
  b_dict* bd = &head;
  while ('e' != *buf->index) {
    bd->next = malloc(sizeof(b_dict));
    bd = bd->next;
    b_encode* bs = parse_string(buf);
    bd->key = bs->data.cpv;
    free(bs);
    bd->value = parse(buf);
    bd->next = NULL;
  }
  buf->index++;
  b_encode* be = b_encode_malloc(B_LIST, begin, buf->index - begin);
  be->data.dpv = head.next;
  return be;
}
static b_encode* parse(b_buffer* buf) {
  switch (*buf->index) {
    case 'd': return parse_dict(buf);
    case 'l': return parse_list(buf);
    case 'i': return parse_int(buf);
    default : return parse_string(buf);
  }
}
static b_encode* b_encode_malloc(b_type type, char* begin, char* end) {
  b_encode* be = malloc(sizeof(b_encode));
  be->type = type;
  be->begin = begin;
  be->len = end - begin;
  return be;
}

/****** the methods of b_buffer_init and b_buffer_free *****************/
static b_buffer* b_buffer_init (const char* file_name) {
  b_buffer* buf = malloc(sizeof(b_buffer));

  FILE* fp = fopen(file_name, "rb");
  if (NULL == fp) {
    printf("%s:%d can not open file[%s]\n", __FILE__, __LINE__, file_name);
    return NULL;
  }

  fseek(fp, 0, SEEK_END);
  long file_size = ftell(fp);
  if (file_size < 0) {
    printf("%s:%d fseek failed\n", __FILE__, __LINE__);
    return NULL;
  }

  buf->head = buf->index = malloc(file_size + 1);
  buf->len = file_size;

  long i;
  fseek(fp, 0, SEEK_SET);
  for (i = 0; i < file_size; i++) {
    *(buf->head + i) = fgetc(fp);
  }
  fclose(fp);

  buf->tail = &(buf->head[i - 1]);
  buf->head[i] = '\0';

#ifdef DEBUG
  printf("the head char is %c , the index char is %c, the tail char is %c\n", *buf->head, *buf->index, *buf->tail);
  printf("the size of the %s file is %ld\n", file_name, buf->len);
  printf("the content of the %s file is %s\n", file_name, buf->head);
#endif

  return buf;
}
static b_size b_buffer_read_int(b_buffer* buf) {
  b_size value = 0;
  for(; isdigit(*buf->index); buf->index++) {
    value = value * 10 + (*buf->index - '0');
  }
  return value;
}
static void b_buffer_free (b_buffer* buf) {
  if (NULL != buf) {
    if (NULL != buf->head) {
      free(buf->head);
    }
    free(buf);
  }
}
