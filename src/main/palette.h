#pragma once
// Dark and light mode. Every neutral colour of the UI exists in both modes;
// the YAML is written in the dark colours (src/main/theme.yaml). Switching
// walks all widgets once and swaps each colour for its counterpart; colours
// set from lambdas go through pal(). Accents are per scheme (schemes.h).

#include <cstdint>

struct PanelColourPair {
  uint32_t dark;
  uint32_t light;
};

// dark (theme.yaml hex_*)  ->  light
static const PanelColourPair PANEL_PALETTE[] = {
    {0x000000, 0xF4F4F4},  // hex_bg
    {0xF0F0F0, 0x161616},  // hex_text
    {0x9A9A9A, 0x5C5C5C},  // hex_text2
    {0x4A4A4A, 0xB4B4B4},  // hex_line
    {0x303030, 0xD8D8D8},  // hex_track
    {0x3A3A3A, 0xD0D0D0},  // hex_card_hi
    {0x3C3C3C, 0xBEBEBE},  // hex_off
    {0xFFC042, 0xC48200},  // hex_warn
};

inline bool panel_light = false;

// A dark-mode colour as it is in the current mode
inline uint32_t pal(uint32_t dark) {
  if (!panel_light)
    return dark;
  for (const auto &p : PANEL_PALETTE)
    if (p.dark == dark)
      return p.light;
  return dark;
}

// Any palette colour (dark or light) as it is in the given mode
inline uint32_t panel_to_mode(uint32_t c, bool light) {
  for (const auto &p : PANEL_PALETTE) {
    if (light && c == p.dark)
      return p.light;
    if (!light && c == p.light)
      return p.dark;
  }
  return c;
}

// Theme colours (shared LVGL styles) become each widget's own colours, so
// the walk below can swap them. Buttons pass their state on to their
// labels, which are light normally and dark on a selected (accent) button.
inline void panel_localize(lv_obj_t *obj) {
  lv_style_value_t v;
  const bool in_button = lv_obj_get_parent(obj) && lv_obj_check_type(lv_obj_get_parent(obj), &lv_button_class);
  if (lv_obj_check_type(obj, &lv_button_class)) {
    lv_obj_add_flag(obj, LV_OBJ_FLAG_STATE_TRICKLE);
    if (lv_obj_get_local_style_prop(obj, LV_STYLE_BORDER_COLOR, &v, LV_PART_MAIN) != LV_RESULT_OK)
      lv_obj_set_style_border_color(obj, lv_color_hex(0x4A4A4A), LV_PART_MAIN);
    if (lv_obj_get_local_style_prop(obj, LV_STYLE_BG_COLOR, &v, LV_PART_MAIN | LV_STATE_PRESSED) != LV_RESULT_OK)
      lv_obj_set_style_bg_color(obj, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_PRESSED);
  } else if (lv_obj_check_type(obj, &lv_label_class) &&
             lv_obj_get_local_style_prop(obj, LV_STYLE_TEXT_COLOR, &v, LV_PART_MAIN) != LV_RESULT_OK) {
    lv_obj_set_style_text_color(obj, lv_color_hex(0xF0F0F0), LV_PART_MAIN);
    if (in_button)
      lv_obj_set_style_text_color(obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_CHECKED);
  }
  for (uint32_t i = 0; i < lv_obj_get_child_count(obj); i++)
    panel_localize(lv_obj_get_child(obj, i));
}

// Swap every palette colour of obj and its children to the given mode
inline void panel_recolor(lv_obj_t *obj, bool light) {
  static const lv_style_prop_t PROPS[] = {LV_STYLE_BG_COLOR, LV_STYLE_BORDER_COLOR, LV_STYLE_TEXT_COLOR,
                                          LV_STYLE_ARC_COLOR, LV_STYLE_OUTLINE_COLOR};
  static const lv_part_t PARTS[] = {LV_PART_MAIN, LV_PART_INDICATOR, LV_PART_KNOB};
  static const lv_state_t STATES[] = {LV_STATE_DEFAULT, LV_STATE_CHECKED, LV_STATE_PRESSED, LV_STATE_DISABLED};
  lv_style_value_t v;
  for (lv_part_t part : PARTS)
    for (lv_state_t state : STATES)
      for (lv_style_prop_t prop : PROPS) {
        const lv_style_selector_t sel = part | state;
        if (lv_obj_get_local_style_prop(obj, prop, &v, sel) != LV_RESULT_OK)
          continue;
        const uint32_t c = lv_color_to_u32(v.color) & 0xFFFFFF;
        const uint32_t n = panel_to_mode(c, light);
        if (n != c) {
          v.color = lv_color_hex(n);
          lv_obj_set_local_style_prop(obj, prop, v, sel);
        }
      }
  for (uint32_t i = 0; i < lv_obj_get_child_count(obj); i++)
    panel_recolor(lv_obj_get_child(obj, i), light);
}
