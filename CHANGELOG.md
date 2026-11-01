# Changelog

## v3.3.0 — 2026-11-01

### Changed

- The heating dial no longer reacts to touch (no dragging the knob or
  tapping the ring): the target is set with − / + only. Swipes to the
  next room now work from anywhere on the dial too.

### Fixed

- A touch on the dimmed screen pressed whatever was under the finger; now
  it only brightens the screen.
- Library stayed empty: Home Assistant wraps the action's answer in
  `{"response": …}`, and the panel looked for the titles one level too
  high.
- − / + sit 10 px higher, clear of the page dots.

## v3.2.2 — 2026-11-01

### Fixed

- Volume popup: the speaker icon sat off the line of the level; icon and
  level are now centred on each other.

## v3.2.1 — 2026-11-01

### Changed

- The volume button is a small round icon button (the icon shows the
  level) instead of a wide bar; the level is in the popup.

## v3.2.0 — 2026-11-01

### Changed

- Music page less cramped: the volume slider is gone; one volume button
  opens a popup with − / + (5 % steps, hold to repeat) and mute, closing
  4 s after the last tap. The library button sits on the cover's corner
  (tapping the cover opens it too), and the playback controls moved down
  into the bottom row.

### Fixed

- Library stayed empty ("expected int at 'offset'"): ESPHome sends action
  data as text; `limit` and `offset` now go through a template so Home
  Assistant passes numbers.

## v3.1.0 — 2026-11-01

### Changed

- Waking from sleep returns to the page and room the panel was showing
  (room, music page or settings; the library comes back as the music
  page) instead of the home room. Kept in RTC memory with the other
  values; after a power-up it still starts on the home room.

## v3.0.1 — 2026-11-01

### Fixed

- No cover: Music Assistant's image proxy only serves 0, 80, 160, 256,
  512 and 1024 px and answered 150 px with an error. The panel asks for
  160 px and scales it to 150.

## v3.0.0 — 2026-11-01

Needs the new `music_assistant_entry` setting in `src/main/entities.yaml`.

### Added

- **Library** (list button on the music page): pick an audiobook,
  playlist, album, track or radio station from Music Assistant, most
  recently played first, five per page; it plays on the selected speaker.

### Fixed

- Pausing showed *Nothing playing* and dropped the cover: Music
  Assistant's Sendspin players report a paused track as `idle`. A player
  that is idle with a title now counts as paused, and a cover that
  vanishes for a moment while the state changes is kept.

## v2.0.0 — 2026-11-01

Needs the new `music_N_entity` and `ha_url` settings in
`src/main/entities.yaml`.

### Added

- **Music page**, opened with the ♫ button bottom left of every room (orange
  while something plays): cover art, title, artist, album, a seekable
  progress bar, shuffle, previous, play/pause, next, repeat, volume and
  mute for a Music Assistant (or any) media player. Titles show umlauts and
  accents. The cover is downloaded only while the page is open.
- **Speaker list** (tap the speaker name): pick which of up to four players
  the page controls, and join other speakers to it so they play in sync
  (`media_player.join` / `unjoin`).

### Under the hood

- New `src/music/` (logic, page, one include per player slot); the demo
  shows the music page with made-up speakers and a generated cover, and
  the screenshot script renders it.

## v1.1.2 — 2026-10-16

### Fixed

- Waking was slow and touch stopped working after v1.1.1: the touch and
  display buses start before the held pins were released. The pin holding
  is removed again; it made no difference to the sleep current (still
  ~42 mA with touch wake, measured).

## v1.1.1 — 2026-10-16

### Changed

- Deep sleep held the display bus, touch I²C, audio amplifier and SD card
  pins at fixed levels (removed again in v1.1.2).

## v1.1.0 — 2026-10-16

### Added

- *Wake by touch* setting (settings page and Home Assistant, on by
  default). Off: the touch controller is put to sleep with the panel and
  only the BOOT button wakes it, for less current in deep sleep.

## v1.0.4 — 2026-10-16

### Fixed

- Deep sleep drew 61 mA: the display controller kept running. It is now
  sent *display off* and *sleep in* before the ESP32 sleeps.

## v1.0.3 — 2026-10-16

### Added

- *Wake reason* sensor and log line: what started the panel (touch, BOOT
  button, power on, RST button, restart/update, crash, watchdog,
  brownout).

## v1.0.2 — 2026-10-16

### Fixed

- The header sometimes said "no connection" while Home Assistant was
  connected: any API client leaving (the ESPHome app's dashboard or log
  viewer) counted as Home Assistant leaving. Now it shows whether a client
  is subscribed to the panel's states, which only Home Assistant is.

## v1.0.1 — 2026-10-16

### Changed

- Fixed IP from `secrets.yaml` (`thermostat_ip`, `thermostat_gateway`,
  `thermostat_subnet`): no DHCP on each wake, live data about a second
  sooner. **Add these secrets before updating.**

## v1.0.0 — 2026-10-16

First release, tested on the Guition JC4827W543C: same firmware as v0.6.0.

- One tile per room for five Home Assistant climate entities (made for
  Versatile Thermostat): dial with drag and −/+, presets, heating on/off,
  room temperature, humidity, valve opening, window warning. Swipe to
  switch rooms.
- Header with clock, outdoor temperature, main heating supply, central
  mode *Stopped* and Home Assistant connection.
- Settings page (⋯): dark, light or auto mode (by the sun), six colour
  schemes, brightness, *Sleep after*; all also Home Assistant entities.
- Battery operation: dims, then deep sleep; wakes on touch or BOOT with the
  last values on screen. *Stay awake* switch for OTA updates. Optional
  battery level (needs a resistor divider).

## v0.6.0 — 2026-10-16

### Added

- Light mode, and *Auto* (light while the sun is up, from Home Assistant's
  `sun.sun`; the default). Three buttons next to the colour swatches, and a
  *Mode* entity in Home Assistant. The last sun state survives deep sleep.
- Light-mode variants of every colour scheme's accents.

### Changed

- Text on a selected button is dark on the accent (was white, unreadable
  in the Mono scheme).

## v0.5.0 — 2026-10-16

### Added

- Settings page, opened with ⋯ top left: colour scheme (Ember, Amber, Rose,
  Forest, Ocean, Mono), brightness slider, *Sleep after* (30 s, 1 min,
  5 min, Never) and an info line (version, IP, Wi-Fi signal).
- The three settings are Home Assistant entities too and survive reboots
  and deep sleep.

### Changed

- Deep sleep after 1 min by default (was 40 s), or as set.
- Room names start right of the ⋯ button.
- `entities.yaml`: `idle_dim`, `idle_sleep`, `dim_brightness`,
  `active_brightness(_float)` are replaced by `idle_dim_s`,
  `dim_brightness_f` and the `default_*` first-boot values.

## v0.4.0 — 2026-10-16

### Changed

- New look for this panel: outlined buttons on black, solid orange for the
  active preset and *Heating on*, humidity and valve without tiles. Dim
  filled shapes had stair-stepped corners in 16-bit colour.
- Header: room name on the left, clock and status icons on the right (they
  overlapped). The window icon next to the name is gone; the dial still
  says *Window open*. *Central mode stopped* is a radiator-off icon.

### Fixed

- Swiping worked in one direction only: rooms are now switched from the
  touch coordinates (50 px sideways), and a swipe no longer presses the
  button it started on. Swipes and touches are logged.

## v0.3.3 — 2026-10-16

### Changed

- Black background and neutral greys. The panel stretches dark shades and
  tints them teal: the old navy cards came out bright teal, and the uneven
  dark tones made rounded corners and the dial look stair-stepped.

## v0.3.2 — 2026-10-16

### Fixed

- Swiping did nothing: the swipe went past the room view to the screen, so
  the handler never saw it.

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
