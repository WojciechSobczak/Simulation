#include "pch.h"
#include "Simulation.h"
#ifdef DEBUG
#define KEYBOARD_DEBUG
#endif
void Simulation::onKeyboardKeyPress() {
	Vector3 buf = Vector3::Zero;
	if ((1 << 16) & GetAsyncKeyState('W')) {
		moveForward = 1;
	#ifdef KEYBOARD_DEBUG
		Debug::printString("W pressed");
	#endif // KEYBOARD_DEBUG
	} else if ((1 << 16) & GetAsyncKeyState('S')) {
		moveForward = -1;
	#ifdef KEYBOARD_DEBUG
		Debug::printString("S pressed");
	#endif // KEYBOARD_DEBUG
	} 
	if ((1 << 16) & GetAsyncKeyState('A')) {
		moveRight = 1;
	#ifdef KEYBOARD_DEBUG
		Debug::printString("A pressed");
	#endif // KEYBOARD_DEBUG
	} else if ((1 << 16) & GetAsyncKeyState('D')) {
		moveRight = -1;
	#ifdef KEYBOARD_DEBUG
		Debug::printString("D pressed");
	#endif // KEYBOARD_DEBUG
	} 
	
	if ((1 << 16) & GetAsyncKeyState('Q')) {
		moveUp = 1;
	#ifdef KEYBOARD_DEBUG
		Debug::printString("Q pressed");
	#endif // KEYBOARD_DEBUG
	} else if ((1 << 16) & GetAsyncKeyState('E')) {
		moveUp = -1;
	#ifdef KEYBOARD_DEBUG
		Debug::printString("E pressed");
	#endif // KEYBOARD_DEBUG
	} 

	static int zpressed = 0;
	if (0x8000 & GetKeyState('Z') && zpressed++ % 2) {
		physicsEnabled = !physicsEnabled;
	#ifdef KEYBOARD_DEBUG
		Debug::printString("Z pressed");
	#endif // KEYBOARD_DEBUG
	}

	static int pPressed;
	if (0x8000 & GetKeyState('P') && pPressed++ % 2) {
		increaseMSAA();
	#ifdef KEYBOARD_DEBUG
		Debug::printString("P pressed");
	#endif // KEYBOARD_DEBUG
	}

	static int oPressed;
	if (0x8000 & GetKeyState('O') && oPressed++ % 2) {
		decreaseMSAA();
	#ifdef KEYBOARD_DEBUG
		Debug::printString("O pressed");
	#endif // KEYBOARD_DEBUG
	}

	resetViewMatrix();
}







