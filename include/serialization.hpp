#pragma once

#include "json.hpp"
#include "container.hpp"

namespace swayipc::data {

//Send types
struct command_success_s {
    bool success;
    opt<std::string> error;
};
DEFINE_TYPE_DEFAULT(command_success_s, success, error);

struct workspace_s {
    int num;
    std::string name;
    bool visible;
    bool focused;
    bool urgent;
    rect_s rect;
    std::string output;
};
DEFINE_TYPE_DEFAULT(workspace_s, num, name, visible, focused, urgent, rect, output);

enum class subpixel_hinting_t {
    RGB,
    BGR,
    VRGB,
    VBGR,
    NONE,
};
NLOHMANN_JSON_SERIALIZE_ENUM(subpixel_hinting_t, {
        {subpixel_hinting_t::RGB,  "rgb"},
        {subpixel_hinting_t::BGR,  "bgr"},
        {subpixel_hinting_t::VRGB, "vrgb"},
        {subpixel_hinting_t::VBGR, "vbgr"},
        {subpixel_hinting_t::NONE, "none"},
})

enum class transform_t {
    NORMAL,
    _90,
    _180,
    _270,
    FLIPPED_90,
    FLIPPED_180,
    FLIPPED_270
};
NLOHMANN_JSON_SERIALIZE_ENUM(transform_t, {
        {transform_t::NORMAL,      "normal"},
        {transform_t::_90,         "normal"},
        {transform_t::_180,        "normal"},
        {transform_t::_270,        "normal"},
        {transform_t::FLIPPED_90,  "normal"},
        {transform_t::FLIPPED_180, "normal"},
        {transform_t::FLIPPED_270, "normal"},
})

struct mode_s {
    int width;
    int height;
    int refresh;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(mode_s, width, height, refresh);

struct output_s {
    opt<std::string> name;
    opt<std::string> make;
    opt<std::string> model;
    opt<std::string> serial;
    opt<bool> active;
    opt<bool> dpms;
    opt<bool> primary;
    opt<float> scale;
    opt<subpixel_hinting_t> subpixel_hinting;
    opt<transform_t> transform;
    opt<std::string> current_workspace;
    opt<std::vector<mode_s>> modes;
    opt<mode_s> current_mode;
    opt<rect_s> rect;
};
DEFINE_TYPE_DEFAULT(output_s, name, make, model, serial, active, dpms, primary, scale, subpixel_hinting,
                    transform, current_workspace, modes, current_mode, rect);

enum class bar_mode_t {
    DOCK,
    HIDE,
    INVISIBLE
};
NLOHMANN_JSON_SERIALIZE_ENUM(bar_mode_t, {
        {bar_mode_t::DOCK, "dock"},
        {bar_mode_t::HIDE, "hide"},
        {bar_mode_t::INVISIBLE, "invisible"},
})

enum class bar_position_t {
    BOTTOM,
    TOP
};
NLOHMANN_JSON_SERIALIZE_ENUM(bar_position_t, {
        {bar_position_t::BOTTOM, "bottom"},
        {bar_position_t::TOP,    "top"},
})

struct bar_gaps_t {
    int top;
    int right;
    int bottom;
    int left;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(bar_gaps_t, top, right, bottom, left);

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
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(bar_colors_s, background, statusline,
                                   separator, focused_background,
                                   focused_statusline, focused_separator,
                                   focused_workspace_text, focused_workspace_bg,
                                   focused_workspace_border, active_workspace_text,
                                   active_workspace_bg, active_workspace_border,
                                   inactive_workspace_text, inactive_workspace_bg,
                                   inactive_workspace_border, urgent_workspace_text,
                                   urgent_workspace_bg, urgent_workspace_border,
                                   binding_mode_text, binding_mode_bg,
                                   binding_mode_border);

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
DEFINE_TYPE_DEFAULT(bar_config_s, id, mode, position, status_command,
                    font, workspace_buttons, binding_mode_indicator,
                    verbose, colors, gaps, bar_height, status_padding,
                    status_edge_padding);

struct version_s {
    int major;
    int minor;
    int patch;
    std::string human_readable;
    std::string loaded_config_file_name;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(version_s, major, minor, patch, human_readable, loaded_config_file_name);

enum class input_type_t {
    KEYBOARD,
    POINTER,
    TOUCH,
    TABLET_TOOL,
    TABLET_PAD,
    SWITCH
};
NLOHMANN_JSON_SERIALIZE_ENUM(input_type_t, {
        {input_type_t::KEYBOARD, "keyboard"},
        {input_type_t::POINTER, "pointer"},
        {input_type_t::TOUCH, "touch"},
        {input_type_t::TABLET_TOOL, "tablet_tool"},
        {input_type_t::TABLET_PAD, "tablet_pad"},
        {input_type_t::SWITCH, "switch"},
})

enum class send_events_t {
    DISABLED,
    ENABLED,
    DISABLED_ON_EXTERNAL_MOUSE
};
NLOHMANN_JSON_SERIALIZE_ENUM(send_events_t, {
        {send_events_t::ENABLED, "enabled"},
        {send_events_t::DISABLED, "disabled"},
        {send_events_t::DISABLED_ON_EXTERNAL_MOUSE, "disabled_on_external_mouse"},
})

enum class state_t {
    DISABLED,
    ENABLED
};
NLOHMANN_JSON_SERIALIZE_ENUM(state_t, {
        {state_t::ENABLED, "enabled"},
        {state_t::DISABLED, "disabled"},
})

enum class tab_button_map_t {
    LMR,
    LRM
};
NLOHMANN_JSON_SERIALIZE_ENUM(tab_button_map_t, {
        {tab_button_map_t::LMR, "lmr"},
        {tab_button_map_t::LRM, "lrm"},
})

enum class accel_profile_t {
    NONE,
    FLAT,
    ADAPTIVE
};
NLOHMANN_JSON_SERIALIZE_ENUM(accel_profile_t, {
        {accel_profile_t::NONE, "none"},
        {accel_profile_t::FLAT, "flat"},
        {accel_profile_t::ADAPTIVE, "adaptive"},
})

enum class click_method_t {
    NONE,
    BUTTON_AREAS,
    CLICKFINGER
};
NLOHMANN_JSON_SERIALIZE_ENUM(click_method_t, {
        {click_method_t::NONE, "none"},
        {click_method_t::BUTTON_AREAS, "button_areas"},
        {click_method_t::CLICKFINGER, "clickfinger"},
})

enum class scroll_method_t {
    NONE,
    TWO_FINGER,
    EDGE,
    ON_BUTTON_DOWN
};
NLOHMANN_JSON_SERIALIZE_ENUM(scroll_method_t, {
        {scroll_method_t::NONE, "none"},
        {scroll_method_t::TWO_FINGER, "two_finger"},
        {scroll_method_t::EDGE, "edge"},
        {scroll_method_t::ON_BUTTON_DOWN, "on_button_down"},
})

struct libinput_s {
    opt<send_events_t> send_events;
    opt<state_t> tap;
    opt<tab_button_map_t> tap_button_map;
    opt<state_t> tap_drag;
    opt<state_t> tap_drag_lock;
    opt<double> accel_speed;
    opt<accel_profile_t> accel_profile;
    opt<state_t> natural_scroll;
    opt<state_t> left_handed;
    opt<click_method_t> click_method;
    opt<state_t> middle_emulation;
    opt<scroll_method_t> scroll_method;
    opt<int> scroll_button;
    opt<state_t> dwt;
    opt<std::vector<float>> calibration_matrix;
};
DEFINE_TYPE_DEFAULT(libinput_s, send_events, tap, tap_button_map,
                    tap_drag, tap_drag_lock, accel_speed, accel_profile,
                    natural_scroll, left_handed, click_method,
                    middle_emulation, scroll_method, scroll_button,
                    dwt, calibration_matrix);

struct input_s {
    std::string identifier;
    std::string name;
    int vendor;
    int product;
    input_type_t type;
    opt<std::string> xkb_active_layout_name;
    opt<std::vector<std::string>> xkb_layout_names;
    opt<int> xkb_active_layout_index;
    opt<libinput_s> libinput;
};
DEFINE_TYPE_DEFAULT(input_s, identifier, name, vendor, product, type,
                    xkb_active_layout_name, xkb_layout_names,
                    xkb_active_layout_index, libinput);

struct seat_s {
    std::string name;
    int capabilities;
    int focus;
    std::vector<input_s> devices;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(seat_s, name, capabilities, focus, devices);

enum class workspace_ev_change_t {
    INIT,
    EMPTY,
    FOCUS,
    MOVE,
    RENAME,
    URGENT,
    RELOAD
};
NLOHMANN_JSON_SERIALIZE_ENUM(workspace_ev_change_t, {
        {workspace_ev_change_t::INIT, "init"},
        {workspace_ev_change_t::EMPTY, "empty"},
        {workspace_ev_change_t::FOCUS, "focus"},
        {workspace_ev_change_t::MOVE, "move"},
        {workspace_ev_change_t::RENAME, "rename"},
        {workspace_ev_change_t::URGENT, "urgent"},
        {workspace_ev_change_t::RELOAD, "reload"},
})

struct workspace_ev_s {
    workspace_ev_change_t change;
    opt<container> current;
    opt<container> old;
};
DEFINE_TYPE_DEFAULT(workspace_ev_s, change, current, old)

struct mode_ev_s {
    std::string change;
    bool pango_markup;
};
DEFINE_TYPE_DEFAULT(mode_ev_s, change, pango_markup)

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
NLOHMANN_JSON_SERIALIZE_ENUM(window_ev_change_t, {
        {window_ev_change_t::NEW, "new"},
        {window_ev_change_t::CLOSE, "close"},
        {window_ev_change_t::FOCUS, "focus"},
        {window_ev_change_t::TITLE, "title"},
        {window_ev_change_t::FULLSCREEN_MODE, "fullscreen_mode"},
        {window_ev_change_t::MOVE, "move"},
        {window_ev_change_t::FLOATING, "floating"},
        {window_ev_change_t::URGENT, "urgent"},
        {window_ev_change_t::MARK, "mark"},
})

using cont = container;

struct window_ev_s {
    window_ev_change_t change;
    cont container;
};
DEFINE_TYPE_DEFAULT(window_ev_s, change, container)

using bar_config_update_ev_s = bar_config_s;

enum class binding_ev_change_t {
    RUN
};
NLOHMANN_JSON_SERIALIZE_ENUM(binding_ev_change_t, {
        {binding_ev_change_t::RUN, "run"},
})

enum class binding_ev_input_type_t {
    KEYBOARD,
    MOUSE,
};
NLOHMANN_JSON_SERIALIZE_ENUM(binding_ev_input_type_t, {
        {binding_ev_input_type_t::KEYBOARD, "keyboard"},
        {binding_ev_input_type_t::MOUSE, "mouse"},
})


struct binding_ev_s {
    binding_ev_change_t change;
    std::string command;
    std::vector<std::string> event_state_mask;
    int input_code;
    opt<std::string> symbol;
    binding_ev_input_type_t input_type;
};
DEFINE_TYPE_DEFAULT(binding_ev_s, change, command, event_state_mask,
                    input_code, symbol, input_code)

enum class shutdown_ev_change_t {
    EXIT,
};
NLOHMANN_JSON_SERIALIZE_ENUM(shutdown_ev_change_t, {
        {shutdown_ev_change_t::EXIT, "exit"},
})

struct shutdown_ev_s {
    shutdown_ev_change_t change;
};
DEFINE_TYPE_DEFAULT(shutdown_ev_s, change)

struct tick_ev_s {
    bool first;
    std::string payload;
};
DEFINE_TYPE_DEFAULT(tick_ev_s, first, payload)


struct bar_state_update_ev_s {
    std::string id;
    bool visible_by_modifier;
};
DEFINE_TYPE_DEFAULT(bar_state_update_ev_s, id, visible_by_modifier)

enum class input_ev_change_t {
    ADDED,
    REMOVED,
    XKB_KEYMAP,
    XKB_LAYOUT,
    LIBINPUT_CONFIG
};
NLOHMANN_JSON_SERIALIZE_ENUM(input_ev_change_t, {
        {input_ev_change_t::ADDED, "added"},
        {input_ev_change_t::REMOVED, "removed"},
        {input_ev_change_t::XKB_KEYMAP, "xkb_keymap"},
        {input_ev_change_t::XKB_LAYOUT, "xkb_layout"},
        {input_ev_change_t::LIBINPUT_CONFIG, "libinput_config"},
})

struct input_ev_s {
    input_ev_change_t change;
    input_s input;
};
DEFINE_TYPE_DEFAULT(input_ev_s, change, input)

}
