#include "pch.h"
#include "Simulation.h"

void Simulation::onKeyboardKeyPress() {
	Vector3 buf = Vector3::Zero;
	if ((1 << 16) & GetAsyncKeyState('W')) {
		moveForward = 1;
		OutputDebugStringA("W pressed\n");
	} else if ((1 << 16) & GetAsyncKeyState('S')) {
		moveForward = -1;
		OutputDebugStringA("S pressed\n");
	} else if ((1 << 16) & GetAsyncKeyState('A')) {
		moveRight = 1;
		OutputDebugStringA("A pressed\n");
	} else if ((1 << 16) & GetAsyncKeyState('D')) {
		moveRight = -1;
		OutputDebugStringA("D pressed\n");
	} else if ((1 << 16) & GetAsyncKeyState('Q')) {
		moveUp = 1;
		OutputDebugStringA("Q pressed\n");
	} else if ((1 << 16) & GetAsyncKeyState('E')) {
		moveUp = -1;
		OutputDebugStringA("E pressed\n");
	} 

	resetViewMatrix();
}







