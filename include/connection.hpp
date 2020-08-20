#pragma once

#include <functional>
#include <type_traits>

#include "socket_wrapper.hpp"
#include "serialization.hpp"
#include "assert.hpp"
#include "message.hpp"

namespace swayipc {

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

class sway_socket;

class connection {
public:
    connection(sway_socket*);
    ~connection();
    message_s raw_request(message_type type, std::string payload = "");

    std::vector<data::command_success_s> command(std::string command);
    std::vector<data::command_success_s> command_on(data::container* cont, std::string command);
    std::vector<data::command_success_s> command_on(const std::vector<data::container*>& conts, std::string command);
    std::vector<data::workspace_s> get_workspaces();
    bool subscribe(event_type_t event);
    std::vector<data::output_s> get_outputs();
    data::container get_tree();
    std::vector<std::string> get_marks();
    std::vector<std::string> get_bar_config();
    data::bar_config_s get_bar_config(std::string bar);
    data::version_s get_version();
    std::vector<std::string> get_binding_modes();
    std::string get_config();
    bool send_tick(std::string payload = "");
    // bool sync();
    std::string get_bining_state();
    std::vector<data::input_s> get_inputs();
    std::vector<data::seat_s> get_seats();

private:
    sway_socket* m_socket;
};

}
