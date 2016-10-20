#pragma once
#include "pch.h"


class Object {
public:
	template<typename T>
	Object(std::shared_ptr<DirectX::PrimitiveBatch<T>> batch);

	std::vector<UINT> indicies;
	std::vector<DirectX::SimpleMath::Vector3> verticies;

	virtual void render();
	virtual void printToOutput();
};
