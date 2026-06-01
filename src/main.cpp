#include <Arduino.h>
#include <rtos.h>
using rtos::Thread;

// these functions are imported from the other files
void piano_demo_setup();
void piano_demo_main_loop();
void prox_sensor_demo_setup();
void prox_sensor_demo_main_loop();

// arduino setup function - runs once at startup
void setup() {
    piano_demo_setup();
    prox_sensor_demo_setup();

    // start the prox sensor demo main loop in a background thread, so it runs
    // at the same time as loop()
    static Thread prox_demo_thread;
    prox_demo_thread.start(prox_sensor_demo_main_loop);
}

// arduino loop function - called after setup and runs forever
void loop() {
    // run the main loop for the piano demo
    piano_demo_main_loop();
}
