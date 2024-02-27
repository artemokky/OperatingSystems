#ifndef CONN_MQ_H
#define CONN_MQ_H

#include "conn.h"
#include <mqueue.h>
#include <string>

class ConnMQ : public Conn {
public:
    ConnMQ(int id, bool create);
    virtual ~ConnMQ();

    virtual bool Read(void *buf, size_t count) override;
    virtual bool Write(const void *buf, size_t count) override;

private:
    mqd_t mqdes;
    std::string mqName;
};

#endif // CONN_MQ_H
