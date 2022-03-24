#include "Scanning.h"

MouseCoord CurrentMouseXY(0, 0);
MouseCoord HeadXY(0, 0);

int main()
{
	std::string GameWindow = "VALORANT";
	HWND appWnd = FindWindow(0, "VALORANT");

	if (!appWnd)
	{
		std::cout << "Window not found" << std::endl;
		Sleep(10000);
		return -1;
	}
	else
	{
		std::cout << "Window Found " << GameWindow << std::endl;
	}

	POINT currentPos;
	GetCursorPos(&currentPos);
	CurrentMouseXY.m_x = currentPos.x;
	CurrentMouseXY.m_y = currentPos.y;
	for (;;)
	{
		if (GetAsyncKeyState('X'))
		{
			Aim_Bot(appWnd, GameWindow, &HeadXY);
			SendSerial(&CurrentMouseXY, &HeadXY);
		}
	}

	return 0;
}