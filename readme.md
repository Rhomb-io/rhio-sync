# rhio-sync

High level set of experimental libraries to work with tasks and callbacks on embedded devices.

The main idea is to explore the possibility of parallelism and concurrence in microcontrollers, developing standard libraries for multiple architectures with few resources.

> It's a work in progress, there is a lot of experimental code.
> There is no context switching or other advanced features more tipically on RTOS.

## rhio-concurrent

Create an stack of tasks and run all recursively in the loop. Tasks can use special methods like "concurrent.yield()" or "concurrent.delay()" to work without blocking the loop.

```c++
// Example of blink a led while listening for input data in the serial port
#include "rhio-concurrent.h"
RhioConcurrent concurrent;

void cb1() {
  while (!Serial.available()) {
    concurrent.yield();
  }

  char y = Serial.read();
  Serial.print("read: ");
  Serial.println(y);
}

void cb2() {
  concurrent.delay(100, []() {
    digitalWrite(9, !digitalRead(9));
  });
}

void setup() {
  pinMode(9, OUTPUT);
  Serial.begin(9600);
  Serial.println(F("START"));

  concurrent.add(cb1);
  concurrent.add(cb2);
}

void loop() { concurrent.run(); }

```

## rhio-each

Create an stack of task and run once, calling each task in order, synchronous. If one task returns an error the rest of the tasks will be stopped and the error will be returned.

It uses a class to store the stack and can be manipulated to add or remove tasks, allowing to enable or disable them at runtime.

```c++
#include "rhio-each.h"
Each each;

unsigned char cb1() { /* do something */ return 0; }
unsigned char cb2() { /* do something */ return 0; }
unsigned char cb3() { /* do something */ return 0; }
unsigned char cb4() { /* do something */ return 0; }

void setup() {
  unsigned char (*cbs[4])() = {cb1, cb2, cb3, cb4};
  each.add(cbs, 4);
}
void loop() {
  unsigned char err = each.run();
  if (err > 0) {
    // A callback has return an error...
    // Expect "err" to be an error code
  }
}
```

## rhio-parallel

Create an stack of task and run once, calling each task in order, but allow to use "yield()" ro interrupt a task.

A final callback is called when all the tasks has been execute, passing as parameter an array of integers with the result of each executed task.

```c++
#include "rhio-parallel.h"
Parallel parallel;

unsigned char cb1() {
  while (!Serial.available()) {
    parallel.yield();
  }

  Serial.println("P1");
  return 0;
}

unsigned char cb2() {
  int x = 1;
  while (x) {
    if (!Serial.available()) {
      parallel.yield();
      continue;
    }

    char y = Serial.read();
    if (y == 'a') {
      Serial.println(y);
      x = 0;
    }
  }
  Serial.println("P2");
  return 3;
}

unsigned char cb3() {
  Serial.println("P3");
  return 5;
}

void setup() {
  Serial.begin(9600);
  Serial.println("START");

  parallel.add(cb1);
  parallel.add(cb2);
  parallel.add(cb3);

  // Run each method only once, and get the results.
  // Execution will be ordered in the way that method "add()" has been called,
  // but a task can interrupt its execution using "parallel.yield()"
  parallel.run([](unsigned char* results) {
    Serial.println(results[0]); // cb1
    Serial.println(results[1]); // cb2
    Serial.println(results[2]); // cb3
    // will print:
    // P3
    // P1
    // P2
  });
}

void loop() {
  /* ... */
}
```

## rhio-waterfall

A simple function that accepts an array of callbacks as first parameter and executes all of them. If one callback returns an error the execution of the rest of callbacks will be stopped and the error returned.

```c++
#include "rhio-waterfall.h"

void setup() {
  Serial.begin(9600);
  unsigned char (*cbs[2])();
  unsigned char err;

  cbs[0] = []() -> unsigned char {
    Serial.println("Test 1");
    return 0;
  };

  cbs[1] = []() -> unsigned char {
    Serial.println("Test 2");
    return 0;
  };

  err = waterfall(cbs, 2);
  if (err > 0) {
    // Something bad happend during the execution of a callback.
    // `err` should be an error code.
  }
}

void loop() {}

```
