#pragma once
#include "pch.h"
using namespace DirectX;

class Box : public Object<TEXTURED_VERTEX_TYPE> {
public:
	Box(std::shared_ptr<DirectX::PrimitiveBatch<TEXTURED_VERTEX_TYPE>> batch, XMVECTOR startPoint, float width, float height, float depth, XMVECTOR yawPitchRoll = {0,0,0,0}, float mass = 2);
	virtual void render();
	virtual void printToOutput();

	static const uint16_t sideWalls[];
	static const uint16_t upWall[];
	static const uint16_t downWall[];
};
