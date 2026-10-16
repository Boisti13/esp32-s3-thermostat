#pragma once
// Pin levels held through deep sleep. Without the ESP32 driving them, the
// display bus, touch I2C, audio amplifier and SD lines float, and floating
// CMOS inputs on the panel or the amp can draw current. Held before deep
// sleep (src/main/power.yaml), released first thing at boot, before the
// drivers set the pins up again.
//
// Not touched: GPIO45 (display CS) is a boot-strap pin (flash voltage); the
// backlight (GPIO1) and touch INT (GPIO3) are handled in power.yaml.
// Audio pins: NS4168 amplifier on IO42/IO2/IO41 (Guition schematic v0.2).

#include "driver/gpio.h"

struct SleepPin {
  gpio_num_t pin;
  int level;
};

static const SleepPin SLEEP_PINS[] = {
    // display quad SPI: clock and data low (panel is asleep)
    {GPIO_NUM_47, 0}, {GPIO_NUM_21, 0}, {GPIO_NUM_48, 0}, {GPIO_NUM_40, 0}, {GPIO_NUM_39, 0},
    // touch I2C idle high (no current through the pull-ups), reset released
    {GPIO_NUM_8, 1}, {GPIO_NUM_4, 1}, {GPIO_NUM_38, 1},
    // audio amplifier inputs low: no clock, no data
    {GPIO_NUM_42, 0}, {GPIO_NUM_2, 0}, {GPIO_NUM_41, 0},
    // SD card: not selected, clock and data low
    {GPIO_NUM_10, 1}, {GPIO_NUM_11, 0}, {GPIO_NUM_12, 0},
};

inline void sleep_pins_hold() {
  for (const auto &p : SLEEP_PINS) {
    gpio_reset_pin(p.pin);  // back to plain GPIO (off the SPI/I2C/I2S signals)
    gpio_set_direction(p.pin, GPIO_MODE_OUTPUT);
    gpio_set_level(p.pin, p.level);
    gpio_hold_en(p.pin);
  }
}

inline void sleep_pins_release() {
  for (const auto &p : SLEEP_PINS)
    gpio_hold_dis(p.pin);
}
