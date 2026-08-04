/*
This demo uses an RGB LED and an ultrasonic distance sensor to make a simple
proximity alarm.

The LED will change colors or flash based on the distance detected on the
sensor.
*/

#include <Arduino.h>

// --- AUTOMATIC PIN MAPPING ---
#if defined(ESP32)
  // Trigger and echo pins for ESP32 ultrasonic distance sensor
  static const uint trigPin = 32;
  static const uint echoPin = 33;

  // red, green, blue pins for ESP32 multi-color LED
  static const uint rPin = 26, gPin = 27, bPin = 14;
#else 
  // Trigger and echo pins for Pico ultrasonic distance sensor
  static const uint trigPin = 20;
  static const uint echoPin = 21;

  // red, green, blue pins for Pico multi-color LED
  static const uint rPin = 3, gPin = 4, bPin = 5;
#endif

// set the red, green, and blue levels for the RGB LED
static void set_rgb(uint r, uint g, uint b) {
    analogWrite(rPin, r);
    analogWrite(gPin, g);
    analogWrite(bPin, b);
}

// read the current distance from the ultrasonic distance sensor in centimeters
static float read_distance() {
    // turn the distance sensor trigger pin on for 10 microseconds, then back off
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    // Add a 30,000us (30ms) timeout (~5 meters max) so pulseIn doesn't wait forever
    const float duration = pulseIn(echoPin, HIGH, 30000);

    // If no echo was received, return -1 (invalid)
    if (duration == 0) {
        return -1;
    }

    // convert to CM and return the value
    return (duration * 0.0343) / 2;
}

// run once to configure the hardware for the demo
void prox_sensor_demo_setup() {
    // set the LED pins and prox sensor trigger pins as outputs
    pinMode(rPin, OUTPUT);
    pinMode(gPin, OUTPUT);
    pinMode(bPin, OUTPUT);

    pinMode(trigPin, OUTPUT);
    digitalWrite(trigPin, LOW);

    // set the prox sensor echo pin as input
    pinMode(echoPin, INPUT);
}

// main forever-loop that runs the demo
void prox_sensor_demo_main_loop() {
    while (true) {
        const float distance = read_distance();

        // IF READING IS INVALID (-1), IGNORE IT AND KEEP PREVIOUS COLOR
        if (distance < 0) {
            delay(50);
            continue;
        }

        uint red = 0;
        uint green = 0;
        uint blue = 0;

        // determine the color values based on distance:
        if (distance > 50) {
            // distance is greater than 50 CM: set LED to solid green
            green = 255;

        } else if (distance > 25) {
            // distance is between 50 and 25 CM: set to a color between red and
            // green, based on the distance
            float ratio = (distance - 25) / 25;
            green = ratio * 255;
            red = 255 - green;

        } else {
            // distance is less than 25 CM: alternate flashing red and blue
            if ((millis() % 200) > 100) {
                red = 255;
            } else {
                blue = 255;
            }
        }

        // update the LED color
        set_rgb(red, green, blue);

        delay(100);
    }
}