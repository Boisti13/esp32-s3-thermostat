#!/usr/bin/env bash
# Render the UI without the board: builds demo/screenshot.yaml for the PC
# (ESPHome host platform + SDL), runs it in a virtual X display and saves one
# PNG per room to docs/images/room-<n>.png, plus the settings page, one
# other colour scheme, light mode, the music page and its speaker list.
#
# Linux only. Needs ESPHome (requirements.txt), libsdl2-dev, xvfb, imagemagick.
#   bash scripts/screenshots.sh
set -euo pipefail
cd "$(dirname "$0")/.."

esphome compile demo/screenshot.yaml
bin=$(find demo/.esphome/build/thermostat-demo -type f -name program -perm -u+x | head -1)

Xvfb :97 -screen 0 480x272x24 >/dev/null 2>&1 &
xvfb=$!
trap 'kill $app $xvfb 2>/dev/null || true' EXIT
sleep 1
DISPLAY=:97 "$bin" >/dev/null 2>&1 &
app=$!

# The demo moves to the next room every 3 s after boot; shoot mid-way.
mkdir -p docs/images
sleep 1.5
for name in room-0 room-1 room-2 room-3 room-4 settings scheme-ocean light light-settings music-light music speakers; do
  DISPLAY=:97 import -window root "docs/images/$name.png"
  sleep 3
done
echo "saved docs/images/room-{0..4}.png, settings.png, scheme-ocean.png, light.png, light-settings.png, music.png, music-light.png, speakers.png"
