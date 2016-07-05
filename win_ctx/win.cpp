#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <iostream>
#include "ogl.h"
#include "game.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT uInt, WPARAM wParam, LPARAM lParam);

HDC hdc;
HGLRC hglrc;
PIXELFORMATDESCRIPTOR pfd = {
	sizeof(PIXELFORMATDESCRIPTOR),  //  size of this pfd  
	1,                     // version number  
	PFD_DRAW_TO_WINDOW |   // support window  
	PFD_SUPPORT_OPENGL |   // support OpenGL  
	PFD_DOUBLEBUFFER,      // double buffered  
	PFD_TYPE_RGBA,         // RGBA type  
	24,                    // 24-bit color depth  
	0, 0, 0, 0, 0, 0,      // color bits ignored  
	0,                     // no alpha buffer  
	0,                     // shift bit ignored  
	0,                     // no accumulation buffer  
	0, 0, 0, 0,            // accum bits ignored  
	32,                    // 32-bit z-buffer      
	0,                     // no stencil buffer  
	0,                     // no auxiliary buffer  
	PFD_MAIN_PLANE,        // main layer  
	0,                     // reserved  
	0, 0, 0                // layer masks ignored  
};

bool running = true;

void show_last_error()
{

	LPTSTR msg;
	DWORD dw = GetLastError();

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&msg,
		0, NULL);

	std::string str = std::string(msg);
	std::cout << "LAST SYSTEM ERROR CODE [" << dw << "]=" << str.c_str();

	LocalFree(msg);

	msg = NULL;
}

int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	MSG msg;

	WNDCLASSEX wcex;
	HWND hWnd;

	AllocConsole();
	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);

	static TCHAR szWindowClass[] = _T("Prospectors");
	static TCHAR szTitle[] = _T("Prospectors - explore, extract, exploit");

	
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_OWNDC;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

	if (!RegisterClassEx(&wcex))
	{
		std::cout << "RegisterClassEx() failed" << std::endl;
		return 1;
	}

	hWnd = CreateWindow(
		szWindowClass,
		szTitle,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		800, 800,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	if (!hWnd)
	{
		std::cout << "CreateWindow() failed" << std::endl;
		return 1;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	game::init();

	while (running)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		
		game::update(1);

		SwapBuffers(hdc);
	}
	
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		running = false;
		wglDeleteContext(hglrc);
		break;
	case WM_CREATE:

		hdc = GetDC(hWnd);
		if (hdc == NULL) {
			std::cout << "GetDC() failed" << std::endl;
			show_last_error();
		}

		int  iPixelFormat;
		iPixelFormat = ChoosePixelFormat(hdc, &pfd);

		if (iPixelFormat == 0) {
			std::cout << "ChoosePixelFormat() failed" << std::endl;
			show_last_error();
		}

		if (SetPixelFormat(hdc, iPixelFormat, &pfd) == FALSE) {
			std::cout << "SetPixelFormat() failed" << std::endl;
			show_last_error();
		}

		hglrc = wglCreateContext(hdc);
		if (hglrc == NULL) {
			std::cout << "wglCreateContext() failed" << std::endl;
			show_last_error();
		}

		if (!wglMakeCurrent(hdc, hglrc)) {
			std::cout << "wglMakeCurrent() failed" << std::endl;
			show_last_error();
		}
		
		load_gl_functions();
		std::cout << "OpenGL version: " << (char*)glGetString(GL_VERSION) << std::endl;
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}
	return 0;
}
