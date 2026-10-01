#include "collector.h"
#include "util.h"
#include <chrono>
Collector::Collector(){cache_.hostname=getHostName();cache_.cpuModel=getCpuModel();cache_.gpuModel=getGpuModel();}
Collector::~Collector(){stop();}
void Collector::start(){if(running_)return;running_=true;thread_=std::thread(&Collector::run,this);}
void Collector::stop(){running_=false;if(thread_.joinable())thread_.join();}
StatsCache Collector::snapshot()const{std::lock_guard<std::mutex> lock(mutex_);return cache_;}
void Collector::resetTraffic(){std::lock_guard<std::mutex> lock(mutex_);totalUpBytes_=totalDownBytes_=0;cache_.totalGB=0.0;}
void Collector::run(){using namespace std::chrono_literals;StatsCache pending;bool hasPending=false;while(running_){StatsCache next;next.cpuTotal=getCpuTotal();next.gpuTotal=getGpuUsage();next.net=getNetInfo();next.memory=getMemoryInfo();next.hostname=getHostName();{std::lock_guard<std::mutex> lock(mutex_);if(!trafficReady_){lastInBytes_=next.net.rawInBytes;lastOutBytes_=next.net.rawOutBytes;trafficReady_=true;}else{if(next.net.rawInBytes>=lastInBytes_)totalDownBytes_+=next.net.rawInBytes-lastInBytes_;if(next.net.rawOutBytes>=lastOutBytes_)totalUpBytes_+=next.net.rawOutBytes-lastOutBytes_;lastInBytes_=next.net.rawInBytes;lastOutBytes_=next.net.rawOutBytes;}next.totalGB=(totalUpBytes_+totalDownBytes_)/1073741824.0;next.cpuModel=cache_.cpuModel;next.gpuModel=cache_.gpuModel;if(hasPending)cache_=pending;pending=std::move(next);hasPending=true;}std::this_thread::sleep_for(1s);}}
