#include "AutoClicker.h"
#include <chrono>
#include <random>
#include <windows.h>

std::chrono::steady_clock::time_point lastLeftClickTime;
std::chrono::steady_clock::time_point lastRightClickTime;
std::random_device rd;
std::mt19937 gen(rd());



void AutoClicker::UpdateLeft()
{
    
    if (!GUI::leftclickEnabled) return;

    HWND HandleWindow = GetForegroundWindow();

    RECT rect;
    GetClientRect(HandleWindow, &rect);
    int screenWidth = rect.right - rect.left;
    int screenHeight = rect.bottom - rect.top;

    int centerX = screenWidth / 2;
    int centerY = screenHeight / 2;

    POINT pos_cursor;
    GetCursorPos(&pos_cursor);

    if (abs(pos_cursor.x - centerX) > 100 || abs(pos_cursor.y - centerY) > 100) return;

    auto now = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastLeftClickTime).count();
    int clickInterval = 1000 / GUI::leftclickermaxcps;

    if (duration < clickInterval) return;

    if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) {
        SendMessage(HandleWindow, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(pos_cursor.x, pos_cursor.y));
        SendMessage(HandleWindow, WM_LBUTTONUP, 0, MAKELPARAM(pos_cursor.x, pos_cursor.y));

        lastLeftClickTime = now;

        std::uniform_int_distribution<> distrib(GUI::leftclickermincps, GUI::leftclickermaxcps);
        int nextCps = distrib(gen);
    }
}

void AutoClicker::UpdateRight()
{
    
    if (!GUI::rightclickEnabled) return;

    HWND HandleWindow = GetForegroundWindow();

    RECT rect;
    GetClientRect(HandleWindow, &rect);
    int screenWidth = rect.right - rect.left;
    int screenHeight = rect.bottom - rect.top;

    int centerX = screenWidth / 2;
    int centerY = screenHeight / 2;

    POINT pos_cursor;
    GetCursorPos(&pos_cursor);

    if (abs(pos_cursor.x - centerX) > 100 || abs(pos_cursor.y - centerY) > 100) return;

    auto now = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastRightClickTime).count();
    int clickInterval = 1000 / GUI::rightclickermaxcps;

    if (duration < clickInterval) return;

    if (GetAsyncKeyState(VK_RBUTTON) & 0x8000) {
        SendMessage(HandleWindow, WM_RBUTTONDOWN, MK_RBUTTON, MAKELPARAM(pos_cursor.x, pos_cursor.y));
        SendMessage(HandleWindow, WM_RBUTTONUP, 0, MAKELPARAM(pos_cursor.x, pos_cursor.y));

        lastRightClickTime = now;

        std::uniform_int_distribution<> distrib(GUI::rightclickermincps, GUI::rightclickermaxcps);
        int nextCps = distrib(gen);
    }
}

void AutoClicker::Update()
{
    UpdateLeft();
    UpdateRight();
}
