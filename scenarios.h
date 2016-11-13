#pragma once
#include "pch.h"
#include "Plank.h"

namespace Scenario {
	void boxTowers(std::vector<std::shared_ptr<Object<TEXTURED_VERTEX_TYPE>>> &shapes, btDiscreteDynamicsWorld* world, std::shared_ptr<DirectX::PrimitiveBatch<TEXTURED_VERTEX_TYPE>> batch);
	void plankDomino(std::vector<std::shared_ptr<Object<TEXTURED_VERTEX_TYPE>>> &shapes, btDiscreteDynamicsWorld* world, std::shared_ptr<DirectX::PrimitiveBatch<TEXTURED_VERTEX_TYPE>> batch);
	void houseOfCards(std::vector<std::shared_ptr<Object<TEXTURED_VERTEX_TYPE>>> &shapes, btDiscreteDynamicsWorld* world, std::shared_ptr<DirectX::PrimitiveBatch<TEXTURED_VERTEX_TYPE>> batch);
}
