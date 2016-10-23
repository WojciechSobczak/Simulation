#pragma once
#include "pch.h"

class Cubic : Object {
public:
	DirectX::XMVECTOR startPoint;
	const uint16_t* indices;
	const DirectX::VertexPositionColor* vertices;
	float size;
	std::shared_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> batch;
	Cubic(std::shared_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> batch, DirectX::XMVECTOR startPoint, float size);
	virtual void render();
	virtual void printToOutput();
};
