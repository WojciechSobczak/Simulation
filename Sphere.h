#pragma once
#include "pch.h"

class Sphere : public Object<DirectX::VertexPositionColor> {
public:
	size_t divider;
	Sphere(std::shared_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> batch, DirectX::XMVECTOR startPoint, float size, size_t divider);
	virtual void render();
	virtual void printToOutput();
};
