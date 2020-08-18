#include "sway_socket.hpp"

sway_socket::sway_socket()
    : m_socket_path(std::getenv("SWAYSOCK")), m_socket(AF_UNIX, SOCK_STREAM, 0) {}

sway_socket::~sway_socket() { close(); }

bool sway_socket::connect(std::string socket_path) {
    m_socket_path = socket_path;
    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    std::copy(m_socket_path.begin(), m_socket_path.end(), addr.sun_path);

    return m_socket.connect((const sockaddr&)addr, sizeof(addr));
}

bool sway_socket::connect() {
    if (m_socket_path.empty()) {
        throw std::runtime_error("Sway socket path not found not found\n");
    }
    return connect(m_socket_path);
}

void sway_socket::close() {
    m_socket.close();
}

#include <iostream>

void sway_socket::handle_events() {
    message_s message;
    if (m_event_queue.empty()) {
        std::unique_lock lock{m_read_mutex};
        message.header = peek_header();
        if (!is_event(message)) return;
        message = read(ANY);
    } else {
        message = std::move(m_event_queue.front());
        m_event_queue.pop();
    }

    if (is_event(message)) {
        m_queues[message.header.type].push(message);
    }
}

message_s sway_socket::request(message_type type, std::string payload) {
    write(type, payload);
    return read(type);
}

static size_t read_num = {};
message_s sway_socket::read(message_type type) {
    std::optional<size_t> res;
    message_s message;

    while (true) {
        std::unique_lock lock{m_read_mutex};
        res = m_socket.recv(&message.header, sizeof(message.header));
        if (!res) throw std::runtime_error("read error");

        message.payload.resize(message.header.length);

        res = m_socket.recv(message.payload.data(), message.header.length);
        if (!res) throw std::runtime_error("read error");

        read_num++;
        if (message.header.type != type && type != ANY)
            m_event_queue.push(message);
        else break;
    }

    return message;
}

message_header sway_socket::peek_header() {
    std::optional<size_t> res;
    message_header header;

    std::unique_lock lock{m_read_mutex};
    res = m_socket.recv(&header, sizeof(header), MSG_PEEK);
    if (!res) throw std::runtime_error("read error");

    return header;
}

void sway_socket::write(message_type type, std::string payload) {
    message_s message;
    message.header.type = type;
    message.header.length = payload.size();
    message.payload = std::move(payload);

    std::optional<size_t> res;

    std::unique_lock lock{m_write_mutex};

    res = m_socket.send((const char*)&message.header, sizeof(message.header));
    if (!res) throw std::runtime_error("send error");

    if (message.header.length != 0) {
        res = m_socket.send(message.payload.data(), message.header.length);
        if (!res) throw std::runtime_error("send error");
    }
}
