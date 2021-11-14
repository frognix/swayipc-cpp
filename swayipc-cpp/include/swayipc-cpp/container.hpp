#pragma once

#include <swayipc-cpp/json.hpp>

namespace swayipc::data {

struct rect_s {
    int x;
    int y;
    int width;
    int height;
};
JSON_TYPE_HEADER(rect_s)

enum class node_type_t {
    ROOT,
    OUTPUT,
    WORKSPACE,
    CON,
    FLOATING_CON
};
JSON_TYPE_HEADER(node_type_t)

enum class border_t {
    NORMAL,
    NONE,
    PIXEL,
    CSD
};
JSON_TYPE_HEADER(border_t)

enum class layout_t {
    SPLITH,
    SPLITV,
    STACKED,
    TABBED,
    OUTPUT
};
JSON_TYPE_HEADER(layout_t)

enum class orientation_t {
    VERTICAL,
    HORIZONTAL,
    NONE
};
JSON_TYPE_HEADER(orientation_t)

enum class fullscreen_mode_t : int {
    NONE = 0,
    FULL = 1,
    GLOBAL_FULL = 2
};

enum class application_idle_t {
    ENABLED,
    NONE
};
JSON_TYPE_HEADER(application_idle_t)

enum class user_idle_t {
    FOCUS,
    FULLSCREEN,
    OPEN,
    VISIBLE,
    NONE
};
JSON_TYPE_HEADER(user_idle_t)

struct idle_inhibitors_s {
    application_idle_t application;
    user_idle_t user;
};
JSON_TYPE_HEADER(idle_inhibitors_s)

struct window_properties_s {
    std::string title;
    std::string class_;
    std::string instance;
    std::string window_role;
    std::string window_type;
    std::string transient_for;
};
JSON_TYPE_HEADER(window_properties_s)

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
    std::optional<std::string> representation;
    std::optional<fullscreen_mode_t> fullscreen_mode;
    std::optional<std::string> app_id;
    std::optional<int> pid;
    std::optional<bool> visible;
    std::optional<std::string> shell;
    std::optional<bool> inhibit_idle;
    std::optional<idle_inhibitors_s> idle_inhibitors;
    std::optional<int> window;
    std::optional<window_properties_s> window_properties;

    container* find(std::function<bool(const container&)>);
    std::vector<container*> find_many(std::function<bool(const container&)>);
};
JSON_TYPE_HEADER(container)

}
