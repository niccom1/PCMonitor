#pragma once
#include <windows.h>
#include <shellapi.h>
#include <functional>
#include <string>
#include <vector>
class TrayApp {
public:
    TrayApp(int port, std::vector<std::string> localIps, std::function<void()> onExit);
    ~TrayApp();
    bool initialize();
    int messageLoop();
    void shutdown();
    HWND windowHandle() const { return window_; }
private:
    static LRESULT CALLBACK windowProc(HWND, UINT, WPARAM, LPARAM);
    LRESULT handleMessage(UINT, WPARAM, LPARAM);
    void showMenu();
    void openMonitor();
    void copyLanAddress();
    bool addIcon();
    void deleteIcon();
    int port_;
    std::vector<std::string> localIps_;
    std::function<void()> onExit_;
    HINSTANCE instance_=nullptr;
    HWND window_=nullptr;
    HICON icon_=nullptr;
    NOTIFYICONDATAW notify_{};
    bool iconAdded_=false;
    UINT callbackMessage_=WM_APP+1;
};
