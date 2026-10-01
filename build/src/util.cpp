#include "util.h"
#include <winsock2.h>
#include <windows.h>
#include <iphlpapi.h>
#include <ws2tcpip.h>
#include <vector>
std::vector<std::string> getLocalIPv4List(){std::vector<std::string>out;ULONG n=16*1024;std::vector<unsigned char>b(n);auto*a=reinterpret_cast<IP_ADAPTER_ADDRESSES*>(b.data());ULONG fl=GAA_FLAG_SKIP_ANYCAST|GAA_FLAG_SKIP_MULTICAST|GAA_FLAG_SKIP_DNS_SERVER;ULONG s=GetAdaptersAddresses(AF_INET,fl,nullptr,a,&n);if(s==ERROR_BUFFER_OVERFLOW){b.resize(n);a=reinterpret_cast<IP_ADAPTER_ADDRESSES*>(b.data());s=GetAdaptersAddresses(AF_INET,fl,nullptr,a,&n);}if(s!=NO_ERROR)return out;for(;a;a=a->Next){if(a->OperStatus!=IfOperStatusUp)continue;for(auto*u=a->FirstUnicastAddress;u;u=u->Next){if(!u->Address.lpSockaddr||u->Address.lpSockaddr->sa_family!=AF_INET)continue;char x[INET_ADDRSTRLEN]={};auto*v=reinterpret_cast<sockaddr_in*>(u->Address.lpSockaddr);if(InetNtopA(AF_INET,&v->sin_addr,x,sizeof(x))&&std::string(x)!="127.0.0.1")out.emplace_back(x);}}return out;}
std::string getHostName(){char n[256]={};DWORD s=sizeof(n);return GetComputerNameA(n,&s)?std::string(n,s):"未知主机";}
