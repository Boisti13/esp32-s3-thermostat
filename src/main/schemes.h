#pragma once
// Colour schemes: the accent for "heating" and everything active (selected
// preset, Heating on, slider, flame) and the accent for "on, not heating".
// The background stays black in every scheme: on this panel dark fills
// look tinted and their edges stair-stepped, bright accents on black don't.
// Same order as the options of select.color_scheme (src/main/settings.yaml).

#include <cstdint>

struct PanelScheme {
  const char *name;
  uint32_t heat;
  uint32_t idle;
};

static const PanelScheme PANEL_SCHEMES[] = {
    {"Ember", 0xFF7A2F, 0x4C9BFF},
    {"Amber", 0xFFB020, 0x4C9BFF},
    {"Rose", 0xFF5C8A, 0x8C7BFF},
    {"Forest", 0x3DDC84, 0x4CC3FF},
    {"Ocean", 0x29C5F6, 0x7A8CFF},
    {"Mono", 0xE6E6E6, 0x8A8A8A},
};
static const int PANEL_SCHEME_COUNT = sizeof(PANEL_SCHEMES) / sizeof(PANEL_SCHEMES[0]);
