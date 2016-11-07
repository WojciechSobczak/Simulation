#pragma once
#include "pch.h"

namespace Debug {
	inline void printVector3(DirectX::SimpleMath::Vector3 vec, std::string prefix = "") {
		#ifdef DEBUG
		std::string s = prefix + "Vec3: x=" + std::to_string(vec.x) + ", y=" + std::to_string(vec.y) + ", z=" + std::to_string(vec.z) + "\n";
		OutputDebugStringA(s.c_str());
		#endif // _DEBUG
	}

	inline void printString(std::string prefix) {
		#ifdef DEBUG
		prefix += "\n";
		OutputDebugStringA(prefix.c_str());
		#endif // _DEBUG
	}

	inline void printXMVECTOR(DirectX::XMVECTOR vec, std::string prefix = "") {
		#ifdef DEBUG
		std::string s = prefix + "Vec3: x=" + std::to_string(vec.m128_f32[0]) + ", y=" + std::to_string(vec.m128_f32[1]) + ", z=" + std::to_string(vec.m128_f32[2]) + "\n";
		OutputDebugStringA(s.c_str());
		#endif // _DEBUG
	}

	inline void printTime(clock_t time, std::string prefix = "") {
		#ifdef DEBUG
		std::string s = prefix + "Time: " + std::to_string(float(time) / CLOCKS_PER_SEC) + "\n";
		OutputDebugStringA(s.c_str());
		#endif // _DEBUG
	}

	inline void printInt(int _int, std::string prefix = "") {
		#ifdef DEBUG
		std::string s = prefix + std::to_string(_int) + "\n";
		OutputDebugStringA(s.c_str());
		#endif // _DEBUG
	}

	inline void printFloat(float _float, std::string prefix = "") {
		#ifdef DEBUG
		std::string s = prefix + std::to_string(_float) + "\n";
		OutputDebugStringA(s.c_str());
		#endif // _DEBUG
	}

	inline void printbtVector(btVector3& vec, std::string prefix = "") {
		#ifdef DEBUG
		std::string s = prefix + "btVec: x=" + std::to_string(vec.x()) + ", y=" + std::to_string(vec.y()) + ", z=" + std::to_string(vec.z()) + "\n";
		OutputDebugStringA(s.c_str());
		#endif // DEBUG
	}

	inline void printOrigin(btRigidBody* rigidBody, std::string prefix = "") {
		#ifdef DEBUG
		using namespace DirectX;
		btTransform transform;
		rigidBody->getMotionState()->getWorldTransform(transform);
		Debug::printbtVector(transform.getOrigin(), prefix);
		#endif // _DEBUG
	}
}
