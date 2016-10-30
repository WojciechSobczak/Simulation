#pragma once
#include "pch.h"

class Pyramid : public Object<DirectX::VertexPositionColor> {
private:
	btTriangleMesh* meshInterface;
	static const uint16_t indicies[];
public:
	Pyramid(std::shared_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> batch, DirectX::XMVECTOR startPoint, float size);

	virtual void render();
	virtual void printToOutput();
};
