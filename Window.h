#pragma once
#include "pch.h"

LRESULT CALLBACK windowMessagesHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
HWND createWindow(HINSTANCE hInstance, std::shared_ptr<Simulation> simulation, int nCmdShow);
