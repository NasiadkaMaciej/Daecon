#include <arpa/inet.h>
#include <cstring>
#include <ctime>
#include <iostream>
#include <netinet/in.h>
#include <string>
#include <unistd.h>

#include "client.h"
#include "error.h"

// Define ANSI escape codes for colors
constexpr const char RED[] = "\x1B[31m";
constexpr const char YELLOW[] = "\x1B[33m";
constexpr const char GREEN[] = "\x1B[32m";
constexpr const char RESET[] = "\x1B[0m";
constexpr int BufferSize = 1048576;

int sendMessage(int port, std::string &message) {
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		perror("ERROR opening socket");
		return false;
	}

	sockaddr_in serv_addr{};
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);

	if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
		perror("ERROR invalid address");
		close(sockfd);
		return false;
	}

	if (connect(sockfd, reinterpret_cast<sockaddr *>(&serv_addr), sizeof(serv_addr)) < 0) {
		perror("ERROR connecting");
		close(sockfd);
		return false;
	}
	// Send the message to the daemon
	ssize_t n = write(sockfd, message.c_str(), message.length());
	if (n < 0) {
		perror("ERROR writing to socket");
		close(sockfd);
		return false;
	}
	return sockfd;
}

bool runClient(int port, const std::string &messageType, const std::string &senderInfo, const std::string &messageContent) {

	std::string message = messageType + "" + senderInfo + "" + messageContent + "\n";

	int sockfd = sendMessage(port, message);
	if (!sockfd)
		return false;

	// Read the response from the daemon (optional)
	char buffer[BufferSize];
	bzero(buffer, sizeof(buffer));
	int n = read(sockfd, buffer, sizeof(buffer) - 1);
	if (n < 0) {
		perror("ERROR reading from socket");
		close(sockfd);
		return false;
	} else {
		std::cout << "Response from daemon: \n" << buffer << std::endl;
		return true;
	}

	close(sockfd);
	return false;
}
bool getLogs(int port) {
	std::string message = "5\n";
	int sockfd = sendMessage(port, message);
	if (!sockfd)
		return false;

	while (true) {
		// Read the response from the daemon (optional)
		char buffer[BufferSize];
		bzero(buffer, sizeof(buffer));
		int n = read(sockfd, buffer, sizeof(buffer) - 1);
		if (n < 0) {
			perror("ERROR reading from socket");
			close(sockfd);
			return false;
		} else {
			std::cout << "Response from daemon: \n" << buffer;
			return false;
		}
	}

	close(sockfd);
	return false;
}