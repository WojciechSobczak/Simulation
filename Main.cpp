#include "pch.h"
// Indicates to hybrid graphics systems to prefer the discrete part by default
extern "C"
{
	__declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

std::shared_ptr<Simulation> simulation = std::make_shared<Simulation>();
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	if (!DirectX::XMVerifyCPUSupport() || FAILED(CoInitializeEx(nullptr, COINITBASE_MULTITHREADED))) {
		return 1;
	}

	createWindow(hInstance, simulation, nCmdShow);

	// Main message loop
	MSG msg = {0};
	while (WM_QUIT != msg.message) {
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		} else {
			simulation->tick();
		}
	}

	simulation.reset();
	CoUninitialize();
	return (int) msg.wParam;
}
