#include "connection.hpp"

connection::connection() : m_socket(AF_UNIX, SOCK_STREAM, 0) {}

connection::~connection() { close(); }

bool connection::connect(std::string socket_path) {
    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    std::copy(socket_path.begin(), socket_path.end(), addr.sun_path);

    return m_socket.connect((const sockaddr&)addr, sizeof(addr));
}

bool connection::connect() {
    std::string socket_path = std::getenv("SWAYSOCK");
    if (socket_path.empty()) {
        throw std::runtime_error("SWAYSOCK environment variable not found\n");
    }
    return connect(socket_path);
}

void connection::close() {
    m_socket.close();
}

message& connection::raw_read() {
    std::optional<size_t> res;

    res = m_socket.recv(&m_buffer.header, sizeof(m_buffer.header));
    if (!res) throw std::runtime_error("read error");

    m_payload_buffer.resize(m_buffer.header.length);

    res = m_socket.recv(m_payload_buffer.data(), m_buffer.header.length);
    if (!res) throw std::runtime_error("read error");
    m_buffer.payload = json::parse(m_payload_buffer);

    return m_buffer;
}

void connection::raw_send(message_type type, std::string payload) {
    m_buffer.header.type = type;
    m_buffer.header.length = payload.size();
    m_buffer.payload = json();
    m_payload_buffer = std::move(payload);

    std::optional<size_t> res;

    res = m_socket.send((const char*)&m_buffer.header, sizeof(m_buffer.header));
    if (!res) throw std::runtime_error("send error");
    if (m_payload_buffer.size() != 0) {
        res = m_socket.send(m_payload_buffer.data(), m_payload_buffer.size());
        if (!res) throw std::runtime_error("send error");
    }
}

bool connection::handle_events() {
    raw_read();

    if (m_buffer.header.type == message_type::WORKSPACE_EVENT && m_workspace_ev_handler) {
        m_workspace_ev_handler(m_buffer.payload.get<workspace_ev_s>());
    } else if (m_buffer.header.type == message_type::MODE_EVENT && m_mode_ev_handler) {
        m_mode_ev_handler(m_buffer.payload.get<mode_ev_s>());
    } else if (m_buffer.header.type == message_type::WINDOW_EVENT && m_window_ev_handler) {
        m_window_ev_handler(m_buffer.payload.get<window_ev_s>());
    } else if (m_buffer.header.type == message_type::BAR_CONFIG_UPDATE_EVENT && m_bar_config_update_ev_handler) {
        m_bar_config_update_ev_handler(m_buffer.payload.get<bar_config_update_ev_s>());
    } else if (m_buffer.header.type == message_type::BINDING_EVENT && m_binding_ev_handler) {
        m_binding_ev_handler(m_buffer.payload.get<binding_ev_s>());
    } else if (m_buffer.header.type == message_type::SHUTDOWN_EVENT && m_shutdown_ev_handler) {
        m_shutdown_ev_handler(m_buffer.payload.get<shutdown_ev_s>());
    } else if (m_buffer.header.type == message_type::TICK_EVENT && m_tick_ev_handler) {
        m_tick_ev_handler(m_buffer.payload.get<tick_ev_s>());
    } else if (m_buffer.header.type == message_type::BAR_STATE_UPDATE_EVENT && m_bar_state_update_ev_handler) {
        m_bar_state_update_ev_handler(m_buffer.payload.get<bar_state_update_ev_s>());
    } else if (m_buffer.header.type == message_type::INPUT_EVENT && m_input_ev_handler) {
        m_input_ev_handler(m_buffer.payload.get<input_ev_s>());
    } else {
        return false;
    }
    return true;
}

message& connection::raw_request(message_type type, std::string payload) {
    raw_send(type, payload);
    while (true) {
        if (!handle_events()){
            return m_buffer;
        }
    }
}

std::vector<command_success_s> connection::run_command(std::string command) {
    raw_request(RUN_COMMAND, command);
    return m_buffer.payload.get<std::vector<command_success_s>>();
}

std::vector<workspace_s> connection::get_workspaces() {
    raw_request(GET_WORKSPACES);
    return m_buffer.payload.get<std::vector<workspace_s>>();
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

    raw_request(SUBSCRIBE, j.dump());
    return m_buffer.payload["success"].get<bool>();
}

std::vector<output_s> connection::get_outputs() {
    raw_request(GET_OUTPUTS);
    return m_buffer.payload.get<std::vector<output_s>>();
}

node_s connection::get_tree() {
    raw_request(GET_TREE);
    return m_buffer.payload.get<node_s>();
}

std::vector<std::string> connection::get_marks() {
    raw_request(GET_TREE);
    return m_buffer.payload.get<std::vector<std::string>>();
}

std::vector<std::string> connection::get_bar_config() {
    raw_request(GET_BAR_CONFIG);
    return m_buffer.payload.get<std::vector<std::string>>();
}

bar_config_s connection::get_bar_config(std::string bar) {
    raw_request(GET_BAR_CONFIG, bar);
    return m_buffer.payload.get<bar_config_s>();
}

version_s connection::get_version() {
    raw_request(GET_VERSION);
    return m_buffer.payload.get<version_s>();
}

std::vector<std::string> connection::get_binding_modes() {
    raw_request(GET_BINDING_MODES);
    return m_buffer.payload.get<std::vector<std::string>>();
}

std::string connection::get_config() {
    raw_request(GET_CONFIG);
    return m_buffer.payload["config"].get<std::string>();
}

bool connection::send_tick(std::string payload) {
    raw_request(SEND_TICK, payload);
    return m_buffer.payload["success"].get<bool>();
}

std::string connection::get_bining_state() {
    raw_request(GET_BINDING_STATE);
    return m_buffer.payload["name"].get<std::string>();
}

std::vector<input_s> connection::get_inputs() {
    raw_request(GET_INPUTS);
    return m_buffer.payload.get<std::vector<input_s>>();
}

std::vector<seat_s> connection::get_seats() {
    raw_request(GET_SEATS);
    return m_buffer.payload.get<std::vector<seat_s>>();
}

void connection::set_workspace_event_handler(std::function<void(const workspace_ev_s&)> func) {
    m_workspace_ev_handler = std::move(func);
}

void connection::set_mode_event_handler(std::function<void(const mode_ev_s&)> func) {
    m_mode_ev_handler = std::move(func);
}

void connection::set_window_event_handler(std::function<void(const window_ev_s&)> func) {
    m_window_ev_handler = std::move(func);
}

void connection::set_bar_config_update_event_handler(std::function<void(const bar_config_update_ev_s&)> func) {
    m_bar_config_update_ev_handler = std::move(func);
}

void connection::set_binding_event_handler(std::function<void(const binding_ev_s&)> func) {
    m_binding_ev_handler = std::move(func);
}

void connection::set_shutdown_event_handler(std::function<void(const shutdown_ev_s&)> func) {
    m_shutdown_ev_handler = std::move(func);
}

void connection::set_tick_event_handler(std::function<void(const tick_ev_s&)> func) {
    m_tick_ev_handler = std::move(func);
}

void connection::set_bar_state_update_event_handler(std::function<void(const bar_state_update_ev_s&)> func) {
    m_bar_state_update_ev_handler = std::move(func);
}

void connection::set_input_event_handler(std::function<void(const input_ev_s&)> func) {
    m_input_ev_handler = std::move(func);
}
