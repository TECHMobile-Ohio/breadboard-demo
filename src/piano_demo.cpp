/*
This demo uses a 4x4 keypad and a passive buzzer.

It will play increasing tones on the buzzer based on which key is pressed.
*/

#include <Arduino.h>

#include <hardware/clocks.h>

// macro to get the length of a fixed-size array
#define LEN(A) (sizeof(A) / sizeof((A)[0]))

// Row and Column pins for 4x4 keyboard
static const uint cols[] = {10, 11, 12, 13};
static const uint rows[] = {6, 7, 8, 9};

// Output pin for passive buzzer
static const uint buzzer = 14;

// Get the index (0-15) of the key that is currently pressed (or -1 if no key
// pressed).
static int get_key() {
    int pressed_button_index = -1;

    // for each column pin
    for (uint i = 0; i < LEN(cols); i++) {
        // turn on that column's pin as an output with voltage set to HIGH
        pinMode(cols[i], OUTPUT);
        digitalWrite(cols[i], HIGH);

        // for each row pin
        for (uint j = 0; j < LEN(rows); j++) {
            // check if that pin is reading high - meaning the corresponding
            // button at (i, j) is pressed
            if (digitalRead(rows[j])) {
                // update the return value to the corresponding index between 0
                // and 15
                pressed_button_index = (j * LEN(rows)) + i;
            }
        }
        // turn the column pin back off by switching back to an input with
        // pulldown
        pinMode(cols[i], INPUT_PULLDOWN);
    }
    return pressed_button_index;
}

// Set the specified pin to output a signal at the specified frequency using
// pulse-width-modulation
static void tone_pwm(const uint pin, const uint freq) {
    const uint slice = pwm_gpio_to_slice_num(pin);

    // frequency 0 - no sound. Turn the PWM off and exit
    if (0 == freq) {
        pwm_set_enabled(slice, false);
        return;
    }

    gpio_set_function(pin, GPIO_FUNC_PWM);

    const uint32_t clock = clock_get_hz(clk_sys);

    float div = 1.0;
    uint32_t top = (clock / freq) - 1;

    while (top > 0xFFFF) {
        top /= 2;
        div *= 2;
    }

    pwm_set_clkdiv(slice, div);
    pwm_set_wrap(slice, top);

    pwm_set_chan_level(slice, pwm_gpio_to_channel(pin), top / 2);

    pwm_set_enabled(slice, true);
}

// This will hold the frequencies for the 16 notes.
// The first one is A (440 hertz). We will compute the rest in the setup.
static float tones[16] = {440};

// run once to configure the hardware for the demo
void piano_demo_setup() {
    // set the buzzer pin to output mode
    pinMode(buzzer, OUTPUT);

    // set all the keypad row and column pins to input-pulldown mode
    for (size_t i = 0; i < LEN(rows); i++) {
        pinMode(rows[i], INPUT_PULLDOWN);
    }
    for (size_t i = 0; i < LEN(cols); i++) {
        pinMode(cols[i], INPUT_PULLDOWN);
    }

    // Compute tones for each half-step.
    // There are 12 half-step notes in an octave, so each one is (2 ^ (1/12))
    // times the frequency of the previous one.
    for (size_t i = 1; i < LEN(tones); i++) {
        tones[i] = 1.05946 * tones[i - 1];
    }
}

// main forever-loop that runs the demo
void piano_demo_main_loop() {
    // tracks the previous key that was pressed
    int prev_key = -1;

    while (true) {
        const int key = get_key();

        // if the key state has changed, update the tone
        if (key != prev_key) {
            if (key == -1) {
                // no key pressed - turn off sound
                tone_pwm(buzzer, 0);
            } else {
                // a key is pressed, play the corresponding tone
                tone_pwm(buzzer, tones[key]);
            }

            prev_key = key;
        }

        delay(50);
    }
}
