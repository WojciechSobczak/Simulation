#pragma once
#include "pch.h"
#include "Object.h"
using namespace DirectX;
class Plank : public Box {
public:
	static const float width, height, depth;
	Plank(std::shared_ptr<PrimitiveBatch<TEXTURED_VERTEX_TYPE>> batch, XMVECTOR startPoint, XMVECTOR yawPitchRoll = {0,0,0,0}, float mass = 20) : Box(batch, startPoint, Plank::width, Plank::height, Plank::depth, yawPitchRoll, mass) {};
};

