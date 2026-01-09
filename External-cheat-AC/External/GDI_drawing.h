#ifndef GDI_DRAWING_H  // Prevent redefinition
#define GDI_DRAWING_H

#include <windows.h>
#include <stdint.h>
#include "WinFunctions.h"
#include "Structs.h"
#include "Entities.h"
#include "Player.h"
#include "Mathematics.h"

// 1. THE DEFINITION (Only happens once)
class HardwareMouse {
private:
    HANDLE hSerial;
public:
    HardwareMouse() : hSerial(INVALID_HANDLE_VALUE) {} // Constructor

    bool Connect(const char* portName) {
        hSerial = CreateFileA(portName, GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hSerial == INVALID_HANDLE_VALUE) return false;

        DCB dcbSerialParams = { 0 };
        dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
        GetCommState(hSerial, &dcbSerialParams);
        dcbSerialParams.BaudRate = CBR_115200;
        dcbSerialParams.ByteSize = 8;
        dcbSerialParams.StopBits = ONESTOPBIT;
        dcbSerialParams.Parity = NOPARITY;
        SetCommState(hSerial, &dcbSerialParams);
        return true;
    }

    void Move(int8_t x, int8_t y) {
        if (hSerial == INVALID_HANDLE_VALUE) return;
        char buffer[2] = { (char)x, (char)y };
        DWORD bytesSent;
        WriteFile(hSerial, buffer, 2, &bytesSent, NULL);
    }
};

// 2. THE DECLARATION (Tells other files hwMouse exists elsewhere)
extern HardwareMouse hwMouse;

class GDI_drawing {
private:
    WinFunc winFunc;
public:
    GDI_drawing(WinFunc wFunc) : winFunc(wFunc) {}

    HBRUSH hBrushEnemy = CreateSolidBrush(RGB(255, 0, 0));
    HBRUSH hBrushTeam = CreateSolidBrush(RGB(0, 0, 255));
    COLORREF enemyColor = RGB(255, 0, 0);
    COLORREF teamColor = RGB(0, 0, 255);

    HDC HDC_Desktop;
    HFONT Font;
    HWND Handle;
    COLORREF TextCOLOR;

    void SetupDrawing(HDC hDesktop, HWND handle);
    void DrawFilledRect(int x, int y, int w, int h, HBRUSH brushColor);
    void DrawBorderBox(int x, int y, int w, int h, int thickness, HBRUSH brushColor);
    void DrawString(int x, int y, COLORREF color, const char* text);
    void DrawESP(int x, int y, float distance, int health, char name[20], HBRUSH hBrush, COLORREF Pen, int windowWidth, int windowHeight, int ar_ammo, int smg_ammo, int sniper_ammo, int shotgun_ammo, int carbine_ammo);
    void DrawCircle(int x, int y, int radius, COLORREF color);
    void DrawLine(int startX, int startY, int endX, int endY, COLORREF color);

    DWORD WINAPI esp(Entities entities, Player player, Mathematics math);
};

#endif