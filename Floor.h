#pragma once
#include "pch.h"

class Floor : public Object<DirectX::VertexPositionColor> {
public:
	Floor(std::shared_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> batch, float size);
	virtual void render();
	virtual void printToOutput();
};
