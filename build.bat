@echo off
setlocal
if not exist third_party\httplib.h (echo 缺少 third_party\httplib.h&exit /b 1)
if not exist third_party\nlohmann\json.hpp (echo 缺少 third_party\nlohmann\json.hpp&exit /b 1)
where g++ >nul 2>nul
if errorlevel 1 (echo 未找到 MinGW g++&exit /b 1)
g++ -std=c++17 -O2 -pthread -static -static-libgcc -static-libstdc++ -finput-charset=UTF-8 -fexec-charset=UTF-8 -D_WIN32_WINNT=0x0601 -DNTDDI_VERSION=0x06010000 -DWINAPI_FAMILY=WINAPI_FAMILY_DESKTOP_APP -DWIN32_LEAN_AND_MEAN -DNOMINMAX -DUNICODE -D_UNICODE -Ithird_party -Isrc src\*.cpp -o PCMonitor.exe -lws2_32 -lpdh -liphlpapi -lole32 -loleaut32 -lwbemuuid -ldxgi -ladvapi32 -lshell32 -luser32 -lgdi32
if errorlevel 1 (echo 编译失败&exit /b 1)
echo 编译成功: PCMonitor.exe
endlocal
