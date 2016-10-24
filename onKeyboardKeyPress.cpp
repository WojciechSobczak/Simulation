#include "pch.h"
#include "Simulation.h"

void Simulation::onKeyboardKeyPress() {
	Vector3 buf = Vector3::Zero;
	if ((1 << 16) & GetAsyncKeyState('W')) {
		moveForward = 1;
	#ifdef KEYBOARD_DEBUG
		OutputDebugStringA("W pressed\n");
	#endif // KEYBOARD_DEBUG
	} else if ((1 << 16) & GetAsyncKeyState('S')) {
		moveForward = -1;
	#ifdef KEYBOARD_DEBUG
		OutputDebugStringA("S pressed\n");
	#endif // KEYBOARD_DEBUG
	} 
	if ((1 << 16) & GetAsyncKeyState('A')) {
		moveRight = 1;
	#ifdef KEYBOARD_DEBUG
		OutputDebugStringA("A pressed\n");
	#endif // KEYBOARD_DEBUG
	} else if ((1 << 16) & GetAsyncKeyState('D')) {
		moveRight = -1;
	#ifdef KEYBOARD_DEBUG
		OutputDebugStringA("D pressed\n");
	#endif // KEYBOARD_DEBUG
	} 
	
	if ((1 << 16) & GetAsyncKeyState('Q')) {
		moveUp = 1;
	#ifdef KEYBOARD_DEBUG
		OutputDebugStringA("Q pressed\n");
	#endif // KEYBOARD_DEBUG
	} else if ((1 << 16) & GetAsyncKeyState('E')) {
		moveUp = -1;
	#ifdef KEYBOARD_DEBUG
		OutputDebugStringA("E pressed\n");
	#endif // KEYBOARD_DEBUG
	} 

	resetViewMatrix();
}







