#include <Arduino.h>

// Only include Mbed RTOS if compiling for the Raspberry Pi Pico
#if defined(ARDUINO_ARCH_RP2040)
  #include <rtos.h>
  using rtos::Thread;
#endif

// these functions are imported from the other files
void piano_demo_setup();
void piano_demo_main_loop();
void prox_sensor_demo_setup();
void prox_sensor_demo_main_loop();

#if defined(ESP32)
// FreeRTOS task wrapper required for ESP32 background tasks
void prox_sensor_task(void *pvParameters) {
    prox_sensor_demo_main_loop();
    vTaskDelete(NULL);
}
#endif

// arduino setup function - runs once at startup
void setup() {
    piano_demo_setup();
    prox_sensor_demo_setup();

    // start the prox sensor demo main loop in a background thread, so it runs
    // at the same time as loop()
    #if defined(ARDUINO_ARCH_RP2040)
        // Pico uses Mbed OS threads
        static Thread prox_demo_thread;
        prox_demo_thread.start(prox_sensor_demo_main_loop);
    #elif defined(ESP32)
        // ESP32 uses FreeRTOS tasks assigned to Core 0
        xTaskCreatePinnedToCore(
            prox_sensor_task, "ProxTask", 4096, NULL, 1, NULL, 0
        );
    #endif
}

// arduino loop function - called after setup and runs forever
void loop() {
    // run the main loop for the piano demo
    piano_demo_main_loop();
}