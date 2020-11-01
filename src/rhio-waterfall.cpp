#include "rhio-waterfall.h"

unsigned char waterfall(unsigned char (*callbacks[])(), int size) {
  for (int i = 0; i < size; i++) {
    if ((callbacks[i]) != 0) {
      unsigned char err = callbacks[i]();
      if (err > 0) return err;
    }
  }

  return 0;
}

unsigned char waterfall(unsigned char (*callbacks[])(int p), int size, int p) {
  for (int i = 0; i < size; i++) {
    if ((callbacks[i]) != 0) {
      unsigned char err = callbacks[i](p);
      if (err > 0) return err;
    }
  }

  return 0;
}
