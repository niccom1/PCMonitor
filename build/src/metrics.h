#pragma once
#include <cstdint>
#include <string>
struct NetInfo { double upMbps=0.0; double downMbps=0.0; uint64_t rawInBytes=0; uint64_t rawOutBytes=0; };
struct MemoryInfo { double totalGB=0.0; double usedGB=0.0; double percent=0.0; };
double getCpuTotal(); std::string getCpuModel(); std::string getGpuModel(); double getGpuUsage(); MemoryInfo getMemoryInfo(); NetInfo getNetInfo();
