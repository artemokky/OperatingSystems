#ifndef CONN_SOCK_H
#define CONN_SOCK_H

#include "conn.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <string>

class ConnSock : public Conn {
public:
    ConnSock(int id, bool create);
    virtual ~ConnSock();

    virtual bool Read(void *buf, size_t count) override;
    virtual bool Write(const void *buf, size_t count) override;

private:
    int sock;
    struct sockaddr_in address;
};

#endif // CONN_SOCK_H
