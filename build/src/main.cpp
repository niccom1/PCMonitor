#include "collector.h"
#include "config.h"
#include "tray.h"
#include "util.h"
#include <winsock2.h>
#include <windows.h>
#include <httplib.h>
#include <nlohmann/json.hpp>
#include <filesystem>
#include <iostream>
#include <thread>
using json=nlohmann::json;
void configureConsoleEncoding(){SetConsoleCP(CP_UTF8);SetConsoleOutputCP(CP_UTF8);}
int main(int argc,char*argv[]){
    configureConsoleEncoding();
    std::string error;
    AppConfig config=loadConfig(argc,argv,error);
    if(error=="HELP") return 0;
    if(!error.empty()){std::cerr<<"配置错误: "<<error<<"\\n";return 1;}
    ShowWindow(GetConsoleWindow(),SW_HIDE);
    Collector collector; collector.start();
    httplib::Server server;
    server.Get("/api/stats",[&collector](const httplib::Request&,httplib::Response&r){
        auto s=collector.snapshot();
        json data={
            {"cpu",{{"total",s.cpuTotal},{"model",s.cpuModel}}},
            {"gpu",{{"total",s.gpuTotal},{"model",s.gpuModel},{"available",s.gpuTotal>=0.0}}},
            {"memory",{{"totalGB",s.memory.totalGB},{"usedGB",s.memory.usedGB},{"percent",s.memory.percent}}},
            {"net",{{"upMbps",s.net.upMbps},{"downMbps",s.net.downMbps},{"totalGB",s.totalGB}}},
            {"system",{{"hostname",s.hostname}}}
        };
        r.set_content(data.dump(),"application/json; charset=UTF-8");
    });
    server.set_pre_routing_handler([&collector](const httplib::Request&req,httplib::Response&r){
        if(req.method=="POST" && req.path=="/api/reset"){
            collector.resetTraffic();
            r.set_content(R"({"ok":true})","application/json; charset=UTF-8");
            return httplib::Server::HandlerResponse::Handled;
        }
        return httplib::Server::HandlerResponse::Unhandled;
    });
    std::filesystem::path root(config.webRoot);
    if(!std::filesystem::is_directory(root)){std::cerr<<"静态目录不存在: "<<root.string()<<"\\n";collector.stop();return 1;}
    if(!server.set_mount_point("/",root.string().c_str())){std::cerr<<"挂载静态目录失败\\n";collector.stop();return 1;}
    std::thread httpThread([&](){server.listen(config.bind.c_str(),config.port);});
    auto ips=getLocalIPv4List();
    TrayApp tray(config.port,ips,[&](){server.stop();collector.stop();PostQuitMessage(0);});
    if(!tray.initialize())std::cerr<<"创建系统托盘失败，继续提供 HTTP 服务\\n";
    std::cout<<" PCMonitor 已启动，监听地址: "<<config.bind<<":"<<config.port<<"\\n"<<std::flush;
    tray.messageLoop();
    server.stop();collector.stop();
    if(httpThread.joinable())httpThread.join();
    tray.shutdown();
    return 0;
}
