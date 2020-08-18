#pragma once

#include <sys/un.h>
#include <mutex>
#include <thread>

#include "connection.hpp"
#include "event_stream.hpp"

namespace swayipc {

class sway_socket {
public:
    sway_socket();
    ~sway_socket();

    bool connect(std::string socket_path);
    bool connect();
    void close();

    void handle_events();

    template <class T>
    event_stream<T> get_event_stream();

    connection get_client();

    message_s request(message_type type, std::string payload = "");

private:
    message_s read(message_type type);
    message_header peek_header();
    void write(message_type type, std::string payload = "");

    std::mutex m_write_mutex;
    std::recursive_mutex m_read_mutex;
    std::string m_socket_path;
    socket_wrapper m_socket;
    std::queue<message_s> m_event_queue;

    std::map<message_type, safe_queue<message_s>> m_queues;
};

template <class T>
event_stream<T> sway_socket::get_event_stream() {
    return event_stream<T>(&m_queues[get_event_tag<T>()]);
}

}
