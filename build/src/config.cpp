#include "config.h"
#include <winsock2.h>
#include <windows.h>
#include <nlohmann/json.hpp>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
using json=nlohmann::json;
namespace { std::string exeDir(){char b[MAX_PATH]={};DWORD n=GetModuleFileNameA(nullptr,b,MAX_PATH);return n&&n<MAX_PATH?std::filesystem::path(b,b+n).parent_path().string():".";} bool portOf(const std::string&s,int&v){try{size_t n=0;v=std::stoi(s,&n);return n==s.size()&&v>0&&v<=65535;}catch(...){return false;}} }
void printUsage(const char* p) {
    std::cout << "用法:\n"
              << "  " << p << " [端口]\n"
              << "  " << p << " --port 9090\n"
              << "  " << p << " --port=9090\n"
              << "  " << p << " --help\n";
}
AppConfig loadConfig(int argc,char*argv[],std::string&e){AppConfig c;auto path=std::filesystem::path(exeDir())/"config.json";if(std::filesystem::exists(path))try{std::ifstream f(path);json j;f>>j;if(j.contains("port")){c.port=j.at("port").get<int>();if(c.port<1||c.port>65535)throw std::runtime_error("port 必须在 1 到 65535 之间");c.portSource="config.json";}if(j.contains("bind"))c.bind=j.at("bind").get<std::string>();if(j.contains("web_root"))c.webRoot=j.at("web_root").get<std::string>();}catch(const std::exception&x){std::cerr<<"读取 config.json 失败，使用默认配置: "<<x.what()<<"\n";}for(int i=1;i<argc;i++){std::string a=argv[i],v;if(a=="--help"||a=="-h"){printUsage(argv[0]);e="HELP";return c;}if(a=="--port"){if(i+1>=argc){e="--port 缺少端口值";return c;}v=argv[++i];}else if(a.rfind("--port=",0)==0)v=a.substr(7);else if(i==1&&a[0]!='-')v=a;else{e="未知参数: "+a;return c;}if(!portOf(v,c.port)){e="无效端口: "+v;return c;}c.portSource="命令行";}auto r=std::filesystem::path(c.webRoot);if(r.is_relative())c.webRoot=(std::filesystem::path(exeDir())/r).lexically_normal().string();return c;}
