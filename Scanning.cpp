#include "Scanning.h"

bool TakeScreenshot(std::string windowName, BITMAP &bm, HBITMAP &hbmap, BITMAPINFO &bmi, HDC &hdcShot, HBITMAP &hBitmapOld, HWND &hwnd)
{
	RECT rc;
	GetWindowRect(hwnd, &rc);

	hdcShot = CreateCompatibleDC(0);
	hbmap = CreateCompatibleBitmap(GetDC(0), rc.right - rc.left, rc.bottom - rc.top);

	SelectObject(hdcShot, hbmap);

	BitBlt(hdcShot, 0, 0, rc.right - rc.left, rc.bottom - rc.top, GetDC(0), rc.left, rc.top, SRCCOPY);

	if (!GetObject(hbmap, sizeof(BITMAP), (LPSTR)&bm)) { return false; }

	int bitsPerPixel = bm.bmBitsPixel;

	if (bitsPerPixel != 32 || bm.bmPlanes != 1) { return false; }

	SetupBitmapInfo(bmi, bm.bmWidth, bm.bmHeight, bitsPerPixel);
	return true;
}

void SetupBitmapInfo(BITMAPINFO& bmi, int bwidth, int bheight, int bitsPerPixel)
{
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = bwidth;
	bmi.bmiHeader.biHeight = bheight;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = bitsPerPixel;
	bmi.bmiHeader.biCompression = BI_RGB;
	bmi.bmiHeader.biSizeImage = 0;
}

void ShootBot(int x, int y)
{
	mouse_event(MOUSEEVENTF_LEFTDOWN, x, y, 0, 0);
	mouse_event(MOUSEEVENTF_LEFTUP, x, y, 0, 0);
}

void MoveMouse(int newX, int newY)
{
	INPUT Input = { 0 };
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_MOVE;
	Input.mi.dx = newX - CurrentMouseXY.m_x;
	Input.mi.dy = newY - CurrentMouseXY.m_y;
	SendInput(1, &Input, sizeof(INPUT));
}

void SendSerial(MouseCoord* CurrentMouseXY, MouseCoord* HeadXY)
{
	SerialPort port("COM5", 9600);
	port.Open();

	std::string inputX = std::to_string(HeadXY->m_x - CurrentMouseXY->m_x);
	std::string inputY = std::to_string(HeadXY->m_y - CurrentMouseXY->m_y);

	inputX = inputX + "*" + inputY;
	std::cout << inputX << std::endl;
	String^ data = gcnew String(inputX.c_str());

	port.Write(data);
	port.Close();
}

void ScanBMP(ScanContents* scan, MouseCoord* HeadXY)
{
	POINT currentPos;
	GetCursorPos(&currentPos);
	CurrentMouseXY.m_x = currentPos.x;
	CurrentMouseXY.m_y = currentPos.y;
	for (int y = (scan->m_rcWindow.bottom - scan->m_rcWindow.top)/4;
		y < ((scan->m_rcWindow.bottom - scan->m_rcWindow.top) - (scan->m_rcWindow.bottom - scan->m_rcWindow.top)/3.5); y++) 
	{
		for (int x = (scan->m_rcWindow.right - scan->m_rcWindow.left) / 4;
			x < ((scan->m_rcWindow.right - scan->m_rcWindow.left) - (scan->m_rcWindow.right - scan->m_rcWindow.left) / 3.5); x++)
		{
			//SetCursorPos(x + scan->m_rcWindow.left, (y + 4) + scan->m_rcWindow.top);
			if(CompareColour(scan->m_pPixels, scan->m_bm.bmHeight, scan->m_bm.bmWidth, x, y))
			{
				int z = x;
				while (z < (int)((scan->m_rcWindow.right - scan->m_rcWindow.left) - (scan->m_rcWindow.right - scan->m_rcWindow.left) / 3.5))
				{
					if (!CompareColour(scan->m_pPixels, scan->m_bm.bmHeight, scan->m_bm.bmWidth, z, y))
					{
						break;
					}
					z++;
				}

				HeadXY->m_x = (z - ((z - x) / 2)) + scan->m_rcWindow.left;
				HeadXY->m_y = (y + 4) + scan->m_rcWindow.top;

				CurrentMouseXY.m_x = currentPos.x;
				CurrentMouseXY.m_y = currentPos.y;
				return;
			}
		} 
	}
}

//These two functions are identical - any changes to one need to be made to both
bool Aim_Bot(HWND appWnd, std::string gameWindow, MouseCoord* HeadXY)
{
	RECT rcWindow;
	GetWindowRect(appWnd, &rcWindow);
	BITMAP bm;
	HBITMAP hbmap;
	HBITMAP hbmapOld;
	BITMAPINFO bmi;
	HDC hdcShot;
	HDC hdcScreen;

	RGBQUAD* pPixels;
	int TimeTakenScreenAndScan;
	TimeTakenScreenAndScan = clock();
	TakeScreenshot(gameWindow, bm, hbmap, bmi, hdcShot, hbmapOld, appWnd);

	HBITMAP hbmapNew = CreateCompatibleBitmap(hdcShot, rcWindow.right - rcWindow.left, rcWindow.bottom - rcWindow.top);

	HDC hdcShotNew = CreateCompatibleDC(hdcShot);

	HBITMAP OldBmp = (HBITMAP)SelectObject(hdcShotNew, hbmapNew);

	BitBlt(hdcShotNew, 0, 0, rcWindow.right - rcWindow.left, rcWindow.bottom - rcWindow.top, hdcShot, 0, 0, SRCCOPY);

	pPixels = new RGBQUAD[bm.bmWidth * bm.bmHeight];
	if (!pPixels) { return false; }

	SelectObject(hdcShotNew, OldBmp);

	if (!GetDIBits(hdcShotNew, hbmapNew, 0, bm.bmHeight, pPixels, &bmi, DIB_RGB_COLORS))
	{
		ReleaseDC(appWnd, hdcShot);
		delete[] pPixels;
		return false;
	}
	ReleaseDC(appWnd, hdcShot);

	ScanContents scanContentsMain(bm, rcWindow, pPixels);

	ScanBMP(&scanContentsMain, HeadXY);

	if (pPixels) { free(pPixels); }

	SelectObject(hdcShot, hbmapOld);
	DeleteObject(hbmap);
	DeleteDC(hdcShot);
	DeleteObject(hbmapNew);
	DeleteObject(OldBmp);
	DeleteDC(hdcShotNew);
	//std::cout << "Out of scan, took" << clock() - TimeTakenScreenAndScan << " milliseconds" << std::endl;
}

bool CompareColour(RGBQUAD* pPixels, int height, int width, int x, int y)
{
	int p = (height - y - 1) * width + x;

	//std::cout << (int)pPixels[p].rgbRed << ", " << (int)pPixels[p].rgbGreen << ", " << (int)pPixels[p].rgbBlue << std::endl;
	//Sleep(5);

	/*
	if ((int)pPixels[p].rgbRed < 30 && (int)pPixels[p].rgbGreen < 30 && (int)pPixels[p].rgbBlue > 215)
	{
		return true;
	}
	*/
	
	if (((int)pPixels[p].rgbRed > 235) && 
		((int)pPixels[p].rgbGreen > 105 && (int)pPixels[p].rgbGreen < 175) &&
		((int)pPixels[p].rgbBlue > 215)) 
	{ 
		return true; 
	}
	

	if (GetAsyncKeyState(VK_ESCAPE)) { exit(0); }
	return false;
}