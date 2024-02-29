#include "conn_sock.h"
#include <stdexcept>
#include <cstring>
#include <unistd.h>
#include <iostream>

ConnSock::ConnSock(int id, bool create) : Conn(id, create), sock(-1) {
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        throw std::runtime_error("Failed to create socket: " + std::string(strerror(errno)));
    }

    address.sin_family = AF_INET;
    address.sin_port = htons(10000);
    address.sin_addr.s_addr = htonl(INADDR_ANY);

    if (create) {
        if (bind(sock, (struct sockaddr *)&address, sizeof(address)) == -1) {
            throw std::runtime_error("Failed to bind socket: " + std::string(strerror(errno)));
        }

        if (listen(sock, 5) == -1) {
            throw std::runtime_error("Failed to listen on socket: " + std::string(strerror(errno)));
        }
    } else {
        if (connect(sock, (struct sockaddr *)&address, sizeof(address)) == -1) {
            throw std::runtime_error("Failed to connect to socket: " + std::string(strerror(errno)));
        }
    }
}

ConnSock::~ConnSock() {
    if (sock != -1) {
        close(sock);
    }
}

bool ConnSock::Read(void *buf, size_t count) {
    ssize_t bytesRead = recv(sock, buf, count, 0);

    if (bytesRead == -1) {
        std::cerr << "Read error: " << strerror(errno) << std::endl;
        return false;
    }

    return true;
}

bool ConnSock::Write(const void *buf, size_t count) {
    ssize_t bytesWritten = send(sock, buf, count, 0);

    if (bytesWritten == -1) {
        std::cerr << "Write error: " << strerror(errno) << std::endl;
        return false;
    }

    return true;
}
