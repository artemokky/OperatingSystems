#ifndef DAEMON_H
#define DAEMON_H

#include "Daemon.h"
#include <iostream>
#include <chrono>
#include <filesystem>
#include <thread>
#include <fstream>
#include <csignal>
#include <sys/stat.h>
#include <syslog.h>
#include <cstdio>
#include <cstdlib>
#include <sys/types.h>
#include <unistd.h>

namespace fs = std::filesystem;
const int INIT_INTERVAL = 3;

class Daemon {
private:
    static Daemon instance;
    int interval;
    const std::string pid_path = fs::absolute("/var/run/mydaemon.pid");
    fs::path config_file;
    fs::path folder1;
    fs::path folder2;

    Daemon() : interval(INIT_INTERVAL) {};
    void set_config_file(std::string config_file_path);
    int get_file_age(fs::path file_path);
    void move_files();
    void stop_daemon();
    void make_daemon();

public:
    Daemon(const Daemon&) = delete;
    Daemon& operator=(const Daemon&) = delete;

    static Daemon& getInstance() {
        static Daemon instance;
        return instance;
    }

    void start(std::string config_file_path);
    int read_config();
};


#endif
