#include <iostream>
#include <string>
#include <vector>
#include <memory>      
#include <thread>      
#include <csignal>     
#include <cstdlib>     
#include <ctime>       
#include "logger.h"
#include "conn.h"
#include "conn_fifo.h"
#include "conn_mq.h"
#include "conn_sock.h"

Logger& logger = Logger::GetInstance("host_log.log");

volatile sig_atomic_t handshakeSignalReceived = 0;

void HandshakeSignalHandler(int signum) {
    handshakeSignalReceived = 1;
    logger.LogInfo("Handshake signal received from client");
}

void SetupSignalHandlers() {
    struct sigaction sa;
    sa.sa_handler = HandshakeSignalHandler;
    sa.sa_flags = 0; 
    sigemptyset(&sa.sa_mask);
    sigaction(SIGUSR1, &sa, nullptr);
    logger.LogInfo("Signal handlers set up");
}

std::vector<std::unique_ptr<Conn>> connections;
std::vector<std::thread> connectionThreads;

void HandleIncomingConnections(Conn* conn) {
    logger.LogInfo("Handling incoming connection on a separate thread");
    logger.LogInfo("Connection handler thread ending");
}

void InitializeIPC() {
    try {
        connections.push_back(std::make_unique<ConnFIFO>(1, true));
        connectionThreads.emplace_back(HandleIncomingConnections, connections.back().get());
        logger.LogInfo("All IPC mechanisms initialized");
    } catch (const std::runtime_error& e) {
        logger.LogError("IPC Initialization Error: " + std::string(e.what()));
        exit(EXIT_FAILURE);
    }
}

struct Goat {
    int number = 0;
    bool isAlive = true;
};

std::vector<Goat> goats;
const int numberOfGoats = 7;
const int wolfNumberRange = 100;
const int goatNumberRangeAlive = 100;
const int goatNumberRangeDead = 50;

void InitializeGame() {
    goats.resize(numberOfGoats);
    logger.LogInfo("Game initialized with " + std::to_string(numberOfGoats) + " goats");
}

void GameLogic(std::unique_ptr<Conn>& conn) {
    logger.LogInfo("GameLogic started");
    InitializeGame();
    int wolfNumber;
    int roundsWithAllDead = 0;

    while (true) {
        std::cout << "Enter a number for the wolf (1-100), or wait for random choice: ";
        if (std::cin >> wolfNumber) {
            if (wolfNumber <= 0 || wolfNumber > wolfNumberRange) {
                logger.LogInfo("Number out of range. Choosing randomly.");
                wolfNumber = rand() % wolfNumberRange + 1;
            }
        } else {
            std::this_thread::sleep_for(std::chrono::seconds(3));
            wolfNumber = rand() % wolfNumberRange + 1;
        }
        std::cin.clear();
        std::cin.ignore();
        logger.LogInfo("Wolf rolled: " + std::to_string(wolfNumber));

        int aliveCount = 0, hiddenCount = 0;
        for (Goat& goat : goats) {
            if (goat.isAlive) {
                goat.number = rand() % goatNumberRangeAlive + 1;
                if (std::abs(goat.number - wolfNumber) <= wolfNumberRange / numberOfGoats) {
                    hiddenCount++;
                } else {
                    goat.isAlive = false;
                }
                aliveCount++;
            } else {
                goat.number = rand() % goatNumberRangeDead + 1;
                if (std::abs(goat.number - wolfNumber) <= 20 / numberOfGoats) {
                    goat.isAlive = true;
                    aliveCount++;
                }
            }
        }

        int deadCount = numberOfGoats - aliveCount;

        logger.LogInfo(std::to_string(hiddenCount) + " goats hidden, "
                       + std::to_string(aliveCount) + " alive, "
                       + std::to_string(deadCount) + " dead");

        if (aliveCount == 0) {
            roundsWithAllDead++;
            if (roundsWithAllDead >= 2) {
                break;
            }
        } else {
            roundsWithAllDead = 0;
        }
    }
    logger.LogInfo("Game over!");
}

int main(int argc, char* argv[]) {
    SetupSignalHandlers();
    logger.LogInfo("Host started");
    srand(time(nullptr));
    InitializeIPC();
    std::unique_ptr<Conn> conn;
    std::thread gameLogicThread(GameLogic, std::ref(conn));
    for (auto& thr : connectionThreads) {
        if (thr.joinable())
            thr.join();
    }
    if (gameLogicThread.joinable())
        gameLogicThread.join();
    logger.LogInfo("Host exiting");
    return 0;
}
