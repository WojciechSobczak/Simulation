#include "pch.h"
#include "Cube.h"
using namespace DirectX;

const uint16_t Cube::sideWalls[] = {
	7, 3, 4, 0, 5, 1, 6, 2, 7, 3
};
const uint16_t Cube::upWall[] = {
	3, 2, 0, 1
};
const uint16_t Cube::downWall[] = {
	6, 7, 5, 4
};

Cube::Cube(std::shared_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> batch, DirectX::XMVECTOR startPoint, float size) : Object(batch, startPoint, size) {
	btBoxShape* boxShape = new btBoxShape(btVector3(size/2, size/2, size/2));
	btTransform playerWorld;
	btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), XMVECTOR_TO_BTVECTOR(startPoint)));

	btVector3 inertia(0, 0, 0);
	boxShape->calculateLocalInertia(mass, inertia);
	btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(
		mass,                  // mass
		motionState,        // initial position
		boxShape,              // collision shape of body
		inertia    // local inertia
	);

	this->mass = 20;
	rigidBody = new btRigidBody(rigidBodyCI);
	rigidBody->setRestitution(0.5);
	rigidBody->setActivationState(DISABLE_DEACTIVATION);
	rigidBody->setFriction(0.6);
	rigidBody->setUserIndex(-1);

	XMVECTOR origin = {-size / 2, size/2, size/2, 0};
	points = std::vector<XMVECTOR>(8);
	for (int i = 0; i < 8; i += 4) {
		points[i] = origin;
		XMVECTOR_X(origin) = XMVECTOR_X(origin) + size;
		points[i + 1] = origin;
		XMVECTOR_Z(origin) = XMVECTOR_Z(origin) - size;
		points[i + 2] = origin;
		XMVECTOR_X(origin) = XMVECTOR_X(origin) - size;
		points[i + 3] = origin;
		XMVECTOR_Z(origin) = XMVECTOR_Z(origin) + size;
		XMVECTOR_Y(origin) = XMVECTOR_Y(origin) - size;
	}
}

void Cube::render() {

	std::vector<XMVECTOR> newpoints = relocatePoints();
	DirectX::VertexPositionColor vertices[8];
	for (int i = 0; i < 8; i += 4) {
		vertices[i] = DirectX::VertexPositionColor(newpoints[i], DirectX::Colors::Aqua);
		vertices[i + 1] = DirectX::VertexPositionColor(newpoints[i + 1], DirectX::Colors::Black);
		vertices[i + 2] = DirectX::VertexPositionColor(newpoints[i + 2], DirectX::Colors::Yellow);
		vertices[i + 3] = DirectX::VertexPositionColor(newpoints[i + 3], DirectX::Colors::Red);
	}

	batch->DrawIndexed(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP, sideWalls, _countof(sideWalls), vertices, _countof(vertices));
	batch->DrawIndexed(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP, upWall, _countof(upWall), vertices, _countof(vertices));
	batch->DrawIndexed(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP, downWall, _countof(downWall), vertices, _countof(vertices));
}

void Cube::printToOutput() {}
