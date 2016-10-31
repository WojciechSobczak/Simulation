#pragma once
#include "pch.h"
#include "Object.h"
using namespace DirectX;
class Plank : public Object<VertexPositionTexture> {
public:
	Plank(std::shared_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> batch, DirectX::XMVECTOR startPoint, float size);

	virtual void render();
	virtual void printToOutput();

	static const uint16_t sideWalls[];
	static const uint16_t upWall[];
	static const uint16_t downWall[];
};

