#include "sway_socket.hpp"

int main() {
    swayipc::sway_socket socket;
    socket.connect();

    swayipc::client conn = socket.get_client();

    conn.command("exec notify-send \"example 1\"");
}
