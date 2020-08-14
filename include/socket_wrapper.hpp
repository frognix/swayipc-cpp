#pragma once

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <optional>

class socket_wrapper {
public:
    socket_wrapper() : descriptor(0) {}
    socket_wrapper(int domain, int type, int protocol=0);
    ~socket_wrapper();
    socket_wrapper(const socket_wrapper&) = delete;
    socket_wrapper operator = (const socket_wrapper&) = delete;
    socket_wrapper(socket_wrapper&&);
    socket_wrapper& operator = (socket_wrapper&&);
    bool connect(const sockaddr& serv_addr, size_t size);
    bool bind(const sockaddr& my_addr, size_t size);
    bool listen(int backlog = 10);
    socket_wrapper accept(sockaddr& cliaddr, socklen_t* = nullptr);
    socket_wrapper accept();
    std::optional<size_t> recv(void* buf, size_t len, int flags = 0);
    std::optional<size_t> send(const char* buf, size_t len, int flags = 0);
    void close();
    bool is_invalid() const;
private:
    socket_wrapper(int desc) : descriptor(desc) {}
    int descriptor;
};
