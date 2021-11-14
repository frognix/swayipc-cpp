#include <swayipc-cpp/sway_socket.hpp>

#include "assert.hpp"

using namespace std::literals::string_literals;

namespace swayipc {

sway_socket::sway_socket()
    : m_socket_path(std::getenv("SWAYSOCK")), m_socket(AF_UNIX, SOCK_STREAM, 0) {}

void sway_socket::connect(std::string socket_path) {
    m_socket_path = socket_path;
    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    std::copy(m_socket_path.begin(), m_socket_path.end(), addr.sun_path);

    if (!m_socket.connect((const sockaddr&)addr, sizeof(addr))) {
        throw std::runtime_error("swayipc-cpp: Connection error ("s + strerror(errno) + ")");
    }
}

void sway_socket::connect() {
    if (m_socket_path.empty()) {
        throw std::runtime_error("swayipc-cpp: Socket not found");
    }
    return connect(m_socket_path);
}

void sway_socket::close() {
    m_socket.close();
}

#define ANY 0xFFFFFFFF

void sway_socket::handle_events() {
    message_s message;

    std::unique_lock lock{m_read_mutex};
    message.header = peek_header();
    if (!is_event(message)) return;
    message = read(message_type(ANY));

    send_to_event_queue(std::move(message));
}

void sway_socket::send_to_event_queue(message_s&& msg) {
    Expects(is_event(msg));
    m_queues[msg.header.type].push(msg);
}

client sway_socket::get_client() {
    return client(this);
}

message_s sway_socket::request(message_type type, std::string payload) {
    write(type, payload);
    return read(type);
}

message_s sway_socket::read(message_type type) {
    message_s message;

    while (true) {
        std::unique_lock lock{m_read_mutex};
        message = message_s::recv(m_socket);

        if (message.header.type != type && type != ANY)
            send_to_event_queue(std::move(message));
        else break;
    }

    return message;
}

message_header sway_socket::peek_header() {
    std::unique_lock lock{m_read_mutex};
    return message_s::peek_header(m_socket);
}

void sway_socket::write(message_type type, std::string payload) {
    message_s message;
    message.header.type = type;
    message.header.length = payload.size();
    message.payload = std::move(payload);

    std::optional<size_t> res;

    std::unique_lock lock{m_write_mutex};

    message_s::send(m_socket, message);
}

}
