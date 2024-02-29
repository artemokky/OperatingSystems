#ifndef CONN_H
#define CONN_H

#include <cstddef> 

class Conn {
public:
    Conn(int id, bool create) {}

    virtual ~Conn() {}

    virtual bool Read(void *buf, size_t count) = 0;

    virtual bool Write(const void *buf, size_t count) = 0;
};

#endif // CONN_H
