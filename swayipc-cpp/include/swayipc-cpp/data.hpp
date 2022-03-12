#pragma once

#include <swayipc-cpp/container.hpp>

namespace swayipc::data {

struct command_success_s {
    bool success;
    std::optional<std::string> error;
};
JSON_TYPE_HEADER(command_success_s)

struct workspace_s {
    int num;
    std::string name;
    bool visible;
    bool focused;
    bool urgent;
    rect_s rect;
    std::string output;
};
JSON_TYPE_HEADER(workspace_s)

enum class subpixel_hinting_t {
    RGB,
    BGR,
    VRGB,
    VBGR,
    NONE,
};
JSON_TYPE_HEADER(subpixel_hinting_t)

enum class transform_t {
    NORMAL,
    _90,
    _180,
    _270,
    FLIPPED_90,
    FLIPPED_180,
    FLIPPED_270
};
JSON_TYPE_HEADER(transform_t)

struct mode_s {
    int width;
    int height;
    int refresh;
};
JSON_TYPE_HEADER(mode_s)

struct output_s {
    std::optional<std::string> name;
    std::optional<std::string> make;
    std::optional<std::string> model;
    std::optional<std::string> serial;
    std::optional<bool> active;
    std::optional<bool> dpms;
    std::optional<bool> primary;
    std::optional<float> scale;
    std::optional<subpixel_hinting_t> subpixel_hinting;
    std::optional<transform_t> transform;
    std::optional<std::string> current_workspace;
    std::optional<std::vector<mode_s>> modes;
    std::optional<mode_s> current_mode;
    std::optional<rect_s> rect;
    std::optional<bool> focused;
};
JSON_TYPE_HEADER(output_s)

enum class bar_mode_t {
    DOCK,
    HIDE,
    INVISIBLE
};
JSON_TYPE_HEADER(bar_mode_t)

enum class bar_position_t {
    BOTTOM,
    TOP
};
JSON_TYPE_HEADER(bar_position_t)

struct bar_gaps_t {
    int top;
    int right;
    int bottom;
    int left;
};
JSON_TYPE_HEADER(bar_gaps_t)

using color_t = std::string;

struct bar_colors_s {
    color_t background;
    color_t statusline;
    color_t separator;
    color_t focused_background;
    color_t focused_statusline;
    color_t focused_separator;
    color_t focused_workspace_text;
    color_t focused_workspace_bg;
    color_t focused_workspace_border;
    color_t active_workspace_text;
    color_t active_workspace_bg;
    color_t active_workspace_border;
    color_t inactive_workspace_text;
    color_t inactive_workspace_bg;
    color_t inactive_workspace_border;
    color_t urgent_workspace_text;
    color_t urgent_workspace_bg;
    color_t urgent_workspace_border;
    color_t binding_mode_text;
    color_t binding_mode_bg;
    color_t binding_mode_border;
};
JSON_TYPE_HEADER(bar_colors_s)

struct bar_config_s {
    std::string id;
    bar_mode_t mode;
    bar_position_t position;
    std::string status_command;
    std::string font;
    bool workspace_buttons;
    bool binding_mode_indicator;
    bool verbose;
    bar_colors_s colors;
    bar_gaps_t gaps;
    int bar_height;
    int status_padding;
    int status_edge_padding;
};
JSON_TYPE_HEADER(bar_config_s)

struct version_s {
    int major;
    int minor;
    int patch;
    std::string human_readable;
    std::string loaded_config_file_name;
};
JSON_TYPE_HEADER(version_s)

enum class input_type_t {
    KEYBOARD,
    POINTER,
    TOUCH,
    TABLET_TOOL,
    TABLET_PAD,
    SWITCH
};
JSON_TYPE_HEADER(input_type_t)

enum class send_events_t {
    DISABLED,
    ENABLED,
    DISABLED_ON_EXTERNAL_MOUSE
};
JSON_TYPE_HEADER(send_events_t)

enum class state_t {
    DISABLED,
    ENABLED
};
JSON_TYPE_HEADER(state_t)

enum class tab_button_map_t {
    LMR,
    LRM
};
JSON_TYPE_HEADER(tab_button_map_t)

enum class accel_profile_t {
    NONE,
    FLAT,
    ADAPTIVE
};
JSON_TYPE_HEADER(accel_profile_t)

enum class click_method_t {
    NONE,
    BUTTON_AREAS,
    CLICKFINGER
};
JSON_TYPE_HEADER(click_method_t)

enum class scroll_method_t {
    NONE,
    TWO_FINGER,
    EDGE,
    ON_BUTTON_DOWN
};
JSON_TYPE_HEADER(scroll_method_t)

struct libinput_s {
    std::optional<send_events_t> send_events;
    std::optional<state_t> tap;
    std::optional<tab_button_map_t> tap_button_map;
    std::optional<state_t> tap_drag;
    std::optional<state_t> tap_drag_lock;
    std::optional<double> accel_speed;
    std::optional<accel_profile_t> accel_profile;
    std::optional<state_t> natural_scroll;
    std::optional<state_t> left_handed;
    std::optional<click_method_t> click_method;
    std::optional<state_t> middle_emulation;
    std::optional<scroll_method_t> scroll_method;
    std::optional<int> scroll_button;
    std::optional<state_t> dwt;
    std::optional<std::vector<float>> calibration_matrix;
};
JSON_TYPE_HEADER(libinput_s)

struct input_s {
    std::string identifier;
    std::string name;
    int vendor;
    int product;
    input_type_t type;
    std::optional<std::string> xkb_active_layout_name;
    std::optional<std::vector<std::string>> xkb_layout_names;
    std::optional<int> xkb_active_layout_index;
    std::optional<libinput_s> libinput;
};
JSON_TYPE_HEADER(input_s)

struct seat_s {
    std::string name;
    int capabilities;
    int focus;
    std::vector<input_s> devices;
};
JSON_TYPE_HEADER(seat_s)

enum class workspace_ev_change_t {
    INIT,
    EMPTY,
    FOCUS,
    MOVE,
    RENAME,
    URGENT,
    RELOAD
};
JSON_TYPE_HEADER(workspace_ev_change_t)

struct workspace_ev_s {
    workspace_ev_change_t change;
    std::optional<container> current;
    std::optional<container> old;
};
JSON_TYPE_HEADER(workspace_ev_s)

struct mode_ev_s {
    std::string change;
    bool pango_markup;
};
JSON_TYPE_HEADER(mode_ev_s)

enum class window_ev_change_t {
    NEW,
    CLOSE,
    FOCUS,
    TITLE,
    FULLSCREEN_MODE,
    MOVE,
    FLOATING,
    URGENT,
    MARK
};
JSON_TYPE_HEADER(window_ev_change_t)

using cont = container;

struct window_ev_s {
    window_ev_change_t change;
    cont container;
};
JSON_TYPE_HEADER(window_ev_s)

using bar_config_update_ev_s = bar_config_s;

enum class binding_ev_change_t {
    RUN
};
JSON_TYPE_HEADER(binding_ev_change_t)

enum class binding_ev_input_type_t {
    KEYBOARD,
    MOUSE,
};
JSON_TYPE_HEADER(binding_ev_input_type_t)

struct binding_ev_s {
    binding_ev_change_t change;
    std::string command;
    std::vector<std::string> event_state_mask;
    int input_code;
    std::optional<std::string> symbol;
    binding_ev_input_type_t input_type;
};
JSON_TYPE_HEADER(binding_ev_s)

enum class shutdown_ev_change_t {
    EXIT,
};
JSON_TYPE_HEADER(shutdown_ev_change_t)

struct shutdown_ev_s {
    shutdown_ev_change_t change;
};
JSON_TYPE_HEADER(shutdown_ev_s)

struct tick_ev_s {
    bool first;
    std::string payload;
};
JSON_TYPE_HEADER(tick_ev_s)

struct bar_state_update_ev_s {
    std::string id;
    bool visible_by_modifier;
};
JSON_TYPE_HEADER(bar_state_update_ev_s)

enum class input_ev_change_t {
    ADDED,
    REMOVED,
    XKB_KEYMAP,
    XKB_LAYOUT,
    LIBINPUT_CONFIG
};
JSON_TYPE_HEADER(input_ev_change_t)

struct input_ev_s {
    input_ev_change_t change;
    input_s input;
};
JSON_TYPE_HEADER(input_ev_s)

}
