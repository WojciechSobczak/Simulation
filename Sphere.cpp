#include "pch.h"
#include "Sphere.h"


using namespace DirectX;
Sphere::Sphere(std::shared_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> batch, DirectX::XMVECTOR startPoint, float size, size_t divider = 10) : Object(batch, startPoint, size) {
	btTransform groundTransform;
	groundTransform.setIdentity();
	groundTransform.setOrigin(XMVECTOR_TO_BTVECTOR(startPoint));

	float radius = size / 2;

	btSphereShape* shape = new btSphereShape(radius);
	btDefaultMotionState* sphereMotionShape = new btDefaultMotionState(groundTransform);
	btRigidBody::btRigidBodyConstructionInfo sphereRigidBodyCI(mass, sphereMotionShape, shape, btVector3(0, 0, 0));
	btRigidBody* sphereRigidBody = new btRigidBody(sphereRigidBodyCI);
	sphereRigidBody->setRestitution(3.0f);
	sphereRigidBody->setFriction(1.9f);
	sphereRigidBody->setRollingFriction(5.f);
	sphereRigidBody->setActivationState(DISABLE_DEACTIVATION);
	this->rigidBody = sphereRigidBody;
	this->points = std::vector<XMVECTOR>();

	XMVECTOR origin = {0, radius, 0, 0};
	points.push_back(origin);

	assert(divider > 2);
	//Tymczasowo zostawiam limit, poprawiæ jak bêdzie na to czas TODO
	assert(divider < 46);
	this->divider = divider;
	float step = radius / divider;
	std::vector<XMVECTOR> circlePoints = std::vector<XMVECTOR>(divider);
	for (size_t i = 0; i < divider; i++) {
		float angle = ((2 * M_PI) / float(divider)) * i;
		XMMATRIX rotationMatrix = XMMatrixRotationAxis({0, 0, 1, 0}, angle);
		XMVECTOR transformedPoint = XMVector3Transform(origin, rotationMatrix);
		circlePoints[i] = transformedPoint;
	}

	for (size_t circleIndex = 0; circleIndex < divider; circleIndex++) {
		float angle = ((2 * M_PI) / float(divider)) * circleIndex;
		for (size_t i = 0; i < divider; i++) {
			XMMATRIX rotationMatrix = XMMatrixRotationAxis({0, 1, 0, 0}, angle);
			XMVECTOR transformedPoint = XMVector3Transform(circlePoints[i], rotationMatrix);
			points.push_back(transformedPoint);
		}
	}
}

void Sphere::render() {
	std::vector<XMVECTOR> newPoints = relocatePoints();
	std::vector<VertexPositionColor> vertices = std::vector<VertexPositionColor>(newPoints.size());
	std::vector<uint16_t> indicies = std::vector<UINT16>(2 * newPoints.size());
	for (size_t i = 0; i < newPoints.size(); i++) {
		vertices[i] = VertexPositionColor(newPoints[i], Colors::GreenYellow);
	}

	int index = 0;
	for (size_t circleIndex = 0; circleIndex < divider; circleIndex++) {
		for (size_t i = 0; i < divider - 1; i++) {
			indicies[index++] = (((circleIndex + 1) * divider) + i) % newPoints.size();
			indicies[index++] = ((circleIndex * divider) + i) % newPoints.size();
		}
	}
#define SPHERE_DEBUG
#ifdef SPHERE_DEBUG
	for (size_t i = 0; i < indicies.size() - 1; i++) {
		batch->DrawLine(
			VertexPositionColor(newPoints[indicies[i]], Colors::Black),
			VertexPositionColor(newPoints[indicies[i + 1]], Colors::Black)
		);
	}
#endif // SPHERE_DEBUG


	//max 2048 vertexów na raz mo¿e iœæ
	//size_t verticesSize = vertices.size();
	//size_t indiciesSize = indicies.size();
	//for (int i = 0; i < 1/*ceil(float(verticesSize) / 2048)*/; i++) {
	/*	batch->DrawIndexed(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST,
						   &indicies[(i * 2048) % verticesSize],
						   ((i + 1) * verticesSize) % 2049,
						   &vertices[(i * 2048) % verticesSize],
						   ((i + 1) * verticesSize) % 2049
		);*/
	//}
	batch->DrawIndexed(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
		&indicies[0],
		indicies.size(),
		&vertices[0],
		vertices.size()
	);
	
}

void Sphere::printToOutput() {}