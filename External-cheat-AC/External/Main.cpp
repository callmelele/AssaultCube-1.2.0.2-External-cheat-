#include <iostream>
#include <thread>
#include <windows.h>
#include "GDI_drawing.h"
#include "Settings.h"

HWND overlayHwnd;
HardwareMouse hwMouse;

LRESULT CALLBACK OverlayProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_PAINT: {
        PAINTSTRUCT ps;
        BeginPaint(hwnd, &ps);
        EndPaint(hwnd, &ps);
    } break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

void CreateOverlay(HWND gameHwnd) {
    RECT rect;
    GetWindowRect(gameHwnd, &rect);

    WNDCLASSEXA wc = { sizeof(WNDCLASSEXA), CS_HREDRAW | CS_VREDRAW, OverlayProc, 0, 0, GetModuleHandle(NULL), NULL, NULL, (HBRUSH)GetStockObject(BLACK_BRUSH), NULL, "OverlayClass", NULL };
    RegisterClassExA(&wc);

    overlayHwnd = CreateWindowExA(
        WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED,
        "OverlayClass", "ESP",
        WS_POPUP,
        rect.left, rect.top,
        rect.right - rect.left, rect.bottom - rect.top,
        NULL, NULL, GetModuleHandle(NULL), NULL
    );

    SetLayeredWindowAttributes(overlayHwnd, RGB(0, 0, 0), 0, LWA_COLORKEY);
    ShowWindow(overlayHwnd, SW_SHOW);
}

int main() {
    InitSettings();
    WinFunc win;
    win.GetInfo("AssaultCube");

    HWND gameHwnd = FindWindow(NULL, L"AssaultCube");
    if (!gameHwnd) {
        std::cout << "[-] AssaultCube not found! Please open the game first." << std::endl;
        Sleep(3000);
        return 1;
    }

    if (hwMouse.Connect("\\\\.\\COM7")) {
        std::cout << "[+] Hardware Mouse Connected on COM7!" << std::endl;
    }
    else {
        std::cout << "[!] Failed to find ESP32 on COM7. Aimbot will not move mouse." << std::endl;
    }

    CreateOverlay(gameHwnd);
    std::cout << "[+] Overlay Created." << std::endl;

    Player player(win);
    Entities entities(win);
    Mathematics math(win);
    GDI_drawing draw(win);

    draw.SetupDrawing(GetDC(overlayHwnd), overlayHwnd);

    std::thread espThread([&]() {
        draw.esp(entities, player, math);
        });
    espThread.detach();

    std::cout << "[+] Cheat Thread Started. Press INSERT for Menu." << std::endl;

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);

        if (gameHwnd) {
            RECT rect;
            GetWindowRect(gameHwnd, &rect);
            MoveWindow(overlayHwnd, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, TRUE);
        }

        Sleep(1);
    }

    return 0;
}