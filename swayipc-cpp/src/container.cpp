#include <swayipc-cpp/container.hpp>

#include "json-macro.hpp"

#include <unordered_set>

namespace swayipc::data {

container* container::find(std::function<bool(const container&)> pred) {
    std::function<container*(container&)> inner_find;
    inner_find = [&inner_find, pred](container& cont) -> container* {
        if (pred(cont)) return &cont;
        for (auto& subnode : cont.nodes) {
            if (auto res = inner_find(subnode); res)
                return res;
        }
        return nullptr;
    };
    return inner_find(*this);
}

std::vector<container*> container::find_many(std::function<bool(const container&)> pred) {
    std::vector<container*> res;
    std::function<void(container&)> inner_find;
    inner_find = [&res, &inner_find, pred](container& cont) mutable {
        if (pred(cont)) res.push_back(&cont);
        for (auto& subnode : cont.nodes) {
            inner_find(subnode);
        }
    };
    inner_find(*this);
    return res;
}

DEFINE_STRICT_TYPE(rect_s, x, y, width, height)

DEFINE_ENUM(node_type_t, {
        {node_type_t::ROOT,         "root"},
        {node_type_t::OUTPUT,       "output"},
        {node_type_t::WORKSPACE,    "workspace"},
        {node_type_t::CON,          "con"},
        {node_type_t::FLOATING_CON, "floating_con"}
})

DEFINE_ENUM(border_t, {
        {border_t::NORMAL, "normal"},
        {border_t::NONE,   "none"},
        {border_t::PIXEL,  "pixel"},
        {border_t::CSD,    "csd"}
})

DEFINE_ENUM(layout_t, {
        {layout_t::SPLITH, "splith"},
        {layout_t::SPLITV, "splitv"},
        {layout_t::STACKED, "stacked"},
        {layout_t::TABBED, "tabbed"},
        {layout_t::OUTPUT, "output"},
})

DEFINE_ENUM(orientation_t, {
        {orientation_t::VERTICAL,   "vertical"},
        {orientation_t::HORIZONTAL, "horizontal"},
        {orientation_t::NONE,       "none"},
})

DEFINE_ENUM(application_idle_t, {
        {application_idle_t::ENABLED, "enabled"},
        {application_idle_t::NONE,    "none"},
})

DEFINE_ENUM(user_idle_t, {
        {user_idle_t::FOCUS,      "focus"},
        {user_idle_t::FULLSCREEN, "fullscreen"},
        {user_idle_t::OPEN,       "open"},
        {user_idle_t::VISIBLE,    "visible"},
        {user_idle_t::NONE,       "none"},
})

DEFINE_STRICT_TYPE(idle_inhibitors_s, application, user)

void to_json(nlohmann::json& j, const window_properties_s& w) {
    j = nlohmann::json{{"title", w.title}, {"class", w.class_}, {"instance", w.instance},
                       {"window_role", w.window_role}, {"window_type", w.window_type},
                       {"transient_for", w.transient_for}};
}

void from_json(const nlohmann::json& j, window_properties_s& w) {
    get_or(j, "title", w.title);
    get_or(j, "class", w.class_);
    get_or(j, "instance", w.instance);
    get_or(j, "window_role", w.window_role);
    get_or(j, "window_type", w.window_type);
    get_or(j, "transient_for", w.transient_for);
}

DEFINE_TYPE_DEFAULT(container, id, name, type, border, current_border_width,
                    layout, orientation, percent, rect, window_rect,
                    deco_rect, geometry, urgent, sticky, marks,
                    focused, focus, nodes, floating_nodes, representation,
                    fullscreen_mode, app_id, pid, visible, shell,
                    inhibit_idle, idle_inhibitors, window, window_properties,
                    current_workspace)

} // namespace swayipc::data
