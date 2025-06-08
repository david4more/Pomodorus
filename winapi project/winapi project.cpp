// winapi project.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "winapi project.h"
#include <string>
#include <vector>

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

// My variables
void draw(HDC& hdc, PAINTSTRUCT& ps);
struct msg
{
	msg(int x, int y) : x(x), y(y) {};
	int x, y;
	static const wchar_t* str;
};
const wchar_t* msg::str = L"hehe";

std::vector<msg> messages;
HWND hButton;
int buttonHeight;
int buttonWidth;
HBRUSH	hbrBackground = NULL;

int countdown = 300;
bool timerState = false;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
					 _In_opt_ HINSTANCE hPrevInstance,
					 _In_ LPWSTR    lpCmdLine,
					 _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);  

	// TODO: Place code here.

	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_WINAPIPROJECT, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINAPIPROJECT));

	MSG msg;

	// Main message loop:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	if (!hbrBackground)
		hbrBackground = CreateSolidBrush(RGB(100, 149, 237));
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style          = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc    = WndProc;
	wcex.cbClsExtra     = 0;
	wcex.cbWndExtra     = 0;
	wcex.hInstance      = hInstance;
	wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINAPIPROJECT));
	wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground	= hbrBackground;
	wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINAPIPROJECT);
	wcex.lpszClassName  = szWindowClass;
	wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
	  CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
	  return FALSE;
   }

   RECT rect;
   GetClientRect(hWnd, &rect);
   int centerX = rect.right / 2;
   int centerY = rect.bottom / 2;
   buttonWidth = rect.right / 4;
   buttonHeight = rect.bottom / 4;

   hButton = CreateWindow(
	   L"BUTTON", L"Start timer",
	   WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
	   centerX - (buttonWidth/2), centerY + (centerY / 2) - (buttonHeight / 2), buttonWidth, buttonHeight,
	   hWnd, (HMENU)1, hInstance, nullptr);

   SetTimer(hWnd, 2, 1000, NULL);

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
		{
			int wmId = LOWORD(wParam);
			// Parse the menu selections:
			switch (wmId)
			{
			case IDM_ABOUT:
				DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
				break;
			case IDM_EXIT:
				DestroyWindow(hWnd);
					break;
			case 1:
				if (!timerState)
				{
					SetWindowText(hButton, L"Stop");
					timerState = true;
				}
				else
				{
					SetWindowText(hButton, L"Start");
					timerState = false;
				}

				break;
			default:
				return DefWindowProc(hWnd, message, wParam, lParam);
			}
		}
		break;
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			// TODO: Add any drawing code that uses hdc here...

			draw(hdc, ps);

			EndPaint(hWnd, &ps);
		}
		break;
	case WM_SIZE:
		RECT rect;
		GetClientRect(hWnd, &rect);

		MoveWindow(hButton, (rect.right / 2) - (buttonWidth / 2), (rect.bottom / 2) - (buttonHeight / 2), buttonWidth, buttonHeight, TRUE);

		buttonWidth = rect.right / 4;
		buttonHeight = rect.bottom / 4;

		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_TIMER:
		if (wParam != 2)
			break;

		if (!timerState)
			break;

		if (countdown > 0)
			countdown--;

		InvalidateRect(hWnd, nullptr, TRUE);
		UpdateWindow(hWnd);

		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

void draw(HDC &hdc, PAINTSTRUCT &ps)
{
	SetBkMode(hdc, TRANSPARENT);
	
	for (auto message : messages)
	{
		TextOut(hdc, message.x, message.y, msg::str, wcslen(msg::str));
	}

	wchar_t buffer[32];
	swprintf_s(buffer, L"Time: %d", countdown);
	TextOut(hdc, 10, 10, buffer, wcslen(buffer));
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
