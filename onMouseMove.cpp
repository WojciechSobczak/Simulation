#include "pch.h"
#define MOUSE_MOVE_DEBUG

void Simulation::onMouseMove(int x, int y) {
	if (!leftMouseButtonDown || x > windowWidth || y > windowHeight) {
		return;
	}

	if (y > prevMousey) {
		phi += 0.03f;
	} else {
		phi -= 0.03f;
	}

	if (x > prevMousex) {
		theta += 0.03f;
	} else {
		theta -= 0.03f;
	}

	prevMousex = x;
	prevMousey = y;

	float tx = r * cos(phi) * cos(theta);
	float tz = r * cos(phi) * sin(theta);
	float ty = r * sin(phi);

#ifdef MOUSE_MOVE_DEBUG
	std::string s;
	s += "x = " + std::to_string(tx) + " ";
	s += "y = " + std::to_string(ty) + " ";
	s += "z = " + std::to_string(tz) + " ";
	s += "r = " + std::to_string(r) + " ";
	s += "phi = " + std::to_string(phi) + " ";
	s += "theta = " + std::to_string(theta) + "\n";
	OutputDebugStringA(s.c_str());
#endif
	basicEffect->SetView(Matrix::CreateLookAt(Vector3(tx, ty, tz), Vector3::Zero, Vector3(Vector3::UnitY.x, cos(phi), Vector3::UnitY.z)));
}