#pragma once
#include "pch.h"

class Cube : public Object<DirectX::VertexPositionColor> {
public:
	Cube(std::shared_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> batch, DirectX::XMVECTOR startPoint, float size);
	virtual void render();
	virtual void printToOutput();
};
