#include "rhio-each.h"

void Each::reset() {
  for (auto i = 0; i < 10; i++) {
    this->callbacks[i] = 0;
  }
}

/**
 * @brief Add one callback
 *
 * @param callback
 * @return true If the callback has been added
 * @return false If the storage is full
 */
bool Each::add(unsigned char (*callback)()) {
  for (auto i = 0; i < 10; i++) {
    if (this->callbacks[i] == 0) {
      this->callbacks[i] = callback;
      return true;
    }
  }
  return false;
}

/**
 * @brief Add an array of callbacks
 *
 * @param _callbacks The array
 * @param len The number of callbacks in the array
 */
void Each::add(unsigned char (*_callbacks[])(), unsigned char len) {
  for (auto i = 0; i < len; i++) {
    add(_callbacks[i]);
  }
  return;
}

/**
 * @brief Run all the added callbacks
 * @return unsigned char Error code
 */
unsigned char Each::run() {
  for (auto i = 0; i < 10; i++) {
    if (this->callbacks[i] != 0) {
      unsigned char err = this->callbacks[i]();
      if (err > 0) {
        return err;
      }
    }
  }
  return 0;
}
