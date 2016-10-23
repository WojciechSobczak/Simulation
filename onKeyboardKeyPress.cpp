#include "pch.h"
#include "Simulation.h"

#define keyboard_step 1.0f;

void Simulation::onKeyboardKeyPress() {
	Vector3 buf = Vector3::Zero;
	if ((1 << 16) & GetAsyncKeyState('W')) {
		moveForward += keyboard_step;
		OutputDebugStringA("W pressed\n");
	} else if ((1 << 16) & GetAsyncKeyState('S')) {
		moveForward -= keyboard_step;
		OutputDebugStringA("S pressed\n");
	} else if ((1 << 16) & GetAsyncKeyState('A')) {
		moveRight += keyboard_step;
		OutputDebugStringA("A pressed\n");
	} else if ((1 << 16) & GetAsyncKeyState('D')) {
		moveRight -= keyboard_step;
		OutputDebugStringA("D pressed\n");
	} else if ((1 << 16) & GetAsyncKeyState('Q')) {
		moveUp += keyboard_step;
		OutputDebugStringA("Q pressed\n");
	} else if ((1 << 16) & GetAsyncKeyState('E')) {
		moveUp -= keyboard_step;
		OutputDebugStringA("E pressed\n");
	}

	resetViewMatrix();
}







