#include <stdint.h>
#include <stddef.h>

extern "C" int LLVMFuzzerTestOneInput(uint8_t * buf, size_t len) {
  if (len > 0 && buf[0] == 'H')
    if (len > 1 && buf[1] == 'I')
       if (len > 2 && buf[2] == '!')
       __builtin_trap();
  return 0;
}
