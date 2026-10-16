#pragma once
// Last known Home Assistant values, kept in RTC memory through deep sleep
// (cleared on power-up), so the panel shows them the moment it wakes instead
// of empty dials. Written by save_cache, read on boot (src/main/power.yaml).

#include <cstring>
#include <string>
#include "esp_attr.h"

static const uint32_t PANEL_CACHE_MAGIC = 0x7E3A0001;  // bump when the layout changes

struct RoomCache {
  float tgt, cur, hum, valve, lo, hi;
  char mode[12], action[16], preset[16];
  bool window;
};

struct PanelCache {
  uint32_t magic;
  RoomCache room[5];
  float outdoor;
  bool main_valve;
  char central[24];
};

RTC_DATA_ATTR PanelCache panel_cache;

inline void cache_copy(char *dst, size_t size, const std::string &src) {
  strncpy(dst, src.c_str(), size - 1);
  dst[size - 1] = '\0';
}
