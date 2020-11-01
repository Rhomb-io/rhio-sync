#pragma once

const int PARALLEL_MAX = 10;

class Parallel {
 public:
  Parallel() {}

  void run(void (*done)(unsigned char* errors));
  bool add(unsigned char (*calllback)());
  bool yield();

 private:
  int current = 0;
  int added = 0;

  unsigned char errs[PARALLEL_MAX];

  bool next();
  void resetCounter();
  unsigned char (*callbacks[PARALLEL_MAX])();
};
