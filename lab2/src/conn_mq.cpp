#include "conn_mq.h"
#include <stdexcept>
#include <cstring>


ConnMQ::ConnMQ(int id, bool create) : Conn(id, create), mqdes(-1) {
    mqName = "/mq_" + std::to_string(id);

    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = 1024;
    attr.mq_curmsgs = 0;

    if (create) {
        mqdes = mq_open(mqName.c_str(), O_CREAT | O_WRONLY, 0644, &attr);
    } else {
        mqdes = mq_open(mqName.c_str(), O_RDONLY);
    }

    if (mqdes == (mqd_t)-1) {
        throw std::runtime_error("Failed to open message queue: " + std::string(strerror(errno)));
    }
}

ConnMQ::~ConnMQ() {
    if (mqdes != (mqd_t)-1) {
        mq_close(mqdes);
        if (mq_unlink(mqName.c_str()) != 0) {
        }
    }
}

bool ConnMQ::Read(void *buf, size_t count) {
    if (mq_receive(mqdes, static_cast<char*>(buf), count, nullptr) == -1) {
        return false;
    }
    return true;
}

bool ConnMQ::Write(const void *buf, size_t count) {
    if (mq_send(mqdes, static_cast<const char*>(buf), count, 0) == -1) {
        return false;
    }
    return true;
}
