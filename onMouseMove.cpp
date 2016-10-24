#include "pch.h"
#define mouse_step 0.01f;
#define control_angle(x) if (x > 2*M_PI) {x = 0;} else if (x < 0) {x = 2*M_PI;}
#define pitch_control_angle(x) if (x > M_PI_2) {x = M_PI_2 - 0.01f;} else if (x < -M_PI_2) {x = -M_PI_2 + 0.01f;}

void Simulation::onMouseOver(int x, int y) {
	if (x > windowWidth || y > windowHeight) {
		return;
	}

	if (float(y) > float(windowHeight) * 0.8f) {
		pitch += mouse_step;
	} else if (float(y) < float(windowHeight) * 0.2f) {
		pitch -= mouse_step;
	}

	if ((float(x) > float(windowWidth) * 0.8f)) {
		yaw -= mouse_step;
	} else if ((float(x) < float(windowWidth) * 0.2f)) {
		yaw += mouse_step;
	}

	control_angle(yaw);
	pitch_control_angle(pitch);

	prevMousex = x;
	prevMousey = y;
#ifdef MOUSE_MOVE_DEBUG
	std::string s;
	s += "pitch = " + std::to_string(pitch) + " ";
	s += "yaw = " + std::to_string(yaw) + "\n";
	OutputDebugStringA(s.c_str());
#endif

	resetViewMatrix();
}

