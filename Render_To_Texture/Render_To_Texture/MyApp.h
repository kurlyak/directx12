//======================================================================================
//	Ed Kurlyak 2023 App Class
//======================================================================================

#ifndef _MYAPP_
#define _MYAPP_

#include <windows.h>
#include <windowsx.h>
#include "MeshManager.h"

#define APPNAME L"Render To Texture DirectX12"
#define CLASSNAME L"Sample"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

class CMyApp
{
public:
	int Program_Begin(HINSTANCE	hInstance, int nCmdShow);

private:
	static LRESULT CALLBACK Static_WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	HWND m_hWnd;
	CMeshManager m_MeshManager;
};

#endif