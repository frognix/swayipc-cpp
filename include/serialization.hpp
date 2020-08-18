#pragma once

#include <nlohmann/json.hpp>

namespace nlohmann {
template <typename T>
struct adl_serializer<std::optional<T>> {
  static void to_json(json& j, const std::optional<T>& opt) {
     if (opt == std::nullopt) j = nullptr;
     else j = *opt;
  }
  static void from_json(const json& j, std::optional<T>& opt) {
    if (j.is_null()) opt = std::nullopt;
    else opt = j.get<T>();
  }
};
}

namespace swayipc::data {

using json = nlohmann::json;

template <class T>
inline void get_or(const json& j, const char* name, T& val) {
    if (j.contains(name) && !j[name].is_null()) val = j[name].get<T>();
}

template <class T>
inline void set_or(json& j, const char* name, const T& val) {
    auto jval = json(val);
    if (!jval.is_null()) {
        j[name] = jval;
    } else {
        j.erase(name);
    }
}

template <class T>
using opt = std::optional<T>;

#define DEFAULT_FROM(v1) get_or(nlohmann_json_j, (#v1), nlohmann_json_t.v1);

#define DEFAULT_TO(v1) set_or(nlohmann_json_j, (#v1), nlohmann_json_t.v1);

#define DEFINE_TYPE(Type, To, From, ...) \
       inline void to_json(nlohmann::json& nlohmann_json_j, const Type& nlohmann_json_t) { NLOHMANN_JSON_EXPAND(NLOHMANN_JSON_PASTE(To, __VA_ARGS__)) } \
       inline void from_json(const nlohmann::json& nlohmann_json_j, Type& nlohmann_json_t) { NLOHMANN_JSON_EXPAND(NLOHMANN_JSON_PASTE(From, __VA_ARGS__)) }

#define DEFINE_TYPE_DEFAULT(Type, ...)  DEFINE_TYPE(Type, DEFAULT_TO, DEFAULT_FROM, __VA_ARGS__)

#define DEFINE_STRICT_TYPE(Type, ...) DEFINE_TYPE(Type, NLOHMANN_JSON_TO, NLOHMANN_JSON_FROM, __VA_ARGS__)

//Send types
struct command_success_s {
    bool success;
    opt<std::string> error;
};
DEFINE_TYPE_DEFAULT(command_success_s, success, error);

struct rect_s {
    int x;
    int y;
    int width;
    int height;
};
DEFINE_STRICT_TYPE(rect_s, x, y, width, height);

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

enum class event_type_t : int {
    WORKSPACE        = 0b10000000000000,
    MODE             = 0b01000000000000,
    WINDOW           = 0b00100000000000,
    BARCONFIG_UPDATE = 0b00010000000000,
    BINDING          = 0b00001000000000,
    SHUTDOWN         = 0b00000100000000,
    TICK             = 0b00000010000000,
    BAR_STATE_UPDATE = 0b00000001000000,
    INPUT            = 0b00000000100000
};
NLOHMANN_JSON_SERIALIZE_ENUM(event_type_t, {
        {event_type_t::WORKSPACE,        "workspace"},
        {event_type_t::MODE,             "mode"},
        {event_type_t::WINDOW,           "window"},
        {event_type_t::BARCONFIG_UPDATE, "barconfig_update"},
        {event_type_t::BINDING,          "binding"},
        {event_type_t::SHUTDOWN,         "shutdown"},
        {event_type_t::TICK,             "tick"},
        {event_type_t::BAR_STATE_UPDATE, "bar_state_update"},
        {event_type_t::INPUT,            "input"},
})

inline event_type_t operator | (event_type_t a, event_type_t b) {
    return static_cast<event_type_t>(static_cast<int>(a) | static_cast<int>(b));
}

inline bool operator & (event_type_t a, event_type_t b) {
    return static_cast<int>(a) & static_cast<int>(b);
}

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

enum class node_type_t {
    ROOT,
    OUTPUT,
    WORKSPACE,
    CON,
    FLOATING_CON
};
NLOHMANN_JSON_SERIALIZE_ENUM(node_type_t, {
        {node_type_t::ROOT,         "root"},
        {node_type_t::OUTPUT,       "output"},
        {node_type_t::WORKSPACE,    "workspace"},
        {node_type_t::CON,          "con"},
        {node_type_t::FLOATING_CON, "floating_con"}
})

enum class border_t {
    NORMAL,
    NONE,
    PIXEL,
    CSD
};
NLOHMANN_JSON_SERIALIZE_ENUM(border_t, {
        {border_t::NORMAL, "normal"},
        {border_t::NONE,   "none"},
        {border_t::PIXEL,  "pixel"},
        {border_t::CSD,    "csd"}
})

enum class layout_t {
    SPLITH,
    SPLITV,
    STACKED,
    TABBED,
    OUTPUT
};
NLOHMANN_JSON_SERIALIZE_ENUM(layout_t, {
        {layout_t::SPLITH, "splith"},
        {layout_t::SPLITV, "splitv"},
        {layout_t::STACKED, "stacked"},
        {layout_t::TABBED, "tabbed"},
        {layout_t::OUTPUT, "output"},
})

enum class orientation_t {
    VERTICAL,
    HORIZONTAL,
    NONE
};
NLOHMANN_JSON_SERIALIZE_ENUM(orientation_t, {
        {orientation_t::VERTICAL,   "vertical"},
        {orientation_t::HORIZONTAL, "horizontal"},
        {orientation_t::NONE,       "none"},
})

enum class fullscreen_mode_t : int {
    NONE = 0,
    FULL = 1,
    GLOBAL_FULL = 2
};

enum class application_idle_t {
    ENABLED,
    NONE
};
NLOHMANN_JSON_SERIALIZE_ENUM(application_idle_t, {
        {application_idle_t::ENABLED, "enabled"},
        {application_idle_t::NONE,    "none"},
})

enum class user_idle_t {
    FOCUS,
    FULLSCREEN,
    OPEN,
    VISIBLE,
    NONE
};
NLOHMANN_JSON_SERIALIZE_ENUM(user_idle_t, {
        {user_idle_t::FOCUS,      "focus"},
        {user_idle_t::FULLSCREEN, "fullscreen"},
        {user_idle_t::OPEN,       "open"},
        {user_idle_t::VISIBLE,    "visible"},
        {user_idle_t::NONE,       "none"},
})

struct idle_inhibitors_s {
    application_idle_t application;
    user_idle_t user;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(idle_inhibitors_s, application, user);

struct window_properties_s {
    std::string title;
    std::string class_;
    std::string instance;
    std::string window_role;
    std::string window_type;
    std::string transient_for;
};

inline void to_json(json& j, const window_properties_s& w) {
    j = json{{"title", w.title}, {"class", w.class_}, {"instance", w.instance},
             {"window_role", w.window_role}, {"window_type", w.window_type},
             {"transient_for", w.transient_for}};
}

inline void from_json(const json& j, window_properties_s& w) {
    get_or(j, "title", w.title);
    get_or(j, "class", w.class_);
    get_or(j, "instance", w.instance);
    get_or(j, "window_role", w.window_role);
    get_or(j, "window_type", w.window_type);
    get_or(j, "transient_for", w.transient_for);
}

struct node_s {
    int id;
    std::string name;
    node_type_t type;
    border_t border;
    int current_border_width;
    layout_t layout;
    orientation_t orientation;
    float percent;
    rect_s rect;
    rect_s window_rect;
    rect_s deco_rect;
    rect_s geometry;
    bool urgent;
    bool sticky;
    std::vector<std::string> marks;
    bool focused;
    std::vector<int> focus;
    std::vector<node_s> nodes;
    std::vector<node_s> floating_nodes;
    opt<std::string> representation;
    opt<fullscreen_mode_t> fullscreen_mode;
    opt<std::string> app_id;
    opt<int> pid;
    opt<bool> visible;
    opt<std::string> shell;
    opt<bool> inhibit_idle;
    opt<idle_inhibitors_s> idle_inhibitors;
    opt<int> window;
    opt<window_properties_s> window_properties;
};
DEFINE_TYPE_DEFAULT(node_s, id, name, type, border, current_border_width,
                    layout, orientation, percent, rect, window_rect,
                    deco_rect, geometry, urgent, sticky, marks,
                    focused, focus, nodes, floating_nodes, representation,
                    fullscreen_mode, app_id, pid, visible, shell,
                    inhibit_idle, idle_inhibitors, window, window_properties);

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
    opt<node_s> current;
    opt<node_s> old;
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

struct window_ev_s {
    window_ev_change_t change;
    node_s container;
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
