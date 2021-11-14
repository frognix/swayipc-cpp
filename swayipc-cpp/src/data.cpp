#include <swayipc-cpp/data.hpp>

#include <swayipc-cpp/json.hpp>

#include "json-macro.hpp"

namespace swayipc::data {

DEFINE_TYPE_DEFAULT(command_success_s, success, error)

DEFINE_TYPE_DEFAULT(workspace_s, num, name, visible, focused, urgent, rect, output)

DEFINE_ENUM(subpixel_hinting_t, {
        {subpixel_hinting_t::RGB,  "rgb"},
        {subpixel_hinting_t::BGR,  "bgr"},
        {subpixel_hinting_t::VRGB, "vrgb"},
        {subpixel_hinting_t::VBGR, "vbgr"},
        {subpixel_hinting_t::NONE, "none"},
})

DEFINE_ENUM(transform_t, {
        {transform_t::NORMAL,      "normal"},
        {transform_t::_90,         "normal"},
        {transform_t::_180,        "normal"},
        {transform_t::_270,        "normal"},
        {transform_t::FLIPPED_90,  "normal"},
        {transform_t::FLIPPED_180, "normal"},
        {transform_t::FLIPPED_270, "normal"},
})

DEFINE_STRICT_TYPE(mode_s, width, height, refresh)

DEFINE_TYPE_DEFAULT(output_s, name, make, model, serial, active, dpms, primary, scale, subpixel_hinting,
                    transform, current_workspace, modes, current_mode, rect)

DEFINE_ENUM(bar_mode_t, {
        {bar_mode_t::DOCK, "dock"},
        {bar_mode_t::HIDE, "hide"},
        {bar_mode_t::INVISIBLE, "invisible"},
})

DEFINE_ENUM(bar_position_t, {
        {bar_position_t::BOTTOM, "bottom"},
        {bar_position_t::TOP,    "top"},
})

DEFINE_STRICT_TYPE(bar_gaps_t, top, right, bottom, left)

DEFINE_STRICT_TYPE(bar_colors_s, background, statusline,
                                   separator, focused_background,
                                   focused_statusline, focused_separator,
                                   focused_workspace_text, focused_workspace_bg,
                                   focused_workspace_border, active_workspace_text,
                                   active_workspace_bg, active_workspace_border,
                                   inactive_workspace_text, inactive_workspace_bg,
                                   inactive_workspace_border, urgent_workspace_text,
                                   urgent_workspace_bg, urgent_workspace_border,
                                   binding_mode_text, binding_mode_bg,
                                   binding_mode_border)

DEFINE_TYPE_DEFAULT(bar_config_s, id, mode, position, status_command,
                    font, workspace_buttons, binding_mode_indicator,
                    verbose, colors, gaps, bar_height, status_padding,
                    status_edge_padding)

DEFINE_STRICT_TYPE(version_s, major, minor, patch, human_readable, loaded_config_file_name)

DEFINE_ENUM(input_type_t, {
        {input_type_t::KEYBOARD, "keyboard"},
        {input_type_t::POINTER, "pointer"},
        {input_type_t::TOUCH, "touch"},
        {input_type_t::TABLET_TOOL, "tablet_tool"},
        {input_type_t::TABLET_PAD, "tablet_pad"},
        {input_type_t::SWITCH, "switch"},
})

DEFINE_ENUM(send_events_t, {
        {send_events_t::ENABLED, "enabled"},
        {send_events_t::DISABLED, "disabled"},
        {send_events_t::DISABLED_ON_EXTERNAL_MOUSE, "disabled_on_external_mouse"},
})

DEFINE_ENUM(state_t, {
        {state_t::ENABLED, "enabled"},
        {state_t::DISABLED, "disabled"},
})

DEFINE_ENUM(tab_button_map_t, {
        {tab_button_map_t::LMR, "lmr"},
        {tab_button_map_t::LRM, "lrm"},
})

DEFINE_ENUM(accel_profile_t, {
        {accel_profile_t::NONE, "none"},
        {accel_profile_t::FLAT, "flat"},
        {accel_profile_t::ADAPTIVE, "adaptive"},
})

DEFINE_ENUM(click_method_t, {
        {click_method_t::NONE, "none"},
        {click_method_t::BUTTON_AREAS, "button_areas"},
        {click_method_t::CLICKFINGER, "clickfinger"},
})

DEFINE_ENUM(scroll_method_t, {
        {scroll_method_t::NONE, "none"},
        {scroll_method_t::TWO_FINGER, "two_finger"},
        {scroll_method_t::EDGE, "edge"},
        {scroll_method_t::ON_BUTTON_DOWN, "on_button_down"},
})

DEFINE_TYPE_DEFAULT(libinput_s, send_events, tap, tap_button_map,
                    tap_drag, tap_drag_lock, accel_speed, accel_profile,
                    natural_scroll, left_handed, click_method,
                    middle_emulation, scroll_method, scroll_button,
                    dwt, calibration_matrix)

DEFINE_TYPE_DEFAULT(input_s, identifier, name, vendor, product, type,
                    xkb_active_layout_name, xkb_layout_names,
                    xkb_active_layout_index, libinput)

DEFINE_STRICT_TYPE(seat_s, name, capabilities, focus, devices)

DEFINE_ENUM(workspace_ev_change_t, {
        {workspace_ev_change_t::INIT, "init"},
        {workspace_ev_change_t::EMPTY, "empty"},
        {workspace_ev_change_t::FOCUS, "focus"},
        {workspace_ev_change_t::MOVE, "move"},
        {workspace_ev_change_t::RENAME, "rename"},
        {workspace_ev_change_t::URGENT, "urgent"},
        {workspace_ev_change_t::RELOAD, "reload"},
})

DEFINE_TYPE_DEFAULT(workspace_ev_s, change, current, old)

DEFINE_TYPE_DEFAULT(mode_ev_s, change, pango_markup)

DEFINE_ENUM(window_ev_change_t, {
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

DEFINE_TYPE_DEFAULT(window_ev_s, change, container)

DEFINE_ENUM(binding_ev_change_t, {
        {binding_ev_change_t::RUN, "run"},
})

DEFINE_ENUM(binding_ev_input_type_t, {
        {binding_ev_input_type_t::KEYBOARD, "keyboard"},
        {binding_ev_input_type_t::MOUSE, "mouse"},
})

DEFINE_TYPE_DEFAULT(binding_ev_s, change, command, event_state_mask,
                    input_code, symbol, input_code)

DEFINE_ENUM(shutdown_ev_change_t, {
        {shutdown_ev_change_t::EXIT, "exit"},
})

DEFINE_TYPE_DEFAULT(shutdown_ev_s, change)

DEFINE_TYPE_DEFAULT(tick_ev_s, first, payload)

DEFINE_TYPE_DEFAULT(bar_state_update_ev_s, id, visible_by_modifier)

DEFINE_ENUM(input_ev_change_t, {
        {input_ev_change_t::ADDED, "added"},
        {input_ev_change_t::REMOVED, "removed"},
        {input_ev_change_t::XKB_KEYMAP, "xkb_keymap"},
        {input_ev_change_t::XKB_LAYOUT, "xkb_layout"},
        {input_ev_change_t::LIBINPUT_CONFIG, "libinput_config"},
})

DEFINE_TYPE_DEFAULT(input_ev_s, change, input)

} // namespace swayipc::data
