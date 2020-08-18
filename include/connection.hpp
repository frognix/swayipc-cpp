#pragma once

#include <functional>
#include <type_traits>

#include "socket_wrapper.hpp"
#include "serialization.hpp"
#include "message.hpp"

namespace swayipc {

class sway_socket;

class connection {
public:
    connection(sway_socket*);
    ~connection();
    message_s raw_request(message_type type, std::string payload = "");

    std::vector<data::command_success_s> run_command(std::string command);
    std::vector<data::workspace_s> get_workspaces();
    bool subscribe(data::event_type_t event);
    std::vector<data::output_s> get_outputs();
    data::node_s get_tree();
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
