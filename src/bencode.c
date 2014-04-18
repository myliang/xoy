#include <stdlib.h>
#include <stdio.h>
#include "../inc/bencode.h"

typedef struct {
  char* head;
  char* tail;
  int len;
  char* index;
} b_buffer;

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
  buf->tail = &(buf->head[i - 1]);
  buf->head[i] = '\0';

#ifdef DEBUG
  printf("the head char is %c , the index char is %c, the tail char is %c\n", *buf->head, *buf->index, *buf->tail);
  printf("the size of the %s file is %ld\n", file_name, buf->len);
  printf("the content of the %s file is %s\n", file_name, buf->head);
#endif

  return buf;
}

static void b_buffer_free (b_buffer* buf) {
  if (NULL != buf) {
    free(buf);
  }
}
