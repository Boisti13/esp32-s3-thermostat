# Changelog

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
