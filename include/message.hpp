#pragma once

enum message_type : uint32_t {
    RUN_COMMAND = 0,
    GET_WORKSPACES = 1,
    SUBSCRIBE = 2,
    GET_OUTPUTS = 3,
    GET_TREE = 4,
    GET_MARKS = 5,
    GET_BAR_CONFIG = 6,
    GET_VERSION = 7,
    GET_BINDING_MODES = 8,
    GET_CONFIG = 9,
    SEND_TICK = 10,
    SYNC = 11,
    GET_BINDING_STATE = 12,
    GET_INPUTS = 100,
    GET_SEATS = 101,
    WORKSPACE_EVENT = 0x80000000,
    MODE_EVENT = 0x80000002,
    WINDOW_EVENT = 0x80000003,
    BAR_CONFIG_UPDATE_EVENT = 0x80000004,
    BINDING_EVENT = 0x80000005,
    SHUTDOWN_EVENT = 0x80000006,
    TICK_EVENT = 0x80000007,
    BAR_STATE_UPDATE_EVENT = 0x80000014,
    INPUT_EVENT = 0x80000015,
    ANY = 0xFFFFFFFF
};

struct __attribute__ ((packed)) message_header {
    char magic[6] = {'i', '3', '-', 'i', 'p', 'c'};
    int32_t length;
    message_type type;
};

struct message_s {
    message_header header;
    std::string payload;
};

template <class T>
struct always_false {
    enum { value = false };
};

template <class T>
constexpr message_type get_event_tag() {
    if constexpr (std::is_same_v<T, workspace_ev_s>) {
        return WORKSPACE_EVENT;
    } else if constexpr (std::is_same_v<T, mode_ev_s>) {
        return MODE_EVENT;
    } else if constexpr (std::is_same_v<T, window_ev_s>) {
        return WINDOW_EVENT;
    } else if constexpr (std::is_same_v<T, bar_config_update_ev_s>) {
        return BAR_CONFIG_UPDATE_EVENT;
    } else if constexpr (std::is_same_v<T, binding_ev_s>) {
        return BINDING_EVENT;
    } else if constexpr (std::is_same_v<T, shutdown_ev_s>) {
        return SHUTDOWN_EVENT;
    } else if constexpr (std::is_same_v<T, tick_ev_s>) {
        return TICK_EVENT;
    } else if constexpr (std::is_same_v<T, bar_state_update_ev_s>) {
        return BAR_STATE_UPDATE_EVENT;
    } else if constexpr (std::is_same_v<T, input_ev_s>) {
        return INPUT_EVENT;
    } else {
        static_assert(always_false<T>::value, "Bad event type");
    }
}

inline bool is_event(const message_s& msg) {
    auto type = msg.header.type;
    return type == message_type::WORKSPACE_EVENT ||
        type == message_type::MODE_EVENT ||
        type == message_type::WINDOW_EVENT ||
        type == message_type::BAR_CONFIG_UPDATE_EVENT ||
        type == message_type::BINDING_EVENT ||
        type == message_type::SHUTDOWN_EVENT ||
        type == message_type::TICK_EVENT ||
        type == message_type::BAR_STATE_UPDATE_EVENT ||
        type == message_type::INPUT_EVENT;
}
