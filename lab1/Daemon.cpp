#include "Daemon.h"


static void signal_handler(int sig) {
    switch (sig){
        case SIGHUP:
            Daemon::getInstance().read_config();
            break;
        case SIGTERM:
            syslog(LOG_NOTICE, "Daemon terminated");
            closelog();
            exit(EXIT_SUCCESS);
            break;
        default:
            break;

    }
}

void Daemon::set_config_file(std::string config_file_path) {
    if (fs::exists(config_file_path)) {
        config_file = fs::absolute(config_file_path);
    }
    else {
        syslog(LOG_NOTICE, "Config file does not exist. Daemon terminated");
        closelog();
        exit(EXIT_FAILURE);
    }

}

int Daemon::read_config() {
    std::ifstream input(config_file);
    if(!input) {
        syslog(LOG_ERR, "File do not exist/permisson denied");
        return 1;
    }
    try {
        std::string line;
        std::getline(input, line);
        folder1 = line;
        std::getline(input, line);
        folder2 = line;
        std::getline(input, line);
        interval = std::stoi(line);
    }
    catch (const std::runtime_error& e) {
        syslog(LOG_ERR, "exception: %s" ,e.what());
        return 1;
    }
    return 0;
}

int Daemon::get_file_age(fs::path file_path) { 
    fs::file_time_type file_time = fs::last_write_time(file_path);
    fs::file_time_type now = fs::file_time_type::clock::now();
    fs::file_time_type::duration age = now - file_time;

    int fileAge = std::chrono::duration_cast<std::chrono::duration<double>>(age).count();
    return fileAge;
}

void Daemon::move_files() {
    for (const auto& entry : fs::directory_iterator(folder1)) {
        if (fs::is_regular_file(entry.path())) {
            auto fileAge = get_file_age(entry.path());
            if (fileAge > 2 * 60) {
                fs::rename(entry.path(), folder2 / entry.path().filename());
            }
        }
    }
}


void Daemon::start(std::string config_file_path) {
    set_config_file(config_file_path);
    stop_daemon();
    make_daemon();
    if (read_config() == 1) {
        syslog(LOG_NOTICE, "Daemon not started");
        return;
    }
    syslog(LOG_NOTICE, "Daemon started");
    while (true) {
        try {
            move_files();
            std::this_thread::sleep_for(std::chrono::seconds(interval));
        }
        catch (const std::runtime_error& e) {
            syslog(LOG_ERR, "exception: %s" ,e.what());
            break;
        }
    }
}

void Daemon::stop_daemon(){
    int pid;
    std::ifstream f(pid_path);
    f >> pid;
    if (fs::exists("/proc/" + std::to_string(pid)))
        kill(pid, SIGTERM);
    syslog(LOG_NOTICE, "Daemon terminated");
}

void Daemon::make_daemon() {
    pid_t pid = fork();
    if (pid != 0) {
        exit(EXIT_FAILURE);
    }

    if (setsid() < 0) {
        exit(EXIT_FAILURE);
    }

    std::signal(SIGHUP, signal_handler);
    std::signal(SIGTERM, signal_handler);

    pid = fork();
    if (pid != 0)
        exit(EXIT_FAILURE);

    umask(0);
    if (chdir("/") != 0)
        exit(EXIT_FAILURE);

    for (long x = sysconf(_SC_OPEN_MAX); x >= 0; --x)
        close(x);
    openlog("mydaemon", LOG_PID, LOG_DAEMON);

    std::ofstream f(pid_path, std::ios_base::trunc);
    f << getpid();
}

