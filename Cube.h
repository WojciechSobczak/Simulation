#pragma once
#include "pch.h"

class Cube : public Object<DirectX::VertexPositionColor> {
public:
	Cube(std::shared_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> batch, DirectX::XMVECTOR startPoint, float size);
	virtual void render();
	virtual void printToOutput();

	static const uint16_t sideWalls[];
	static const uint16_t upWall[];
	static const uint16_t downWall[];
};
