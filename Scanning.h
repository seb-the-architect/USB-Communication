#pragma once
#include <process.h>
#include <iostream>
#include <windows.h>
#include <ctime>
#include <string>
#using <system.dll>

using namespace System;
using namespace System::IO::Ports;

class MouseCoord
{
public:
	int m_x;
	int m_y;
	MouseCoord(int x, int y)
	{
		m_x = x;
		m_y = y;
	}
};

bool TakeScreenshot(std::string windowName, BITMAP& bm, HBITMAP& hbmap, BITMAPINFO& bmi, HDC& hdcShot, HBITMAP& hBitmapOld, HWND& hwnd);

void SetupBitmapInfo(BITMAPINFO& bmi, int bwidth, int bheight, int bitsPerPixel);

bool CompareColour(RGBQUAD* pPixels, int height, int width, int x, int y);

void SendSerial(MouseCoord* CurrentMouseXY, MouseCoord* HeadXY);

bool Aim_Bot(HWND appWnd, std::string gameWindow, MouseCoord* HeadXY);

class ScanContents
{
public:
	BITMAP m_bm;
	RECT m_rcWindow;
	RGBQUAD* m_pPixels;

	ScanContents(BITMAP bm, RECT rcWindow, RGBQUAD* pPixels)
	{
		m_bm = bm;
		m_rcWindow = rcWindow;
		m_pPixels = pPixels;
	}
};

void MoveMouse(int newX, int newY);

void ScanBMP(ScanContents* scan, MouseCoord* relative);

extern MouseCoord CurrentMouseXY;