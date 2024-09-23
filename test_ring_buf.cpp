#include <stdio.h>
#include "ring_buf.hpp"
#include "assert.h"

int main(void) {
  auto buf = RingBuf<int>(3);
  for(int i = 0; i < 5; ++i) {
    int* e1 = buf.push_elem();
    *e1 = 13;
    int* re1 = buf.get_next_elem();
    assert(*re1 == 13);
    buf.pop_elem();
  }
  return 0;
}