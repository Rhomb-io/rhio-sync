#pragma once

/**
 * @brief Run an array of callbacks
 *
 * Unlike Each, which allows us to save callbacks and execute them
 * when we want, sync::waterfall() will execute the callbacks in the same
 * moment the method is declared.
 *
 * @example
 * namespace myspace {
 *   // Declare some callbacks to feed the waterfall.
 *   uint8_t cb1() { msg::add("param", "value"); return 0; }
 *   uint8_t cb2() { msg::add("param", "value"); return 0; }
 *   uint8_t cb3() { msg::add("param", "value"); return 0; }
 *   uint8_t cb4() { msg::add("param", "value"); return 0; }
 *
 *   uint8_t init() {
 *     // Create the array of callbacks
 *     uint8_t (*cbs[4])() = {cb1, cb2, cb3, cb4};
 *     // execute all
 *     uint8_t err = sync.waterfall(cbs, 4);
 *     // return standard error code, 0 = no errors
 *     if (err) errs:process(err);
 *   }
 * }
 *
 * @param callbacks Array of callbacks
 * @param size Number of callbacks in the array
 */
unsigned char waterfall(unsigned char (*callbacks[])(), int size);
unsigned char waterfall(unsigned char (*callbacks[])(int p), int size, int p);
