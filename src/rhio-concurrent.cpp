#include "rhio-concurrent.h"

#include <Arduino.h>

RhioConcurrent::RhioConcurrent() {
  current = 0;
  for (auto i = 0; i < RHIO_CONCURRENT_MAX; i++) {
    yielding[i] = 0;
    timers[i] = millis();
  }
}

bool RhioConcurrent::add(void (*callback)()) {
  for (auto i = 0; i < RHIO_CONCURRENT_MAX; i++) {
    if (callbacks[i] == 0) {
      callbacks[i] = callback;
      added++;
      return true;
    }
  }
  return false;
}

void RhioConcurrent::run() { next(); }

void RhioConcurrent::next() {
  if (yielding[current]) {
    current++;
    return;
  }

  if (current >= added) current = 0;

  (*callbacks[current])();
  current++;
}

bool RhioConcurrent::yield() {
  int _current = current;
  addYield(_current);

  for (auto i = _current + 1; i < added; i++) {
    if (yielding[i]) continue;
    current = i;
    (*callbacks[i])();
  }

  int i = _current;
  while (i--) {
    if (yielding[i]) continue;
    current = i;
    (*callbacks[i])();
  }

  current = _current;
  removeYield(_current);
  return true;
}

void RhioConcurrent::delay(unsigned long waitMillis, void (*callback)()) {
  const int _current = current;

  if ((millis() - timers[_current]) >= waitMillis) {
    timers[_current] = millis();
    (*callback)();
  }
}

inline void RhioConcurrent::addYield(int index) { yielding[index] = 1; }

inline void RhioConcurrent::removeYield(int index) { yielding[index] = 0; }
