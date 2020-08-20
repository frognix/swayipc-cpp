#include "connection.hpp"
#include "sway_socket.hpp"

using namespace swayipc::data;

namespace swayipc {

connection::connection(sway_socket* socket)
    : m_socket(socket) {}

connection::~connection() {}

message_s connection::raw_request(message_type type, std::string payload) {
    return m_socket->request(type, payload);
}

std::vector<command_success_s> connection::command(std::string command) {
    auto message = raw_request(RUN_COMMAND, command);
    return json::parse(message.payload).get<std::vector<command_success_s>>();
}

std::vector<data::command_success_s> connection::command_on(data::container* cont, std::string command) {
    auto message = raw_request(RUN_COMMAND, "[con_id=" + std::to_string(cont->id) + "] " + command);
    return json::parse(message.payload).get<std::vector<command_success_s>>();
}

std::vector<data::command_success_s> connection::command_on(const std::vector<data::container*>& conts, std::string command) {
    std::vector<data::command_success_s> res;
    for (auto cont : conts) {
        auto message = raw_request(RUN_COMMAND, "[con_id=" + std::to_string(cont->id) + "] " + command);
        auto vec = json::parse(message.payload).get<std::vector<command_success_s>>();
        res.insert(res.end(), vec.begin(), vec.end());
    }
    return res;
}

std::vector<workspace_s> connection::get_workspaces() {
    auto message = raw_request(GET_WORKSPACES);
    return json::parse(message.payload).get<std::vector<workspace_s>>();
}

bool connection::subscribe(event_type_t event) {
    json j = {};
    if (event & event_type_t::WORKSPACE) j.push_back(event_type_t::WORKSPACE);
    if (event & event_type_t::MODE) j.push_back(event_type_t::MODE);
    if (event & event_type_t::WINDOW) j.push_back(event_type_t::WINDOW);
    if (event & event_type_t::BARCONFIG_UPDATE) j.push_back(event_type_t::BARCONFIG_UPDATE);
    if (event & event_type_t::BINDING) j.push_back(event_type_t::BINDING);
    if (event & event_type_t::SHUTDOWN) j.push_back(event_type_t::SHUTDOWN);
    if (event & event_type_t::TICK) j.push_back(event_type_t::TICK);
    if (event & event_type_t::BAR_STATE_UPDATE) j.push_back(event_type_t::BAR_STATE_UPDATE);
    if (event & event_type_t::INPUT) j.push_back(event_type_t::INPUT);

    auto message = raw_request(SUBSCRIBE, j.dump());
    return json::parse(message.payload)["success"].get<bool>();
}

std::vector<output_s> connection::get_outputs() {
    auto message = raw_request(GET_OUTPUTS);
    return json::parse(message.payload).get<std::vector<output_s>>();
}

container connection::get_tree() {
    auto message = raw_request(GET_TREE);
    return json::parse(message.payload).get<container>();
}

std::vector<std::string> connection::get_marks() {
    auto message = raw_request(GET_TREE);
    return json::parse(message.payload).get<std::vector<std::string>>();
}

std::vector<std::string> connection::get_bar_config() {
    auto message = raw_request(GET_BAR_CONFIG);
    return json::parse(message.payload).get<std::vector<std::string>>();
}

bar_config_s connection::get_bar_config(std::string bar) {
    auto message = raw_request(GET_BAR_CONFIG, bar);
    return json::parse(message.payload).get<bar_config_s>();
}

version_s connection::get_version() {
    auto message = raw_request(GET_VERSION);
    return json::parse(message.payload).get<version_s>();
}

std::vector<std::string> connection::get_binding_modes() {
    auto message = raw_request(GET_BINDING_MODES);
    return json::parse(message.payload).get<std::vector<std::string>>();
}

std::string connection::get_config() {
    auto message = raw_request(GET_CONFIG);
    return json::parse(message.payload)["config"].get<std::string>();
}

bool connection::send_tick(std::string payload) {
    auto message = raw_request(SEND_TICK, payload);
    return json::parse(message.payload)["success"].get<bool>();
}

std::string connection::get_bining_state() {
    auto message = raw_request(GET_BINDING_STATE);
    return json::parse(message.payload)["name"].get<std::string>();
}

std::vector<input_s> connection::get_inputs() {
    auto message = raw_request(GET_INPUTS);
    return json::parse(message.payload).get<std::vector<input_s>>();
}

std::vector<seat_s> connection::get_seats() {
    auto message = raw_request(GET_SEATS);
    return json::parse(message.payload).get<std::vector<seat_s>>();
}

}
