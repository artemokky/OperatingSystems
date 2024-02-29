#ifndef CONN_FIFO_H
#define CONN_FIFO_H

#include "conn.h"
#include <string>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

class ConnFIFO : public Conn {
public:
    ConnFIFO(int id, bool create);
    virtual ~ConnFIFO();

    virtual bool Read(void *buf, size_t count) override;
    virtual bool Write(const void *buf, size_t count) override;

private:
    int fd;
    std::string fifoPath;
};

#endif // CONN_FIFO_H
