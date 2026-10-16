# Working on this repository

ESPHome + LVGL firmware that turns a Sunton/Guition ESP32-4827S043C (4.3"
480×272 touch display) into a Home Assistant thermostat panel. Layout and
how-tos are in [README.md](README.md); this is the checklist.
Infrastructure (Home Assistant, servers) is not in this repository.

## Ground rules

- **Start with `git pull` on `dev`.** The owner works from more than one
  computer.
- **Work on `dev`, commit and push to `dev`.** `main` gets releases only,
  when the owner asks.
- **Ask before side effects**: pushing, flashing, copying to Home Assistant,
  changing Home Assistant.
- **One change = one commit** with the YAML, the version bump, README and
  CHANGELOG.
- **The owner flashes.** Never upload to the device unasked.
- **No secrets in the repo.** `secrets.yaml` is ignored; the entity IDs in
  `src/main/entities.yaml` are the owner's real ones and fine to commit.

## Checking a change

ESPHome checks run on the build server (CT 117 on pve_rptu_dev), never
locally while it is reachable:

```bash
bash ../claude-setup/scripts/esphome-check.sh thermostat.yaml config    # every change
bash ../claude-setup/scripts/esphome-check.sh thermostat.yaml           # + compile
```

**Anything visible gets new screenshots** and a look at them: run
`scripts/screenshots.sh` on the dev box (has SDL, Xvfb, ImageMagick and the
ESPHome venv) from a copy of the working tree, copy `docs/images/` back, and
read the PNGs before calling the change done.

## Home Assistant copy

The ESPHome app builds from `/homeassistant/esphome/esp32-s3-thermostat/`
(`thermostat.yaml` + `src/`) via the stub
`/homeassistant/esphome/thermostat-panel.yaml` (sets `project_dir`). Copy a
new version there only when the owner asks: back up the old folder to
`/config/project-backups/esphome/esp32-s3-thermostat/` first, replace it
with `git archive <commit> thermostat.yaml src`, check the files match.
The shared `secrets.yaml` there has `thermostat_api_key`.

The panel deep-sleeps on battery: it is only reachable over the network
while awake. For OTA, wake it and turn on its *Stay awake* switch.

## Versions to bump

| Change | Bump |
|---|---|
| fix | patch: `thermostat.yaml` (`project.version`), README badge, CHANGELOG |
| new feature or setting | minor, same three places |
| needs a new `entities.yaml` or breaks setups | major |
| docs only | none; `## Unreleased` → *Under the hood* |

## Conventions

- A room is `src/room/room.yaml` (logic) + `src/room/tile.yaml` (widgets),
  included five times with `vars`; every ID in them starts with `r${i}_`.
  Room names and entities only in `src/main/entities.yaml`.
- Colours only as `hex_*` substitutions in `src/main/theme.yaml` (YAML uses
  the matching `col_*`, lambdas `lv_color_hex(0x${hex_*})`). Icons as `ic_*`
  substitutions; a new icon must also go into the `glyphs`/`extras` of every
  font that shows it.
- In YAML flow mappings (`{ ... }`) quote any value containing `${...}`, or
  the YAML parser fails.
- Labels that change text are positioned with `align` relative to their
  parent, never `align_to` another widget (that is computed once).
- Files use LF line endings (`.gitattributes`). Writing files from Python on
  Windows: `open(..., newline="\n")`.
- `demo/screenshot.yaml` must keep compiling: it uses every package except
  `hardware.yaml`, so new hardware-only parts go there and nowhere else.
