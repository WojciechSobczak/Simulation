#pragma once
#include "pch.h"

template<class T>
class Object {
public:

	DirectX::XMVECTOR startPoint;
	const uint16_t* indices;
	const T* vertices;
	float size;
	float mass = 1;
	std::shared_ptr<DirectX::PrimitiveBatch<T>> batch;
	std::vector<DirectX::XMVECTOR> points;
	btRigidBody* rigidBody;
	//Cache
	btTransform cachedTransform;
	std::vector<DirectX::XMVECTOR> cachedPoints;
	//End Cache
	Object(std::shared_ptr<DirectX::PrimitiveBatch<T>> batch, DirectX::XMVECTOR startPoint, float size) : batch(batch), startPoint(startPoint), size(size) {}

	virtual void render() = 0;
	virtual void printToOutput() = 0;
	virtual void registerCollisionObject(btDiscreteDynamicsWorld* world) {
		world->addRigidBody(this->rigidBody);
	}

	virtual void unregisterCollisionObject(btDiscreteDynamicsWorld* world) {
		world->removeRigidBody(this->rigidBody);
	}

	virtual void relocatePoints() {
		using namespace DirectX;
		btTransform transform = this->rigidBody->getCenterOfMassTransform();
		if (transform == cachedTransform) {
			return;
		}
		for (int i = 0; i < points.size(); i++) {
			btVector3 vec = XMVECTOR_TO_BTVECTOR(points[i]);
			vec = transform * vec;
			cachedPoints[i] = vec.get128();
		}
		cachedTransform = transform;
	}
};
