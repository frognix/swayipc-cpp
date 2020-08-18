#pragma once

#include <sys/un.h>
#include <mutex>
#include <thread>

#include "connection.hpp"
#include "event_stream.hpp"

class sway_socket {
public:
    sway_socket();
    ~sway_socket();

    bool connect(std::string socket_path);
    bool connect();
    void close();

    void handle_events();
    template <class T>
    void set_event_handler(std::function<void(connection conn, event_stream<T> stream)>);

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
void sway_socket::set_event_handler(std::function<void(connection conn, event_stream<T> stream)> func) {
    m_queues[get_event_tag<T>()] = safe_queue<message_s>{};
    std::thread thr(func, connection{this}, event_stream<T>(&m_queues[get_event_tag<T>()]));
    thr.detach();
}
