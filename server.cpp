#include <cstring>
#include <fstream>
#include <iostream>
#include <netinet/in.h>
#include <regex>
#include <signal.h>
#include <sstream>
#include <stdexcept>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>

#include "error.h"
#include "server.h"

// Define ANSI escape codes for colors
constexpr const char RED[] = "\x1B[31m";
constexpr const char YELLOW[] = "\x1B[33m";
constexpr const char GREEN[] = "\x1B[32m";
constexpr const char RESET[] = "\x1B[0m";

constexpr const char *logFilePath = "/home/maciej/Development/C++/Daecon/log.txt";
constexpr int BufferSize = 256;
constexpr int MaxMessageSize = 255;

constexpr int MessageTypeEmergency = 1;
constexpr int MessageTypeWarning = 2;
constexpr int MessageTypeOk = 3;
constexpr int MessageTypeDebug = 4;
constexpr int MessageTypeDisplay = 5;

std::vector<std::string> logs;

int sockfd, newsockfd;

void sendLogs(std::vector<std::string> logs, int sockfd) {
	for (const auto &log : logs) {
		ssize_t n = write(sockfd, log.c_str(), log.length());
		if (n < 0)
			throw std::runtime_error("Error writing to socket");
	}
	close(sockfd);
}
void runSerer(int port) {
	char buffer[BufferSize];
	struct sockaddr_in serv_addr, cli_addr;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		perror("ERROR opening socket");

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(port);

	if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
		throw std::runtime_error("Address already in use");

	listen(sockfd, 5);

	socklen_t clilen = sizeof(cli_addr);

	while (true) {
		newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
		if (newsockfd < 0)
			perror("ERROR on accept");

		memset(buffer, 0, BufferSize);

		// Read data from the socket until a newline character is encountered
		ssize_t n;
		std::string rawMessage;
		do {
			n = read(newsockfd, buffer, MaxMessageSize);
			if (n < 0)
				perror("ERROR reading from socket");

			rawMessage += buffer;
		} while (n > 0 && rawMessage.find('\n') == std::string::npos);

		// Process the received message
		if (processMessage(rawMessage)) {
			// IF EMPTY DO SOMETHING
			const char *response = "Message received by daemon.";
			n = write(newsockfd, response, strlen(response));
			if (n < 0)
				throw std::runtime_error("Error writing to socket");
		}

		close(newsockfd);
	}

	close(sockfd);
}

bool processMessage(std::string &rawMessage) {
	// Use stringstream to separate the message into arguments
	std::istringstream iss(rawMessage);
	std::vector<std::string> arguments;
	std::string argument;
	while (std::getline(iss, argument, '\x1F'))
		arguments.push_back(argument);

	try {
		int messageType = std::stoi(arguments[0]);
		std::string message;
		if (messageType == MessageTypeDisplay) {
			sendLogs(logs, newsockfd);
			std::cout << "Logs accessed\n";
			return false;
		} else {
			std::string messageType = arguments[0];
			std::string senderInfo = arguments[1];
			std::string messageContent = arguments[2];

			message = createMessage(std::stoi(messageType), senderInfo, messageContent, true);
			logs.push_back(message);
			std::cout << message;

			std::ofstream logFile(logFilePath, std::ios::app);
			if (!logFile.is_open())
				throw std::runtime_error("Error opening log file: " + std::string(logFilePath));
			logFile << createMessage(std::stoi(messageType), senderInfo, messageContent, false);
		}
	} catch (const std::exception &e) {
		std::cerr << "Error processing message: " << e.what() << '\n';
		return false;
	}

	return true;
}

std::string createMessage(int messageType, const std::string &senderInfo, const std::string &messageContent, bool color) {
	std::string type;
	std::string colorCode;
	switch (messageType) {
	case MessageTypeEmergency:
		type = " Emergency: ";
		colorCode = RED;
		break;
	case MessageTypeWarning:
		type = " Warning: ";
		colorCode = YELLOW;
		break;
	case MessageTypeOk:
		type = " Ok: ";
		colorCode = GREEN;
		break;
	case MessageTypeDebug:
		type = " Debug: ";
		colorCode = "";
		break;
	default:
		throw std::invalid_argument("Invalid message type");
	}

	std::time_t currentTime = std::time(nullptr);
	std::tm *localTime = std::localtime(&currentTime);
	char formattedTime[20];
	std::strftime(formattedTime, sizeof(formattedTime), "%F %T", localTime);

	if (color)
		return colorCode + formattedTime + type + senderInfo + ": " + messageContent + RESET + '\n';
	else
		return formattedTime + type + senderInfo + ": " + messageContent + '\n';
}