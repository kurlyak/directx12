//======================================================================================
//	Ed Kurlyak 2023 App Class
//======================================================================================

#include "MyApp.h"

int WINAPI WinMain(HINSTANCE hInstance,
					HINSTANCE hPrevInstance,
					LPSTR lpCmdLine,
					int nCmdShow)
{
	CMyApp App;

	return App.Program_Begin(hInstance, nCmdShow);
}

LRESULT CALLBACK CMyApp::Static_WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	if ( uMsg == WM_CREATE )
		SetWindowLong( hWnd, GWL_USERDATA, (LONG)(LONG_PTR)((CREATESTRUCT FAR *)lParam)->lpCreateParams);

	CMyApp *MainWndProc = (CMyApp*)(LONG_PTR)GetWindowLong( hWnd, GWL_USERDATA );
	if (MainWndProc) return MainWndProc->WndProc( hWnd, uMsg, wParam, lParam );
	
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

LRESULT CMyApp::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//проверяем сообщения
	switch (uMsg)
	{
		//мы получили сообщение закрыть приложение
		case WM_CLOSE:
			PostQuitMessage(0);
			break;

		default:
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	return 0;
}

int CMyApp::Program_Begin(HINSTANCE	hInstance, int nCmdShow)
{
	WNDCLASS wcl;

	wcl.style			= CS_HREDRAW | CS_VREDRAW;;
	wcl.lpfnWndProc		= (WNDPROC) Static_WndProc;
	wcl.cbClsExtra		= 0L;
	wcl.cbWndExtra		= 0L;
	wcl.hInstance		= GetModuleHandle(NULL);
	wcl.hIcon			= LoadIcon(NULL, IDI_WINLOGO);
	wcl.hCursor			= LoadCursor(NULL, IDC_ARROW);
	wcl.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcl.lpszMenuName	= NULL;
	wcl.lpszClassName	= CLASSNAME;
	
	if(!RegisterClass (&wcl)) return 0;

	m_hWnd = CreateWindow(CLASSNAME, APPNAME,
			  WS_OVERLAPPEDWINDOW,
              0, 0, WINDOW_WIDTH, WINDOW_HEIGHT,
              NULL, NULL, hInstance, this);

	if (!m_hWnd) return 0;
	
	RECT WindowRect = {0,0,WINDOW_WIDTH,WINDOW_HEIGHT};

	AdjustWindowRectEx(&WindowRect,
		GetWindowStyle(m_hWnd),
		GetMenu(m_hWnd) != NULL,
		GetWindowExStyle(m_hWnd));

	UINT WidthScreen = GetSystemMetrics(SM_CXSCREEN);
	UINT HeightScreen = GetSystemMetrics(SM_CYSCREEN);

	UINT WidthX = WindowRect.right - WindowRect.left;
	UINT WidthY = WindowRect.bottom - WindowRect.top;

	UINT PosX =  (WidthScreen - WidthX)/2;
	UINT PosY =  (HeightScreen - WidthY)/2;
	
	MoveWindow(m_hWnd,
		PosX,
        PosY,
        WidthX,
        WidthY,
        FALSE);

	ShowWindow (m_hWnd, nCmdShow);
	UpdateWindow (m_hWnd);
	SetForegroundWindow(m_hWnd);

	m_MeshManager.Init_MeshManager(m_hWnd);
	
	MSG msg;

    while (TRUE)
    {
	    if (PeekMessage (&msg, NULL, 0, 0, PM_REMOVE))
        {
		    if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
            DispatchMessage(&msg);
		}
        
		if(GetKeyState(VK_ESCAPE) & 0xFF00)
			break;

		m_MeshManager.Update_MeshManager();
		m_MeshManager.Draw_MeshManager();
	}
	
	DestroyWindow(m_hWnd);
	UnregisterClass(wcl.lpszClassName, wcl.hInstance);

	return (int) msg.wParam;
}



