#include <swayipc-cpp/message.hpp>
#include <swayipc-cpp/socket_wrapper.hpp>

using namespace swayipc;
using namespace std::literals::string_literals;

void message_s::send(socket_wrapper& socket, const message_s& message) {
    std::optional<size_t> res;

    res = socket.send((const char*)&message.header, sizeof(message.header));
    if (!res) throw std::runtime_error("swayipc-cpp: message send error ("s + strerror(errno) + ")");

    if (message.header.length != 0) {
        res = socket.send(message.payload.data(), message.header.length);
        if (!res) throw std::runtime_error("swayipc-cpp: message send error ("s + strerror(errno) + ")");
    }
}

message_s message_s::recv(socket_wrapper& socket) {
    message_s message;
    std::optional<size_t> res;

    res = socket.recv(&message.header, sizeof(message.header));
    if (!res) throw std::runtime_error("swayipc-cpp: message recv error ("s + strerror(errno) + ")");

    message.payload.resize(message.header.length);

    res = socket.recv(message.payload.data(), message.header.length);
    if (!res) throw std::runtime_error("swayipc-cpp: message recv error ("s + strerror(errno) + ")");

    return message;
}

message_header message_s::peek_header(socket_wrapper& socket) {
    std::optional<size_t> res;
    message_header header;

    res = socket.recv(&header, sizeof(header), MSG_PEEK);
    if (!res) throw std::runtime_error("swayipc-cpp: message header peek error ("s + strerror(errno) + ")");

    return header;
}
