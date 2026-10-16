#pragma once
// Colour schemes: the accent for "heating" and everything active (selected
// preset, Heating on, slider, flame) and the accent for "on, not heating",
// each in a dark-mode and a (deeper, readable on white) light-mode version.
// Dark mode keeps a black background in every scheme: on this panel dark
// fills look tinted and their edges stair-stepped, bright accents don't.
// Same order as the options of select.color_scheme (src/main/settings.yaml).

#include <cstdint>

struct PanelScheme {
  const char *name;
  uint32_t heat, idle;              // dark mode
  uint32_t heat_light, idle_light;  // light mode
};

static const PanelScheme PANEL_SCHEMES[] = {
    {"Ember", 0xFF7A2F, 0x4C9BFF, 0xF0661A, 0x2F7FE8},
    {"Amber", 0xFFB020, 0x4C9BFF, 0xD98E00, 0x2F7FE8},
    {"Rose", 0xFF5C8A, 0x8C7BFF, 0xE33E6C, 0x6C58E8},
    {"Forest", 0x3DDC84, 0x4CC3FF, 0x1E9E58, 0x1E8FCC},
    {"Ocean", 0x29C5F6, 0x7A8CFF, 0x0A9AD0, 0x5566E0},
    {"Mono", 0xE6E6E6, 0x8A8A8A, 0x262626, 0x7A7A7A},
};
static const int PANEL_SCHEME_COUNT = sizeof(PANEL_SCHEMES) / sizeof(PANEL_SCHEMES[0]);
