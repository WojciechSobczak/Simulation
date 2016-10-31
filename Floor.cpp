#include "pch.h"
#include "Floor.h"


Floor::Floor(std::shared_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> batch, float size) : Object(batch, {0, 0, 0, 0}, size) {

	btTransform groundTransform;
	groundTransform.setIdentity();
	groundTransform.setOrigin(btVector3(0, 0, 0)); //Nie mam pojêcia czemu -200 ale spoko
	this->mass = 0;
	btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), 0);
	btDefaultMotionState* groundMotionState = new btDefaultMotionState(groundTransform);
	btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(mass, groundMotionState, groundShape, btVector3(0, 0, 0));
	btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);
	groundRigidBody->setRestitution(1);
	groundRigidBody->setFriction(1);
	groundRigidBody->setActivationState(DISABLE_DEACTIVATION);
	groundRigidBody->setUserIndex(-1);
	this->rigidBody = groundRigidBody;
}

void Floor::render() {
	using namespace DirectX;
	size_t divisions = size / 50;
	Vector3 origin = Vector3::Zero;
	Vector3 xaxis(size, 0.f, 0.f);
	Vector3 yaxis(0.f, 0.f, size);
	for (size_t i = 0; i <= divisions; ++i) {
		float fPercent = float(i) / float(divisions);
		fPercent = (fPercent * 2.0f) - 1.0f;

		Vector3 scale = xaxis * fPercent + origin;

		VertexPositionColor v1(scale - yaxis, DirectX::Colors::LightSlateGray);
		VertexPositionColor v2(scale + yaxis, DirectX::Colors::LightSlateGray);
		batch->DrawLine(v1, v2);
	}

	for (size_t i = 0; i <= divisions; i++) {
		float fPercent = float(i) / float(divisions);
		fPercent = (fPercent * 2.0f) - 1.0f;

		Vector3 scale = yaxis * fPercent + origin;

		VertexPositionColor v1(scale - xaxis, DirectX::Colors::LightSlateGray);
		VertexPositionColor v2(scale + xaxis, DirectX::Colors::LightSlateGray);
		batch->DrawLine(v1, v2);
	}
}

void Floor::printToOutput() {}
