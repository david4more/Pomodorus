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
void draw(HDC& hdc, HWND hWnd);
void updateCoords(HWND hWnd, int& centerX, int& centerY, int& buttonHeight, int& buttonWidth);
void updateFontSize(int& fontSize);
int centerX, centerY;
HWND hButton;
int buttonHeight, buttonWidth, fontSize;
HBRUSH hbrBackground = NULL;
HFONT buttonFont = NULL;
RECT rect;

int countdown = 25 * 60;
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
	if (!InitInstance(hInstance, nCmdShow))
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

	return (int)msg.wParam;
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

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINAPIPROJECT));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = hbrBackground;
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_WINAPIPROJECT);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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

	updateCoords(hWnd, centerX, centerY, buttonHeight, buttonWidth);

	hButton = CreateWindow(
		L"BUTTON", L"Start",
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
		centerX - (buttonWidth / 2), (centerY * 1.5) - (buttonHeight / 2), buttonWidth, buttonHeight,
		hWnd, (HMENU)1, hInstance, nullptr);

	updateFontSize(fontSize);
	buttonFont = CreateFont(
		fontSize, // 12-point font
		0, 0, 0, 400, FALSE, FALSE, FALSE,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE, L"Times New Roman");

	SendMessage(hButton, WM_SETFONT, (WPARAM)buttonFont, TRUE);

	SetTimer(hWnd, 2, 1000, NULL);

	ShowWindow(hWnd, nCmdShow);

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

		draw(hdc, hWnd);

		EndPaint(hWnd, &ps);
	}
	break;
	case WM_SIZE:
		updateCoords(hWnd, centerX, centerY, buttonHeight, buttonWidth);
		MoveWindow(hButton, centerX - (buttonWidth / 2), (centerY * 1.5) - (buttonHeight / 2), buttonWidth, buttonHeight, TRUE);

		break;
	case WM_EXITSIZEMOVE:
		if (buttonFont)
			DeleteObject(buttonFont);

		updateFontSize(fontSize);
		buttonFont = CreateFont(
			fontSize, // 12-point font
			0, 0, 0, 400, FALSE, FALSE, FALSE,
			DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
			DEFAULT_PITCH | FF_DONTCARE, L"Times New Roman");

		SendMessage(hButton, WM_SETFONT, (WPARAM)buttonFont, FALSE);
		InvalidateRect(hWnd, nullptr, TRUE);

		break;
	case WM_DESTROY:
		if (hbrBackground) DeleteObject(hbrBackground);
		if (buttonFont) DeleteObject(buttonFont);
		PostQuitMessage(0);
		break;
	case WM_TIMER:
		if (wParam != 2)
			break;

		if (!timerState)
			break;

		if (countdown < 0)
			break;
		
		countdown--;
		InvalidateRect(hWnd, nullptr, TRUE);

		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

void draw(HDC& hdc, HWND hWnd)
{
	SetBkMode(hdc, TRANSPARENT);
	SelectObject(hdc, buttonFont);

	wchar_t buffer[32];
	swprintf_s(buffer, L"Time: %02d:%02d", countdown / 60, countdown % 60);

	RECT rect;
	GetClientRect(hWnd, &rect);
	DrawText(hdc, buffer, -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

void updateCoords(HWND hWnd, int& centerX, int& centerY, int& buttonHeight, int& buttonWidth)
{
	GetClientRect(hWnd, &rect);
	centerX = rect.right / 2;
	centerY = rect.bottom / 2;

	buttonWidth = rect.right / 6;
	buttonHeight = rect.bottom / 6;
}

void updateFontSize(int& fontSize)
{
	fontSize = buttonHeight * -0.5;
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
