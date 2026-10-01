#pragma once
#include <string>
struct AppConfig { int port=8080; std::string bind="0.0.0.0"; std::string webRoot="./web"; std::string portSource="默认值"; };
AppConfig loadConfig(int argc,char* argv[],std::string& errorMessage);
void printUsage(const char* programName);
