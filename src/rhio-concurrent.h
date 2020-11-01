#pragma once

const int RHIO_CONCURRENT_MAX = 5;

class RhioConcurrent {
 public:
  RhioConcurrent();

  void run();
  bool add(void (*calllback)());
  bool yield();
  void delay(unsigned long millis, void (*callback)());

 private:
  int current = 0;
  int added = 0;
  char yielding[RHIO_CONCURRENT_MAX] = {0};
  unsigned long timers[RHIO_CONCURRENT_MAX];

  void (*callbacks[RHIO_CONCURRENT_MAX])();

  void next();
  void addYield(int index);
  void removeYield(int index);
};
