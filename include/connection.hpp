#pragma once

#include "socket_wrapper.hpp"
#include "serialization.hpp"
#include <functional>

#include <sys/un.h>

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
};

struct __attribute__ ((packed)) message_header {
    char magic[6] = {'i', '3', '-', 'i', 'p', 'c'};
    int32_t length;
    message_type type;
};

struct message {
    message_header header;
    json payload;
};

class connection {
public:
    connection();
    ~connection();
    bool connect(std::string socket_path);
    bool connect();
    void close();
    bool handle_events();
    message& raw_request(message_type type, std::string payload = "");

    std::vector<command_success_s> run_command(std::string command);
    std::vector<workspace_s> get_workspaces();
    bool subscribe(event_type_t event);
    std::vector<output_s> get_outputs();
    node_s get_tree();
    std::vector<std::string> get_marks();
    std::vector<std::string> get_bar_config();
    bar_config_s get_bar_config(std::string bar);
    version_s get_version();
    std::vector<std::string> get_binding_modes();
    std::string get_config();
    bool send_tick(std::string payload = "");
    // bool sync();
    std::string get_bining_state();
    std::vector<input_s> get_inputs();
    std::vector<seat_s> get_seats();


    void set_workspace_event_handler(std::function<void(const workspace_ev_s&)>);
    void set_mode_event_handler(std::function<void(const mode_ev_s&)>);
    void set_window_event_handler(std::function<void(const window_ev_s&)>);
    void set_bar_config_update_event_handler(std::function<void(const bar_config_update_ev_s&)>);
    void set_binding_event_handler(std::function<void(const binding_ev_s&)>);
    void set_shutdown_event_handler(std::function<void(const shutdown_ev_s&)>);
    void set_tick_event_handler(std::function<void(const tick_ev_s&)>);
    void set_bar_state_update_event_handler(std::function<void(const bar_state_update_ev_s&)>);
    void set_input_event_handler(std::function<void(const input_ev_s&)>);
private:
    message& raw_read();
    void raw_send(message_type type, std::string payload = "");

    message m_buffer;
    std::string m_payload_buffer;
    socket_wrapper m_socket;

    std::function<void(const workspace_ev_s&)> m_workspace_ev_handler;
    std::function<void(const mode_ev_s&)>      m_mode_ev_handler;
    std::function<void(const window_ev_s&)>    m_window_ev_handler;
    std::function<void(const bar_config_update_ev_s&)> m_bar_config_update_ev_handler;
    std::function<void(const binding_ev_s&)>   m_binding_ev_handler;
    std::function<void(const shutdown_ev_s&)>  m_shutdown_ev_handler;
    std::function<void(const tick_ev_s&)>      m_tick_ev_handler;
    std::function<void(const bar_state_update_ev_s&)> m_bar_state_update_ev_handler;
    std::function<void(const input_ev_s&)>     m_input_ev_handler;
};
