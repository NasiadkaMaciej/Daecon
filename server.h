#include <string>
#include <vector>
#pragma once

void sendLogs(std::vector<std::string> logs, int sockfd);
void runSerer(int port);
bool processMessage(std::string &rawMessage);
std::string createMessage(int messageType, const std::string &senderInfo, const std::string &messageContent, bool color);