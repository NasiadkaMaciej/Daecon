#include <iostream>
#include <stdexcept>
#include <string>
#include <cstdlib>
#include <unistd.h>
#include <getopt.h>

#include "client.h"
#include "server.h"

int main(int argc, const char *argv[])
{
    int port = 12345;
    std::string arg1, arg2, arg3;
    bool runAsDaemon = false;

    const char *shortOptions = "dp:";
    const option longOptions[] = {
        {"daemon", no_argument, nullptr, 'd'},
        {"port", required_argument, nullptr, 'p'},
        {nullptr, 0, nullptr, 0}};

    int option;
    while ((option = getopt_long(argc, const_cast<char *const *>(argv), shortOptions, longOptions, nullptr)) != -1)
    {
        switch (option)
        {
        case 'd':
            runAsDaemon = true;
            break;
        case 'p':
            port = std::stoi(optarg);
            break;
        case '?':
        default:
            std::cerr << "Usage: daecon [-d] [-p port] [message_type sender_info message_content]" << std::endl;
            return EXIT_FAILURE;
        }
    }

    if (optind < argc)
    {
        // Additional arguments found, assuming client mode
        if (argc - optind == 3)
        {
            arg1 = argv[optind++];
            arg2 = argv[optind++];
            arg3 = argv[optind++];
        }
        else
        {
            std::cerr << "Usage: daecon [-d] [-p port] [message_type sender_info message_content]" << std::endl;
            return EXIT_FAILURE;
        }
    }

    try
    {
        if (runAsDaemon)
        {
            std::cout << "Server on port: " << port << std::endl;
            runSerer(port);
        }
        else if (arg1.empty())
        {
            while (runClient(port, "5", " ", " "))
                continue;
        }
        else
            runClient(port, arg1, arg2, arg3);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}