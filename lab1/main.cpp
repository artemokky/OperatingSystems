#include "Daemon.h"


int main(int argc, char** argv)
{
    if (argc != 2)
    {
        printf("Incorrect usage. Enter 1 argument with configuration file\n");
        return EXIT_FAILURE;
    }

    Daemon::getInstance().start(argv[1]);

    return EXIT_SUCCESS;
}