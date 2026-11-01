#pragma once
// Helpers for the music page (src/main/music.yaml).

#include <cstdio>
#include <cstdint>
#include <ctime>
#include <cstring>
#include <string>

// Home Assistant's media_position_updated_at ("2026-11-01T06:32:02.306+00:00",
// always UTC) as Unix time; 0 if it doesn't parse.
inline int64_t music_parse_utc(const std::string &s) {
  int y, mo, d, h, mi, se;
  if (sscanf(s.c_str(), "%d-%d-%dT%d:%d:%d", &y, &mo, &d, &h, &mi, &se) != 6)
    return 0;
  // Days since 1970-01-01 (Howard Hinnant's days_from_civil)
  y -= mo <= 2;
  const int era = (y >= 0 ? y : y - 399) / 400;
  const unsigned yoe = (unsigned) (y - era * 400);
  const unsigned doy = (153 * (mo + (mo > 2 ? -3 : 9)) + 2) / 5 + d - 1;
  const unsigned doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;
  const int64_t days = (int64_t) era * 146097 + (int64_t) doe - 719468;
  return days * 86400 + h * 3600 + mi * 60 + se;
}

// 75 -> "1:15", 3640 -> "1:00:40"
inline void music_fmt_time(char *buf, size_t n, int s) {
  if (s < 0) s = 0;
  if (s >= 3600)
    snprintf(buf, n, "%d:%02d:%02d", s / 3600, (s / 60) % 60, s % 60);
  else
    snprintf(buf, n, "%d:%02d", s / 60, s % 60);
}

// Cover URL for the panel: relative paths (HA's media player proxy) get
// Home Assistant's address, and Music Assistant's image proxy is asked for
// a size close to what the page shows instead of 512 px (it only serves
// 0, 80, 160, 256, 512 and 1024; anything else is a 400).
inline std::string music_cover_url(const std::string &pic, const char *ha_url, int size) {
  if (pic.empty() || pic == "None" || pic == "unknown" || pic == "unavailable")
    return "";
  std::string url = pic[0] == '/' ? std::string(ha_url) + pic : pic;
  if (url.find("/imageproxy") != std::string::npos) {
    const size_t p = url.find("size=");
    if (p != std::string::npos) {
      size_t e = p + 5;
      while (e < url.size() && isdigit((unsigned char) url[e])) e++;
      url.replace(p + 5, e - p - 5, std::to_string(size));
    }
  }
  return url;
}

// A player slot's sensors (src/music/player.yaml), filled in at boot by
// src/music/music.yaml so scripts can work on the selected one by number.
struct MusicPlayer {
  const char *entity;
  esphome::text_sensor::TextSensor *state, *name, *title, *artist, *picture, *position_at, *repeat, *group;
  esphome::sensor::Sensor *volume, *duration, *position;
  esphome::binary_sensor::BinarySensor *shuffle, *muted;

  bool used() const { return strcmp(entity, "media_player.none") != 0; }
  bool playing() const { return used() && state->state == "playing"; }
  // A track to show: also paused, and idle with a title (Music Assistant's
  // Sendspin players report a paused track as idle and keep its details)
  bool active() const {
    if (!used()) return false;
    const std::string &s = state->state;
    if (s == "playing" || s == "paused" || s == "buffering") return true;
    const std::string &t = title->state;
    return s == "idle" && title->has_state() && !t.empty() && t != "None" && t != "unknown";
  }
};

static const int MUSIC_PLAYERS = 4;
inline MusicPlayer music_players[MUSIC_PLAYERS];

// Whether `other` is in this player's group (group_members holds entity IDs)
inline bool music_in_group(const MusicPlayer &leader, const MusicPlayer &other) {
  return &leader != &other && other.used() &&
         leader.group->state.find(std::string(other.entity) + "'") != std::string::npos;
}

// A text attribute that holds something
inline bool music_valid(const esphome::text_sensor::TextSensor *t) {
  const std::string &s = t->state;
  return t->has_state() && !s.empty() && s != "None" && s != "unknown" && s != "unavailable";
}
