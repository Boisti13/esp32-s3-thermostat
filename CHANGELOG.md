# Changelog

## v0.3.1 — 2026-10-16

### Fixed

- Touch was mirrored: taps landed on the opposite side and swiping only
  worked one way.
- Swiping was choppy and drew the screen in visible strips: a swipe now
  switches rooms in a single full-frame redraw.

### Changed

- Display SPI at 40 MHz (was 20), LVGL draws whole frames.
- Slightly heavier digits in the dial (less jagged on the 128 ppi panel).

## v0.3.0 — 2026-10-16

### Changed

- **Targets the Guition JC4827W543C**, the board this was meant for: NV3041A
  panel on quad SPI, backlight on GPIO1, GT911 on SDA 8 / SCL 4 / INT 3.
  v0.1.0–v0.2.1 used the pins of the look-alike Sunton ESP32-4827S043C, so
  the screen stayed dark and touch didn't answer.
- Display back to 480 × 272 (the 280-row workaround was for the Sunton's RGB
  driver).
- Touch uses its interrupt again; the same pin (GPIO3) wakes from deep sleep.
- Optional battery level reads an ADC1 pin (`battery_adc_pin`, default
  GPIO5).

## v0.2.1 — 2026-10-16

### Fixed

- Screen stayed dark: the panel driver failed to start ("lcd setup failed")
  because 272 rows aren't a multiple of the driver's 10-row bounce buffer.
  The display now runs as 480 × 280 (the last 8 rows are off the glass).
- Deep-sleep wake-ups counted as failed boots and would have put the panel
  into safe mode after ten sleep cycles (`boot_is_good_after: 10s`).

## v0.2.0 — 2026-10-16

### Added

- Battery operation: deep sleep after 40 s idle (dim after 15 s), wake by
  touch (if the GT911 INT line is connected) or the BOOT button.
- The last known values are kept through deep sleep and shown right after
  waking, before Home Assistant has reconnected.
- *Stay awake* switch in Home Assistant (for OTA updates or USB power).
- Optional battery level (`src/main/battery.yaml`, needs a resistor divider
  to GPIO17): status bar icon and Home Assistant sensors.

### Changed

- Backlight at 70 % while in use (`active_brightness`), 15 % when dimmed.
- Wi-Fi connects with `fast_connect` and light power save.
- Touch is polled; GPIO18 (INT) is only used to wake from deep sleep.

## v0.1.0 — 2026-10-16

First version.

### Added

- Swipeable tile per room (five rooms) for Home Assistant climate entities,
  made for Versatile Thermostat.
- Dial with drag, tap and − / + (hold to repeat); the new target is sent
  when you let go.
- Presets Comfort / Eco / Frost / Boost, heating on/off per room.
- Room temperature, humidity, valve opening, window-open warning.
- Status bar: clock, outdoor temperature, main heating supply, central mode
  *Stopped*, Home Assistant connection.
- Screen saver: dim after 60 s, dark after 5 min, touch wakes; backlight as
  a Home Assistant light.

### Under the hood

- ESPHome 2026.9.1, LVGL 9, `mipi_rgb` driver for the ST7262 RGB panel,
  GT911 touch, octal PSRAM.
- `demo/screenshot.yaml` and `scripts/screenshots.sh` render the UI on
  Linux (SDL) for the README.
