#include <iostream>
#include <string>
#include <cstring>
#include "cmath"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#include "json.hpp"

using namespace std;
using json = nlohmann::json;

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[256] = {0};

    // Создаем сокет
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8886);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Слушаем на сокете
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    // Принимаем входящее соединение
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    while (true) {
        memset(buffer, 0, sizeof(buffer));
        read(new_socket, buffer, sizeof(buffer));

        if (buffer[0] == '\n')
            break;

        try {
            json data = json::parse(buffer);
            const string method = data["method"];
            const double value = data["value"];
            double ret = 0.0;
            if (method == "sin")
                ret = std::sin(value);
            else if (method == "cos")
                ret = std::cos(value);
            else if (method == "tg")
                ret = std::tan(value);
            else if (method == "ctg")
                ret = cos(value) / sin(value);
            else {
                send(new_socket, "Unknown method", 14, 0);
                continue;
            }

            string result = to_string(ret);
            send(new_socket, result.c_str(), result.length(), 0);
        } catch (json::parse_error &exception) {
        }
    }
    close(server_fd);
    return 0;
}
