/*
This demo uses an RGB LED and an ultrasonic distance sensor to make a simple
proximity alarm.

The LED will change colors or flash based on the distance detected on the
sensor.
*/

#include <Arduino.h>

// Trigger and echo pins for ultrasonic distance sensor
static const uint trigPin = 20;
static const uint echoPin = 21;

// red, green, blue pins for multi-color LED
static const uint rPin = 3, gPin = 4, bPin = 5;

// set the red, green, and blue levels for the RGB LED
static void set_rgb(uint r, uint g, uint b) {
    analogWrite(rPin, r);
    analogWrite(gPin, g);
    analogWrite(bPin, b);
}

// read the current distance from the ultrasonic distance sensor in centimeters
static float read_distance() {
    // turn the distance sensor trigger pin on for 10 microseconds, then back
    // off digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    // check how long it takes for the echo pin to receive the sonar signal back
    const float duration = pulseIn(echoPin, HIGH);

    // convert to CM and return the value
    return (duration * .0343) / 2;
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

            // get the current time in milliseconds, and get the remainder of
            // division by 200, see if that's greater than 100. This is how we
            // alternate red/blue every 100ms.
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
