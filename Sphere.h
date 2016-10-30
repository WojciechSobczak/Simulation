#pragma once
#include "pch.h"

class Sphere : Object<DirectX::VertexPositionColor> {
public:
	Sphere(std::shared_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> batch, DirectX::XMVECTOR startPoint, float size, float divider);
	virtual void render();
	virtual void printToOutput();
};
