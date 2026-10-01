#pragma once
#include "metrics.h"
#include <atomic>
#include <cstdint>
#include <mutex>
#include <string>
#include <thread>
struct StatsCache { double cpuTotal=0.0,gpuTotal=-1.0; std::string cpuModel="未知 CPU",gpuModel="未知显卡",hostname; NetInfo net; MemoryInfo memory; double totalGB=0.0; };
class Collector { public: Collector(); ~Collector(); void start(); void stop(); StatsCache snapshot() const; void resetTraffic(); private: void run(); mutable std::mutex mutex_; StatsCache cache_; std::thread thread_; std::atomic<bool> running_{false}; uint64_t totalUpBytes_=0,totalDownBytes_=0,lastInBytes_=0,lastOutBytes_=0; bool trafficReady_=false; };
