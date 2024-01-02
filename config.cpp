#include "config.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

namespace fs = std::filesystem;
std::string logFilePath = "/var/log/daecon.log";
int port = 12345;

bool readConfig(const std::string &configFile) {
	std::ifstream file(configFile);

	if (!file.is_open()) {
		// Create the directory if it does not exist
		fs::path configDir = fs::path(configFile).parent_path();
		if (!fs::exists(configDir))
			if (!fs::create_directories(configDir)) {
				std::cerr << "Error: Unable to create the configuration directory." << std::endl;
				return false;
			}

		// Create a new configuration file with default values
		std::ofstream newFile(configFile);
		if (newFile.is_open()) {
			newFile << "logFilePath=" << logFilePath << "\n";
			newFile << "port=" << port << "\n";
			newFile.close();
		} else {
			std::cerr << "Error: Unable to create a new configuration file." << std::endl;
			return false;
		}
		return true;
	}

	std::string line;
	while (std::getline(file, line)) {
		std::istringstream iss(line);
		std::string key, value;
		if (std::getline(iss, key, '=') && std::getline(iss, value))
			if (key == "logFilePath")
				logFilePath = value;
			else if (key == "port")
				port = std::stoi(value);
	}

	file.close();
	return true;
}