#pragma once
// Send a raw command to the display controller. ESPHome's mipi_spi driver
// keeps write_command_() protected; a derived class may take its address,
// so this reaches it without changing ESPHome. Used to put the NV3041A to
// sleep before the ESP32 does (src/main/power.yaml): the panel has its own
// picture memory and would otherwise keep scanning the LCD all night.

#include <cstdint>

template<typename T> struct PanelCommandAccess : T {
  static void send(T *display, uint8_t cmd) {
    auto fn = static_cast<void (T::*)(uint8_t)>(&PanelCommandAccess::write_command_);
    (display->*fn)(cmd);
  }
};

template<typename T> inline void panel_command(T *display, uint8_t cmd) {
  PanelCommandAccess<T>::send(display, cmd);
}
