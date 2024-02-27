#include "conn_fifo.h"
#include <stdexcept> 
#include <cstring>   
#include <iostream>  


ConnFIFO::ConnFIFO(int id, bool create) : Conn(id, create), fd(-1) {
    fifoPath = "fifo_" + std::to_string(id); 

    if (create) {
        if (mkfifo(fifoPath.c_str(), 0666) == -1) {
            if (errno != EEXIST) {
                throw std::runtime_error("Failed to create FIFO: " + std::string(strerror(errno)));
            }
        }
        fd = open(fifoPath.c_str(), O_WRONLY);
    } else {
        fd = open(fifoPath.c_str(), O_RDONLY);
    }

    if (fd == -1) {
        throw std::runtime_error("Failed to open FIFO: " + std::string(strerror(errno)));
    }
}

ConnFIFO::~ConnFIFO() {
    if (fd != -1) {
        close(fd);
    }

    if (unlink(fifoPath.c_str()) == -1) {
        std::cerr << "Warning: Failed to delete FIFO: " << strerror(errno) << std::endl;
    }
}

bool ConnFIFO::Read(void *buf, size_t count) {
    ssize_t bytesRead = read(fd, buf, count);

    if (bytesRead == -1) {
        std::cerr << "Read error: " << strerror(errno) << std::endl;
        return false;
    }

    return true;
}

bool ConnFIFO::Write(const void *buf, size_t count) {
    ssize_t bytesWritten = write(fd, buf, count);

    if (bytesWritten == -1) {
        std::cerr << "Write error: " << strerror(errno) << std::endl;
        return false;
    }

    return true;
}
