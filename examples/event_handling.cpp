#include "sway_socket.hpp"
#include <iostream>

using swayipc::event_type_t,
    swayipc::data::window_ev_s,
    swayipc::data::window_ev_change_t;

void event_handler(swayipc::client conn, swayipc::event_stream<window_ev_s> stream) {
    while (true) {
        window_ev_s event = stream.get_event();

        // print name of focused container to standart output
        if (event.change == window_ev_change_t::FOCUS) {
            std::cout << event.container.name << std::endl;
        }
    }
}

int main() {
    swayipc::sway_socket socket;
    socket.connect();

    swayipc::client conn = socket.get_client();

    conn.subscribe(event_type_t::WINDOW);
    // you can subscribe to many events with | operator
    // conn.subscribe(event_type_t::WINDOW | event_type_t::WORKSPACE);

    // connection and event_stream classes are threadsafe
    // you can run event handler in a separate thread
    std::thread thread(event_handler, conn,
                    socket.get_event_stream<window_ev_s>());
    thread.detach();

    while (true) {
        socket.handle_events();
    }

}
