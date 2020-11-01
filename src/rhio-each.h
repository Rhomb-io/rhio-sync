#pragma once

/**
 * @brief Save a set of callbacks in memory and execute on demand at the same
 * time, one by one, in order (synchronous)
 *
 * If one of the callbacks return an error the execution will be stop.
 *
 * Is required that all the callbacks added return an standar error value where
 * 0 = true and error = 1 or >. Read more about error nomenclatures on
 * core/error.h
 *
 * @example
 *   // start declaring a new Each object
 *   Each myEach;
 *
 *   // Declare some callbacks to feed myEach.
 *   unsigned char cb1() { msg::add("param", "value"); return 0; }
 *   unsigned char cb2() { msg::add("param", "value"); return 0; }
 *   unsigned char cb3() { msg::add("param", "value"); return 0; }
 *   unsigned char cb4() { msg::add("param", "value"); return 0; }
 *
 *   void setup() {
 *     // Fill myEach
 *     unsigned char (*cbs[4])() = {cb1, cb2, cb3, cb4};
 *     myEach.add(cbs, 4);
 *
 *     // subscribe to any method
 *     puf::on(EV_TICK1, doAction);
 *   }
 *
 *   void loop() {
 *     myEach.run();
 *   }
 */
class Each {
 public:
  /// clear all
  void reset();

  /**
   * @brief Add one callback
   *
   * @param callback
   * @return true If the callback has been added
   * @return false If the storage is full
   */
  bool add(unsigned char (*callback)());

  /**
   * @brief Add an array of callbacks
   *
   * @param _callbacks The array
   * @param len The number of callbacks in the array
   */
  void add(unsigned char (*_callbacks[])(), unsigned char len);

  /**
   * @brief Run all the added callbacks
   * @return unsigned char Error code
   */
  unsigned char run();

 private:
  /// Storage for callbacks. Max 10.
  unsigned char (*callbacks[10])();
};
