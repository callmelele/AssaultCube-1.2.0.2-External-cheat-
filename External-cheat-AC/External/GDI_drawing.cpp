#include "GDI_drawing.h"
#include <sstream>
#include <iostream>
#include "Settings.h"
#include "Offsets.h"
#include <cmath>

#pragma warning(disable : 4996) 
#pragma warning(disable : 4244) 

extern HWND overlayHwnd;

void GDI_drawing::SetupDrawing(HDC hDesktop, HWND handle)
{
    this->HDC_Desktop = hDesktop;
    this->Handle = handle;
    this->TextCOLOR = RGB(0, 255, 0);

    this->Font = CreateFontA(14, 0, 0, 0, FW_BOLD, false, false, false, DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE, "Arial");
}

void GDI_drawing::DrawFilledRect(int x, int y, int w, int h, HBRUSH brushColor)
{
    RECT rect = { x, y, x + w, y + h };
    FillRect(this->HDC_Desktop, &rect, brushColor);
}

void GDI_drawing::DrawBorderBox(int x, int y, int w, int h, int thickness, HBRUSH brushColor)
{
    this->DrawFilledRect(x, y, w, thickness, brushColor);
    this->DrawFilledRect(x, y, thickness, h, brushColor);
    this->DrawFilledRect((x + w - thickness), y, thickness, h, brushColor);
    this->DrawFilledRect(x, y + h - thickness, w, thickness, brushColor);
}

void GDI_drawing::DrawString(int x, int y, COLORREF color, const char* text)
{
    SetTextAlign(this->HDC_Desktop, TA_CENTER | TA_NOUPDATECP);
    SetBkMode(this->HDC_Desktop, TRANSPARENT);
    SetTextColor(this->HDC_Desktop, color);
    SelectObject(this->HDC_Desktop, this->Font);
    TextOutA(this->HDC_Desktop, x, y, text, (int)strlen(text));
}

void GDI_drawing::DrawESP(
    int x, int y, float distance, int health, char name[20], HBRUSH hBrush, COLORREF Pen,
    int windowWidth, int windowHeight,
    int ar_ammo, int smg_ammo, int sniper_ammo, int shotgun_ammo, int carbine_ammo)
{
    if (!g_showESP) return;

    int width = static_cast<int>(windowWidth * 1.1375 / distance);
    int height = static_cast<int>(windowHeight * 3.3333 / distance);

    if (g_showBoxes)
        this->DrawBorderBox(x - (width / 2), y - height, width, height, 1, hBrush);

    if (g_showHealthBar)
    {
        int barWidth = 3;
        int barX = x - (width / 2) - barWidth - 2;
        int barY = y - height;
        int filledHeight = static_cast<int>(height * (health / 100.0f));
        int filledY = barY + (height - filledHeight);

        HBRUSH hBrushBG = CreateSolidBrush(RGB(40, 40, 40));
        this->DrawFilledRect(barX, barY, barWidth, height, hBrushBG);
        DeleteObject(hBrushBG);

        COLORREF healthColor = (health > 60) ? RGB(0, 255, 0) : (health > 30) ? RGB(255, 255, 0) : RGB(255, 0, 0);
        HBRUSH hBrushHealth = CreateSolidBrush(healthColor);
        this->DrawFilledRect(barX, filledY, barWidth, filledHeight, hBrushHealth);
        DeleteObject(hBrushHealth);
    }

    if (g_showNames)
    {
        std::string distStr = std::to_string((int)distance) + "m";
        this->DrawString(x, y - height - 20, this->TextCOLOR, name);
        this->DrawString(x, y - height - 35, this->TextCOLOR, distStr.c_str());
    }
}

void GDI_drawing::DrawLine(int startX, int startY, int endX, int endY, COLORREF color)
{
    HPEN hLinePen = CreatePen(PS_SOLID, 1, color);
    HPEN hOldPen = (HPEN)SelectObject(this->HDC_Desktop, hLinePen);

    // Move to the start point (e.g., bottom of screen)
    MoveToEx(this->HDC_Desktop, startX, startY, NULL);

    // Draw the line to the target (enemy feet)
    LineTo(this->HDC_Desktop, endX, endY);

    // Clean up memory
    SelectObject(this->HDC_Desktop, hOldPen);
    DeleteObject(hLinePen);
}

void GDI_drawing::DrawCircle(int x, int y, int radius, COLORREF color)
{
    // Create a pen for the outline
    HPEN hPen = CreatePen(PS_SOLID, 1, color);
    HPEN hOldPen = (HPEN)SelectObject(this->HDC_Desktop, hPen);

    // Select a NULL brush so the circle is not filled
    HBRUSH hOldBrush = (HBRUSH)SelectObject(this->HDC_Desktop, GetStockObject(NULL_BRUSH));

    // Ellipse takes (left, top, right, bottom) coordinates
    Ellipse(this->HDC_Desktop, x - radius, y - radius, x + radius, y + radius);

    // Clean up
    SelectObject(this->HDC_Desktop, hOldPen);
    SelectObject(this->HDC_Desktop, hOldBrush);
    DeleteObject(hPen);
}

DWORD WINAPI GDI_drawing::esp(Entities entities, Player player, Mathematics math)
{
    bool g_showMenu = true;
    int fovRadius = 200; // This matches your 'if (dist < 200)' logic

    

    while (true)
    {
        // --- 1. HANDLE INPUT TOGGLES ---
        if (GetAsyncKeyState(VK_INSERT) & 1) g_showMenu = !g_showMenu;

        if (g_showMenu) {
            if (GetAsyncKeyState(VK_F1) & 1) g_showESP = !g_showESP;
            if (GetAsyncKeyState(VK_F2) & 1) g_showBoxes = !g_showBoxes;
            if (GetAsyncKeyState(VK_F3) & 1) g_showHealthBar = !g_showHealthBar;
            if (GetAsyncKeyState(VK_F4) & 1) g_showNames = !g_showNames;
            if (GetAsyncKeyState(VK_F5) & 1) g_showTrigger = !g_showTrigger;
            if (GetAsyncKeyState(VK_F6) & 1) g_showTriggerAim = !g_showTriggerAim;
            if (GetAsyncKeyState(VK_F7) & 1) g_showaimbot = !g_showaimbot;
            if (GetAsyncKeyState(VK_F8) & 1) g_showSmoothing = !g_showSmoothing;
            if (GetAsyncKeyState(VK_F9) & 1) g_drawFOV = !g_drawFOV;
            if (GetAsyncKeyState(VK_F10) & 1) g_showSnaplines = !g_showSnaplines;
            if (GetAsyncKeyState(VK_F11) & 1) g_targetAll = !g_targetAll;
        }

        player.GetInfo();
        entities.GetInfo();

        HWND gameHwnd = FindWindowA(NULL, "AssaultCube");
        if (!gameHwnd) { Sleep(100); continue; }

        RECT clientRect;
        GetClientRect(gameHwnd, &clientRect);
        int w = clientRect.right;
        int h = clientRect.bottom;

        int centerX = w / 2;
        int centerY = h / 2;

        HDC overlayDC = GetDC(overlayHwnd);
        HDC memDC = CreateCompatibleDC(overlayDC);
        HBITMAP memBitmap = CreateCompatibleBitmap(overlayDC, w, h);
        SelectObject(memDC, memBitmap);

        HBRUSH hBlack = CreateSolidBrush(RGB(0, 0, 0));
        FillRect(memDC, &clientRect, hBlack);
        DeleteObject(hBlack);

        this->HDC_Desktop = memDC;

        if (g_drawFOV)
        {
            // Draws a green circle around the center of the screen
            this->DrawCircle(centerX, centerY, fovRadius, RGB(0, 255, 0));
        }


        // --- 2. DRAW MENU ---
        if (g_showMenu) {
            HBRUSH hMenuBg = CreateSolidBrush(RGB(30, 30, 30));
            this->DrawFilledRect(10, 10, 200, 210, hMenuBg);
            DeleteObject(hMenuBg);
            this->DrawString(106, 25, RGB(255, 255, 0), "INSERT: MENU");
            this->DrawString(106, 45, g_showESP ? RGB(0, 255, 0) : RGB(255, 0, 0), "F1: ESP");
            this->DrawString(106, 60, g_showBoxes ? RGB(0, 255, 0) : RGB(255, 0, 0), "F2: BOXES");
            this->DrawString(106, 75, g_showHealthBar ? RGB(0, 255, 0) : RGB(255, 0, 0), "F3: HEALTH");
            this->DrawString(106, 90, g_showNames ? RGB(0, 255, 0) : RGB(255, 0, 0), "F4: NAMES");
            this->DrawString(106, 105, g_showTrigger ? RGB(0, 255, 0) : RGB(255, 0, 0), "F5: TriggerBot");
            this->DrawString(106, 120, g_showTriggerAim ? RGB(0, 255, 0) : RGB(255, 0, 0), "F6: TriggerBot (Aimbot Only)");
            this->DrawString(106, 135, g_showaimbot ? RGB(0, 255, 0) : RGB(255, 0, 0), "F7: Aimbot");
            this->DrawString(106, 150, g_showSmoothing ? RGB(0, 255, 0) : RGB(0, 200, 255), g_showSmoothing ? "F8: Aimbot Type: smooth" : "F8: Aimbot Type: snappy");
            this->DrawString(106, 165, g_drawFOV ? RGB(0, 255, 0) : RGB(255, 0, 0), "F9: Draw Fov");
            this->DrawString(106, 180, g_showSnaplines ? RGB(0, 255, 0) : RGB(255, 0, 0), "F10: SnapLines");
            this->DrawString(106, 195, g_targetAll ? RGB(255, 165, 0) : RGB(0, 200, 255), g_targetAll ? "F11: Target All" : "F11: Enemies Only");
        }

        for (int i = 0; i < entities.amount; i++)
        // --- 3. ESP LOOP ---
        {
            if (entities.list[i].health <= 0 || entities.list[i].health > 100) continue;
            if (math.WorldToScreen(entities.list[i].position_feet, math.screen, player.matrix, w, h))

            {
                HBRUSH currentBrush = (entities.list[i].team != player.team) ? hBrushEnemy : hBrushTeam;
                COLORREF currentColor = (entities.list[i].team != player.team) ? enemyColor : teamColor;

                float dist3D = math.GetDistance3D(player.position_feet, entities.list[i].position_feet);

                this->DrawESP(math.screen.x, math.screen.y, dist3D,
                    entities.list[i].health, entities.list[i].name, currentBrush, currentColor,
                    w, h, 0, 0, 0, 0, 0);

                if (g_showSnaplines)
                {
                    this->DrawLine(w / 2, h, (int)math.screen.x, (int)math.screen.y, currentColor);
                }
            }
            
        }

        // --- 4. INDEPENDENT AIMBOT LOGIC ---
        if (g_showaimbot && (GetAsyncKeyState(VK_RBUTTON) & 0x8000))
        {
            int centerX = w / 2;
            int centerY = h / 2;
            float closestToCrosshair = 99999.0f;
            int targetX = 0, targetY = 0;
            bool foundTarget = false;

            for (int i = 0; i < entities.amount; i++)
            {
                if (entities.list[i].health <= 0) continue;
                if (!g_targetAll && entities.list[i].team == player.team) continue;

                if (math.WorldToScreen(entities.list[i].position_head, math.screen, player.matrix, w, h))
                {
                    float dx = math.screen.x - centerX;
                    float dy = math.screen.y - centerY;
                    float dist = sqrt(dx * dx + dy * dy);

                    if (dist < closestToCrosshair && dist < 200)
                    {
                        closestToCrosshair = dist;
                        targetX = (int)dx;
                        targetY = (int)dy;
                        foundTarget = true;
                    }
                }
            }

            if (foundTarget)
            {
                float smoothing = 0.4f;
                if (g_showSmoothing) {
                    mouse_event(MOUSEEVENTF_MOVE, (DWORD)(targetX* smoothing), (DWORD)(targetY* smoothing), 0, 0);
                }  else if (!g_showSmoothing) {
                    mouse_event(MOUSEEVENTF_MOVE, (DWORD)(targetX), (DWORD)(targetY), 0, 0);
				}
                


                if (g_showTriggerAim) {
                    mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
                    Sleep(10);
                    mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
                }
            }

        }



        // --- 5. INDEPENDENT TRIGGERBOT LOGIC ---
        if (g_showTrigger && !g_showTriggerAim)
        {
            int centerX = w / 2;
            int centerY = h / 2;
            bool foundShootable = false;

            for (int i = 0; i < entities.amount; i++)
            {
                if (entities.list[i].health <= 0) continue;
                if (!g_targetAll && entities.list[i].team == player.team) continue;

                if (math.WorldToScreen(entities.list[i].position_head, math.screen, player.matrix, w, h))
                {
                    float dx = math.screen.x - centerX;
                    float dy = math.screen.y - centerY;

                    // Trigger radius: 35 pixels
                    if (abs(dx) < 55 && abs(dy) < 55)
                    {
                        foundShootable = true;
                        break;
                    }
                }
            }

            if (foundShootable)
            {
                mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
                Sleep(10);
                mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
            }
        }



        BitBlt(overlayDC, 0, 0, w, h, memDC, 0, 0, SRCCOPY);
        DeleteObject(memBitmap);
        DeleteDC(memDC);
        ReleaseDC(overlayHwnd, overlayDC);
        Sleep(2);
    }
}