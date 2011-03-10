#include <libavformat/avio.h>
//#include "org/esb/util/Log.h"
static int test_open(URLContext *h, const char *uri, int flags){
  printf("test_open\n");
  return 0;
}

static int test_read(URLContext *h, uint8_t *buf, int size){
  printf("test_read\n");
  return 0;
}

static int test_write(URLContext *h, const unsigned char *buf, int size){
  printf("test_write\n");
  return 0;
}

static int64_t test_seek(URLContext *h, int64_t pos, int whence){
  printf("test_seek\n");
  return 0;
}

static int test_close(URLContext *h){
  printf("test_close\n");
  return 0;
}

URLProtocol test_protocol = {
  "test",
  test_open,
  test_read,
  test_write,
  test_seek,
  test_close,
};

