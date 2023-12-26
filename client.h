#include <string>
#pragma once

void receiveLogs(int sockfd);
bool runClient(int port, const std::string &messageType, const std::string &senderInfo, const std::string &messageContent);
bool getLogs(int port);