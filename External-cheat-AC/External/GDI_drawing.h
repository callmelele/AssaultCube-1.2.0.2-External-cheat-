//#pragma once
#include "WinFunctions.h"
#include "Structs.h"
#include "Entities.h"
#include "Player.h"
#include "Mathematics.h"

class GDI_drawing {

private:
	WinFunc winFunc;

public:

	GDI_drawing(WinFunc wFunc)
	{
		winFunc = wFunc;
	}

	HBRUSH hBrushEnemy = CreateSolidBrush(RGB(255, 0, 0));
	HBRUSH hBrushTeam = CreateSolidBrush(RGB(0, 0, 255));
	HBRUSH hBrushNeutral = CreateSolidBrush(RGB(255, 255, 255));
	COLORREF enemyColor = RGB(255, 0, 0);
	COLORREF teamColor = RGB(0, 0, 255);
	COLORREF neutralColor = RGB(255, 255, 255);
	
	//ESP
	HDC HDC_Desktop;
	HFONT Font;
	HWND TargetWnd;
	HWND Handle;
	COLORREF TextCOLOR;
	RECT m_Rect;

	void SetupDrawing(HDC hDesktop, HWND handle);

	void DrawFilledRect(int x, int y, int w, int h, HBRUSH brushColor);

	void DrawBorderBox(int x, int y, int w, int h, int thickness, HBRUSH brushColor);
	
	void DrawString(int x, int y, COLORREF color, const char* text);

	void DrawESP(
		int x, int y,
		float distance,
		int health,
		char name[20],
		HBRUSH hBrush,
		COLORREF Pen,
		int windowWidth, int windowHeight,
		int ar_ammo, int smg_ammo, int sniper_ammo, int shotgun_ammo, int carbine_ammo
	);
	void DrawCircle(int x, int y, int radius, COLORREF color);
	void DrawLine(int startX, int startY, int endX, int endY, COLORREF color);


	DWORD WINAPI esp(Entities entities, Player player, Mathematics math);


};