#pragma once
#include "pch.h"

class Pyramid : public Object<DirectX::VertexPositionColor> {
public:
	Pyramid(std::shared_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> batch, DirectX::XMVECTOR startPoint, float size);

	virtual void render();
	virtual void printToOutput();
};
