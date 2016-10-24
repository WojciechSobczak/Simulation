#pragma once
#include "pch.h"

namespace Debug {
	inline void printVector3(DirectX::SimpleMath::Vector3 vec, std::string prefix = "") {
		std::string s = prefix + "Vec3: x=" + std::to_string(vec.x) + ", y=" + std::to_string(vec.y) + ", z=" + std::to_string(vec.z) + "\n";
		OutputDebugStringA(s.c_str());
	}

	inline void printXMVECTOR(DirectX::XMVECTOR vec, std::string prefix = "") {
		std::string s = prefix + "Vec3: x=" + std::to_string(vec.m128_f32[0]) + ", y=" + std::to_string(vec.m128_f32[1]) + ", z=" + std::to_string(vec.m128_f32[2]) + "\n";
		OutputDebugStringA(s.c_str());
	}

	inline void printTime(clock_t time, std::string prefix = "") {
		std::string s = prefix + "Time: " + std::to_string(float(time) / CLOCKS_PER_SEC) + "\n";
		OutputDebugStringA(s.c_str());
	}
}