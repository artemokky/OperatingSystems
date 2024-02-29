#include <iostream>
#include <vector>
#include <memory>      
#include <cstdlib>     
#include <ctime>       
#include <cstring>     
#include "logger.h"
#include "conn.h"
#include "conn_fifo.h"
#include "conn_mq.h"
#include "conn_sock.h"

Logger& logger = Logger::GetInstance("client_log.log");

struct Goat {
    int number = 0;
    bool isAlive = true;
};

std::vector<Goat> goats;
const int numberOfGoats = 7;
const int goatNumberRangeAlive = 100;
const int goatNumberRangeDead = 50;
const int wolfNumberRange = 100;

void InitializeGoats() {
    goats.resize(numberOfGoats, Goat());
    logger.LogInfo("Client initialized with " + std::to_string(numberOfGoats) + " goats");
}

void UpdateGoatStatus(Goat& goat, int status) {
    goat.isAlive = (status != 0);
}

void PlayRound(std::unique_ptr<Conn>& conn, int wolfNumber) {
    for (Goat& goat : goats) {
        if (goat.isAlive) {
            goat.number = rand() % goatNumberRangeAlive + 1;
        } else {
            goat.number = rand() % goatNumberRangeDead + 1;
        }

        conn->Write(&goat.number, sizeof(int));

        int status;
        conn->Read(&status, sizeof(int));
        UpdateGoatStatus(goat, status);
    }
    logger.LogInfo("Round played, goat numbers sent to host and statuses updated");
}

int main() {
    srand(time(nullptr));
    logger.LogInfo("Client started");


    int fifoId = 1; 
    std::unique_ptr<Conn> conn = std::make_unique<ConnFIFO>(fifoId, false);

    InitializeGoats();

    int wolfNumber;
    while (true) {

        if (conn->Read(&wolfNumber, sizeof(int)) <= 0) {
            logger.LogError("Error reading wolf number from host");
            break;
        }
        if (wolfNumber == -1) { 
            logger.LogInfo("Game over signal received");
            break;
        }

        logger.LogInfo("Received wolf number: " + std::to_string(wolfNumber));
        PlayRound(conn, wolfNumber);
    }

    logger.LogInfo("Client exiting");
    return 0;
}
