#include "rhio-parallel.h"

bool Parallel::add(unsigned char (*callback)()) {
  for (auto i = 0; i < PARALLEL_MAX; i++) {
    if (callbacks[i] == 0) {
      callbacks[i] = callback;
      added++;
      return true;
    }
  }
  return false;
}

void Parallel::run(void (*done)(unsigned char* errors)) {
  resetCounter();
  while (next())
    ;
  done(errs);
}

bool Parallel::yield() {
  if (current >= added) return true;

  current++;
  while (next())
    ;

  return true;
}

bool Parallel::next() {
  if (current >= added) {
    return false;
  }

  errs[current] = callbacks[current]();
  current++;

  return true;
}

void Parallel::resetCounter() {
  current = 0;
  for (auto i = 0; i < PARALLEL_MAX; i++) {
    errs[i] = 0;
  }
}
