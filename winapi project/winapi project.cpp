// winapi project.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "winapi project.h"
#include <string>
#include <vector>

#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

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
void draw(HDC hdc, HWND hWnd);
void updateCoords(HWND hWnd, int& centerX, int& centerY, int& buttonHeight, int& buttonWidth);
void updateFontSize(int& fontSize);
int centerX, centerY;
HWND hButton;
int buttonHeight, buttonWidth, fontSize;
HBRUSH hbrBackground = NULL;
HFONT buttonFont = NULL;
RECT rect;

int countdown = 0;
bool timerState = false;
void toggleTimer(HWND hWnd);
void activateAlarm(HWND hWnd);

HWND hEditFirst, hEditSecond;
int timeEditWidth = 25, timeEditHeight = 20;
int GetTimeValue(HWND hEdit);
bool inputValidation(HWND hWnd, int& value);

HWND hCheckbox;
int checkboxWidth = 70, checkboxHeight = 30;
int cycleNumber = 0, firstCycle = 0, secondCycle = 0;
bool cyclesMode = false;

HWND hButtonUpdate;
int updateButtonWidth = 70, updateButtonHeight = 30;

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
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg) &&
			!IsDialogMessage(GetActiveWindow(), &msg))
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

	hEditFirst = CreateWindow(
		L"EDIT", L"25",
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER,
		(centerX / 8) - (timeEditWidth / 2), (centerY / 4) - (timeEditHeight / 2), timeEditWidth, timeEditHeight,
		hWnd, (HMENU)3, hInstance, nullptr);

	hEditSecond = CreateWindow(
		L"EDIT", L"5",
		WS_TABSTOP| WS_VISIBLE | WS_CHILD  | WS_BORDER | ES_NUMBER,
		(centerX / 8) + (timeEditWidth / 2), (centerY / 4) - (timeEditHeight / 2), timeEditWidth, timeEditHeight,
		hWnd, (HMENU)4, hInstance, nullptr);

	hCheckbox = CreateWindowW(
		L"BUTTON", L"Cycles",
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
		(centerX / 8) - (timeEditWidth / 2), (centerY / 4) - (timeEditHeight * 2), checkboxWidth, checkboxHeight,
		hWnd, (HMENU)5, hInst, nullptr);
	SendMessage(hCheckbox, BM_SETCHECK, BST_CHECKED, 0);

	hButtonUpdate = CreateWindow(
		L"BUTTON", L"Update",
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
		(centerX / 8) - (timeEditWidth / 2), (centerY / 4) + (timeEditHeight / 2), updateButtonWidth, updateButtonHeight,
		hWnd, (HMENU)2, hInstance, nullptr);

	mciSendString(L"open \"C:\\Users\\doman\\Downloads\\Mystic Night.mp3\" type mpegvideo alias alarm", NULL, 0, NULL);

	SetWindowPos(hCheckbox, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	SetWindowPos(hEditFirst, hCheckbox, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	SetWindowPos(hEditSecond, hEditFirst, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	SetWindowPos(hButtonUpdate, hEditSecond, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	SetWindowPos(hButton, hButtonUpdate, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

	SendMessage(hButton, WM_SETFONT, (WPARAM)buttonFont, TRUE);

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
			toggleTimer(hWnd);
			break;
		case 2:
		{
			cyclesMode = SendMessage(hCheckbox, BM_GETCHECK, 0, 0) == BST_CHECKED;
			if (cyclesMode)
			{
				firstCycle = GetTimeValue(hEditFirst);
				inputValidation(hWnd, firstCycle);
				secondCycle = GetTimeValue(hEditSecond);
				inputValidation(hWnd, secondCycle);

				firstCycle *= 60; secondCycle *= 60;

				countdown = firstCycle;
			}
			else
			{
				cycleNumber = 0;
				int mins = GetTimeValue(hEditFirst);
				int secs = GetTimeValue(hEditSecond);

				if (!inputValidation(hWnd, mins))
					secs = 59;
				else
					inputValidation(hWnd, secs);
				countdown = (mins * 60) + secs;
			}

			InvalidateRect(hWnd, nullptr, TRUE);
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
		DestroyWindow(hButton);
		DestroyWindow(hEditFirst);
		DestroyWindow(hEditSecond);
		DestroyWindow(hCheckbox);
		DestroyWindow(hButtonUpdate);

		if (hbrBackground) 
			DeleteObject(hbrBackground);
		if (buttonFont) 
			DeleteObject(buttonFont);
		if (timerState)
			toggleTimer(hWnd);
		mciSendString(L"close alarm", NULL, 0, NULL);
		
		PostQuitMessage(0);
		break;
	case WM_TIMER:

		countdown--;
		if (countdown <= 0)
		{
			countdown = 0;
			toggleTimer(hWnd);
			activateAlarm(hWnd);
		}
		else
			InvalidateRect(hWnd, nullptr, TRUE);

		break; 
	case WM_CREATE:
		PostMessage(hWnd, WM_COMMAND, 2, 0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

void draw(HDC hdc, HWND hWnd)
{
	SetBkMode(hdc, TRANSPARENT);
	SelectObject(hdc, buttonFont);

	wchar_t buffer[32];

	if (cyclesMode)
		swprintf_s(buffer, L"%s: %02d:%02d", (cycleNumber % 2 == 0) ? L"Work" : L"Rest", countdown / 60, countdown % 60);
	else
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
	fontSize = sqrt(pow(buttonWidth, 2) + pow(buttonHeight, 2)) * -0.25;
}

void toggleTimer(HWND hWnd)
{
	if (timerState)
	{
		SetWindowText(hButton, L"Start");
		KillTimer(hWnd, 2);
		timerState = false;
	}
	else
	{
		SetWindowText(hButton, L"Stop");
		SetTimer(hWnd, 2, 1000, NULL);
		timerState = true;
	}
}

void activateAlarm(HWND hWnd)
{
	InvalidateRect(hWnd, nullptr, TRUE);

	mciSendString(L"seek alarm to start", NULL, 0, NULL);
	mciSendString(L"play alarm", NULL, 0, NULL);
	MessageBox(hWnd, L"⏰ Timer expired!", L"Alarm", MB_OK);
	mciSendString(L"stop alarm", NULL, 0, NULL);

	if (cyclesMode)
	{
		cycleNumber++;
		countdown = (cycleNumber % 2 == 0) ? firstCycle : secondCycle;
		toggleTimer(hWnd);
		InvalidateRect(hWnd, nullptr, TRUE);
	}
}

bool inputValidation(HWND hWnd, int& value)
{
	if (value <= 59)
		return true;

	value = 59;
	MessageBox(hWnd, L"Variable value is out of bounds.", L"Invalid input", MB_OK);
	return false;
}

int GetTimeValue(HWND hEdit)
{
	wchar_t buffer[16];
	GetWindowText(hEdit, buffer, sizeof(buffer) / sizeof(buffer[0]));
	return _wtoi(buffer);
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
