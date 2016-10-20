#include "pch.h"
#include "Window.h"


LRESULT CALLBACK windowMessagesHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	PAINTSTRUCT ps;
	HDC hdc;

	static bool s_in_sizemove = false;
	static bool s_in_suspend = false;
	static bool s_minimized = false;
	static bool s_fullscreen = false;

	Simulation* simulation = reinterpret_cast<Simulation*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

	switch (message) {
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			EndPaint(hWnd, &ps);
			break;

		case WM_SIZE:
			if (!s_in_sizemove && simulation) {
				simulation->onWindowSizeChanged(LOWORD(lParam), HIWORD(lParam));
			}
			break;

		case WM_ENTERSIZEMOVE:
			s_in_sizemove = true;
			break;

		case WM_EXITSIZEMOVE:
			s_in_sizemove = false;
			if (simulation) {
				RECT rect;
				GetClientRect(hWnd, &rect);
				simulation->onWindowSizeChanged(rect.right - rect.left, rect.bottom - rect.top);
			}
			break;

		case WM_GETMINMAXINFO: {
			MINMAXINFO* info = reinterpret_cast<MINMAXINFO*>(lParam);
			info->ptMinTrackSize.x = 320;
			info->ptMinTrackSize.y = 200;
		}
							   break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		case WM_SYSKEYDOWN:
			if (wParam == VK_RETURN && (lParam & 0x60000000) == 0x20000000) {
				// Implements the classic ALT+ENTER fullscreen toggle
				if (s_fullscreen) {
					SetWindowLongPtr(hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
					SetWindowLongPtr(hWnd, GWL_EXSTYLE, 0);

					int width = simulation->defaultWidth;
					int height = simulation->defaultHeight;

					ShowWindow(hWnd, SW_SHOWNORMAL);
					SetWindowPos(hWnd, HWND_TOP, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);
				} else {
					SetWindowLongPtr(hWnd, GWL_STYLE, 0);
					SetWindowLongPtr(hWnd, GWL_EXSTYLE, WS_EX_TOPMOST);

					SetWindowPos(hWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
					ShowWindow(hWnd, SW_SHOWMAXIMIZED);
				}

				s_fullscreen = !s_fullscreen;
			}
			break;

		case WM_MOUSEMOVE:
			simulation->onMouseMove(LOWORD(lParam), HIWORD(lParam));
			break;
		case WM_LBUTTONDOWN:
			SetCapture(hWnd);
			simulation->onLeftMouseButtonDown(LOWORD(lParam), HIWORD(lParam));
			break;
		case WM_LBUTTONUP:
			ReleaseCapture();
			simulation->onLeftMouseButtonUp(LOWORD(lParam), HIWORD(lParam));
			break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

HWND createWindow(HINSTANCE hInstance, std::shared_ptr<Simulation> simulation, int nCmdShow) {
	// Register class and create window
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = windowMessagesHandler;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, L"IDI_ICON");
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = L"EngineWindowClass";
	wcex.hIconSm = LoadIcon(wcex.hInstance, L"IDI_ICON");
	if (!RegisterClassEx(&wcex)) {
		exit(1);
	}
	// Create window
	RECT rect;
	rect.top = 0;
	rect.left = 0;
	rect.right = static_cast<LONG>(simulation->defaultWidth);
	rect.bottom = static_cast<LONG>(simulation->defaultHeight);

	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	HWND windowHandler = CreateWindowEx(
		0, L"EngineWindowClass",
		L"Engine",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		rect.right - rect.left,
		rect.bottom - rect.top,
		nullptr,
		nullptr,
		hInstance,
		nullptr
	);

	if (!windowHandler) {
		exit(1);
	}

	ShowWindow(windowHandler, nCmdShow);
	SetWindowLongPtr(windowHandler, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(simulation.get()));
	GetClientRect(windowHandler, &rect);
	simulation->initialize(windowHandler, rect.right - rect.left, rect.bottom - rect.top);
	return windowHandler;
}
