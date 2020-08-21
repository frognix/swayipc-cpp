#pragma once

#include "json.hpp"

namespace swayipc::data {

using json = nlohmann::json;

enum class node_type_t {
    ROOT,
    OUTPUT,
    WORKSPACE,
    CON,
    FLOATING_CON
};
DEFINE_ENUM(node_type_t, {
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
DEFINE_ENUM(border_t, {
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
DEFINE_ENUM(layout_t, {
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
DEFINE_ENUM(orientation_t, {
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
DEFINE_ENUM(application_idle_t, {
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
DEFINE_ENUM(user_idle_t, {
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
DEFINE_STRICT_TYPE(idle_inhibitors_s, application, user)

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

struct container {
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
    std::vector<container> nodes;
    std::vector<container> floating_nodes;
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

    container* find(std::function<bool(const container&)>);
    std::vector<container*> find_many(std::function<bool(const container&)>);
};

DEFINE_TYPE_DEFAULT(container, id, name, type, border, current_border_width,
                    layout, orientation, percent, rect, window_rect,
                    deco_rect, geometry, urgent, sticky, marks,
                    focused, focus, nodes, floating_nodes, representation,
                    fullscreen_mode, app_id, pid, visible, shell,
                    inhibit_idle, idle_inhibitors, window, window_properties)

}
