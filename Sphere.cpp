#include "pch.h"
#include "Sphere.h"


using namespace DirectX;
Sphere::Sphere(std::shared_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> batch, DirectX::XMVECTOR startPoint, float size, float divider = 10) : Object(batch, startPoint, size) {
	btTransform groundTransform;
	groundTransform.setIdentity();
	groundTransform.setOrigin(XMVECTOR_TO_BTVECTOR(startPoint));

	float radius = size / 2;

	btSphereShape* shape = new btSphereShape(radius);
	btDefaultMotionState* sphereMotionShape = new btDefaultMotionState(groundTransform);
	btRigidBody::btRigidBodyConstructionInfo sphereRigidBodyCI(mass, sphereMotionShape, shape, btVector3(0, 0, 0));
	btRigidBody* sphereRigidBody = new btRigidBody(sphereRigidBodyCI);
	sphereRigidBody->setRestitution(1);
	sphereRigidBody->setFriction(1);
	sphereRigidBody->setActivationState(DISABLE_DEACTIVATION);
	sphereRigidBody->setUserIndex(-1);
	this->rigidBody = sphereRigidBody;
	this->points = std::vector<XMVECTOR>();

	XMVECTOR origin = {0, radius, 0, 0};
	points.push_back(origin);

	assert(divider < 2);
	float step = radius / divider;
	for (size_t circleIndex = 0; circleIndex < divider; circleIndex++) {
		for (size_t i = 1; i < divider; i++) {
			if (i == divider / 2) {
				continue;
			}
			XMVECTOR nextPoint = origin;
			XMMATRIX rotationMatrix = XMMatrixRotationAxis({0, 1, 0, 0}, ((2 * M_PI) / divider) * i);
			XMVECTOR transformedPoint = XMVector3Transform(nextPoint, rotationMatrix);
			points.push_back(nextPoint);
		}
	}
}

void Sphere::render() {
	std::vector<XMVECTOR> newPoints = relocatePoints();
	VertexPositionColor* vertices = new VertexPositionColor[newPoints.size()];
	size_t* indicies = new size_t[newPoints.size()];
	for (size_t i = 0; i < newPoints.size(); i++) {

	}

}

void Sphere::printToOutput() {}