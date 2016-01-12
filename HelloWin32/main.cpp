#include<Windows.h>
#include<tchar.h>
#include <thread>
#include <chrono>

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void CallForRepaint(HWND hWnd);

static wchar_t APPNAME[] = L"MyWinApp";
static wchar_t WINNAME[] = L"MyWindow";
static int movment = 20;

struct gameObject {
	int x;
	int y;
	int h;
	int w;
};

static gameObject GameObject;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	//Windows Class
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = APPNAME;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	if (!RegisterClassEx(&wcex)) {
		return 8;
	}

	//Window
	HWND myWindow = CreateWindow(APPNAME, WINNAME, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
	if (!myWindow) {
		return 9;
	}
	//The squre on screen
	GameObject.x = 0;
	GameObject.y = 0;
	GameObject.w = 50;
	GameObject.h = 50;
	ShowWindow(myWindow, nCmdShow);

	//Animation Thread
	std::thread aniThread(CallForRepaint, myWindow);

	//Message Loop
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) 
{
	PAINTSTRUCT ps;
	HDC hdc;
	RECT myRect;

	switch (message)
	{

	case WM_LBUTTONDOWN:
		GameObject.x += movment;
		GameObject.y += movment;
		RedrawWindow(hWnd, NULL, NULL, RDW_INTERNALPAINT | RDW_INVALIDATE | RDW_ERASE);
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_RIGHT:
			GameObject.x += movment;
			break;
		case VK_LEFT:
			GameObject.x -= movment;
			break;
		case VK_UP:
			GameObject.y -= movment;
			break;
		case VK_DOWN:
			GameObject.y += movment;
			break;
		}
		RedrawWindow(hWnd, NULL, NULL, RDW_INTERNALPAINT | RDW_INVALIDATE | RDW_ERASE);
		break;

	case WM_PAINT:

		myRect.left = GameObject.x;
		myRect.top = GameObject.y;
		myRect.right = GameObject.x + GameObject.w;
		myRect.bottom = GameObject.y + GameObject.h;

		hdc = BeginPaint(hWnd, &ps);
		FillRect(hdc, &myRect, CreateSolidBrush(RGB(255, 0, 0)));

		EndPaint(hWnd, &ps);

		break;
	
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}
}

void CallForRepaint(HWND hWnd)
{
	while (true)
	{
		GameObject.x += 20;
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		RedrawWindow(hWnd, NULL, NULL, RDW_INTERNALPAINT | RDW_INVALIDATE | RDW_ERASE);
	}
}