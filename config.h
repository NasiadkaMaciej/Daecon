#include <string>
#pragma once

extern std::string logFilePath;
extern int port;

bool readConfig(const std::string& configFile);