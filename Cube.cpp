#include "pch.h"
#include "Cube.h"
using namespace DirectX;


Cube::Cube(std::shared_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> batch, DirectX::XMVECTOR startPoint, float size) : Object(batch, startPoint, size) {
	//XMVECTOR_X(startPoint), XMVECTOR_Y(startPoint), XMVECTOR_Z(startPoint)
	btCollisionShape* colShape = new btBoxShape(btVector3(size, size, size));
	btTransform playerWorld;
	playerWorld.setIdentity();
	playerWorld.setOrigin(btVector3(XMVECTOR_X(startPoint), XMVECTOR_Y(startPoint), XMVECTOR_Z(startPoint)));
	btDefaultMotionState* motionState = new btDefaultMotionState(playerWorld);

	btVector3 inertia(0, 0, 0);
	colShape->calculateLocalInertia(mass, inertia);
	btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(
		mass,                  // mass
		motionState,        // initial position
		colShape,              // collision shape of body
		inertia    // local inertia
	);
	rigidBody = new btRigidBody(rigidBodyCI);
	rigidBody->setCollisionShape(colShape);
	rigidBody->setWorldTransform(playerWorld);

	points = std::vector<XMVECTOR>(8);
	for (int i = 0; i < 8; i += 4) {
		points[i] = startPoint;
		XMVECTOR_Z(startPoint) = XMVECTOR_Z(startPoint) + size;
		points[i + 1] = startPoint;
		XMVECTOR_X(startPoint) = XMVECTOR_X(startPoint) + size;
		points[i + 2] = startPoint;
		XMVECTOR_Z(startPoint) = XMVECTOR_Z(startPoint) - size;
		points[i + 3] = startPoint;
		XMVECTOR_X(startPoint) = XMVECTOR_X(startPoint) - size;
		XMVECTOR_Y(startPoint) = XMVECTOR_Y(startPoint) + size;
	}
}

void Cube::render() {
	btTransform transform;
	rigidBody->getMotionState()->getWorldTransform(transform);
	XMVECTOR translation = transform.getOrigin().get128();
	XMVECTOR rotationQuaterion = transform.getRotation().get128();
	btScalar s = transform.getOrigin().getY();
	

	//for (int i = 0; i < points.size(); i++) {
		/*XMVECTOR one = {1, 1, 1, 1};
		XMMATRIX m = DirectX::XMMatrixAffineTransformation(one, startPoint, rotationQuaterion, translation);
		XMVECTOR newpos = DirectX::XMVector3TransformCoord(startPoint, m);
		OutputDebugStringA(("BUL: " + std::to_string(s) + "\n").c_str());
		OutputDebugStringA(("COM: " + std::to_string(XMVECTOR_Y(newpos)) + "\n").c_str());*/

	//}

	DirectX::VertexPositionColor vertices[8];
	for (int i = 0; i < 8; i += 4) {
		vertices[i] = DirectX::VertexPositionColor(points[i], DirectX::Colors::Aqua);
		vertices[i + 1] = DirectX::VertexPositionColor(points[i + 1], DirectX::Colors::Black);
		vertices[i + 2] = DirectX::VertexPositionColor(points[i + 2], DirectX::Colors::Yellow);
		vertices[i + 3] = DirectX::VertexPositionColor(points[i + 3], DirectX::Colors::Red);
	}

	const uint16_t sideWalls[] = {
		3, 7, 0, 4, 1, 5, 2, 6, 3, 7
	};
	const uint16_t upWall[] = {
		7,6,4,5
	};
	const uint16_t downWall[] = {
		2, 3, 1, 0
	};
	batch->DrawIndexed(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP, sideWalls, _countof(sideWalls), vertices, _countof(vertices));
	batch->DrawIndexed(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP, upWall, _countof(upWall), vertices, _countof(vertices));
	batch->DrawIndexed(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP, downWall, _countof(downWall), vertices, _countof(vertices));
}

void Cube::printToOutput() {}
