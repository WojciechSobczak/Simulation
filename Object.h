#pragma once
#include "pch.h"

#define XM_VEC_FROM_BT_QUATERNION(q) 

template<class T>
class Object {
public:

	DirectX::XMVECTOR startPoint;
	const uint16_t* indices;
	const T* vertices;
	float size;
	float mass = 50;
	std::shared_ptr<DirectX::PrimitiveBatch<T>> batch;
	std::vector<DirectX::XMVECTOR> points;
	btRigidBody* rigidBody;
	Object(std::shared_ptr<DirectX::PrimitiveBatch<T>> batch, DirectX::XMVECTOR startPoint, float size) : batch(batch), startPoint(startPoint), size(size) {}

	virtual void render() = 0;
	virtual void printToOutput() = 0;
	virtual void registerCollisionObject(btDiscreteDynamicsWorld* world) {
		world->addRigidBody(this->rigidBody);
	}

};
