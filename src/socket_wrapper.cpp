#include "socket_wrapper.hpp"

socket_wrapper::socket_wrapper(int domain, int type, int protocol) {
    descriptor = socket(domain, type, protocol);
}

socket_wrapper::~socket_wrapper() {
    if (descriptor != 0) ::close(descriptor);
}

socket_wrapper::socket_wrapper(socket_wrapper&& s) : socket_wrapper(s.descriptor) {
    s.descriptor = 0;
}

socket_wrapper& socket_wrapper::operator = (socket_wrapper&& s) {
    descriptor = s.descriptor;
    s.descriptor = 0;
    return *this;
}

bool socket_wrapper::connect(const sockaddr& serv_addr, size_t size) {
    return ::connect(descriptor, &serv_addr, size) != -1;
}

bool socket_wrapper::bind(const sockaddr& my_addr, size_t size) {
    return ::bind(descriptor, &my_addr, size) != -1;
}

bool socket_wrapper::listen(int backlog) {
    return ::listen(descriptor, backlog) != -1;
}

socket_wrapper socket_wrapper::accept(sockaddr& cliaddr, socklen_t* len) {
    return ::accept(descriptor, &cliaddr, len);
}

socket_wrapper socket_wrapper::accept() {
    return ::accept(descriptor, nullptr, nullptr);
}

void socket_wrapper::close() {
    if (descriptor != 0) {
        ::close(descriptor);
        descriptor = 0;
    }
}

bool socket_wrapper::is_invalid() const {
    return descriptor == -1;
}

std::optional<size_t> socket_wrapper::recv(void* buf, size_t len, int flags) {
    if (auto res = ::recv(descriptor, buf, len, flags); res == -1)
        return {};
    else
        return res;
}

std::optional<size_t> socket_wrapper::send(const char* buf, size_t len, int flags) {
    if (auto res = ::send(descriptor, buf, len, flags); res == -1)
        return {};
    else
        return res;
}
