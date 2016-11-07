#pragma once
#include "pch.h"
#include "Box.h"

class Cube : public Box {
public:
	Cube(std::shared_ptr<DirectX::PrimitiveBatch<TEXTURED_VERTEX_TYPE>> batch, DirectX::XMVECTOR startPoint, float size, float mass = 1) : Box(batch, startPoint, size, size, size, {0,0,0,0}, mass) {};
};
