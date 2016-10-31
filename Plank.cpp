#include "pch.h"
#include "Plank.h"
//Praktycznie to samo co cube, poszukaæ uwspólnieñ TODO

Plank::Plank(std::shared_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> batch, DirectX::XMVECTOR startPoint, float size) : Object(batch, startPoint, size) {
	btBoxShape* boxShape = new btBoxShape(btVector3(size / 8, size / 2, size / 16));
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

	rigidBody = new btRigidBody(rigidBodyCI);
	rigidBody->setRestitution(0.5);
	rigidBody->setActivationState(DISABLE_DEACTIVATION);
	rigidBody->setFriction(0.6);
	rigidBody->setUserIndex(-1);

	XMVECTOR origin = {-size / 8, size / 2, size / 16, 0};
	points = std::vector<XMVECTOR>(8);
	for (int i = 0; i < 8; i += 4) {
		points[i] = origin;
		XMVECTOR_X(origin) = XMVECTOR_X(origin) + (size / 4);
		points[i + 1] = origin;
		XMVECTOR_Z(origin) = XMVECTOR_Z(origin) - (size / 8);
		points[i + 2] = origin;
		XMVECTOR_X(origin) = XMVECTOR_X(origin) - (size / 4);
		points[i + 3] = origin;
		XMVECTOR_Z(origin) = XMVECTOR_Z(origin) + (size / 8);
		XMVECTOR_Y(origin) = XMVECTOR_Y(origin) - size;
	}
}


void Plank::render() {

	std::vector<XMVECTOR> newpoints = relocatePoints();
	DirectX::VertexPositionTexture vertices[8];
	vertices[0] = VertexPositionTexture(newpoints[0], {0, 50, 0, 0});



	//vertices[0] = DirectX::VertexPositionColor(newpoints[0], DirectX::Colors::Aqua);
	//vertices[1] = DirectX::VertexPositionColor(newpoints[1], DirectX::Colors::Black);
	//vertices[2] = DirectX::VertexPositionColor(newpoints[2], DirectX::Colors::Yellow);
	//vertices[3] = DirectX::VertexPositionColor(newpoints[3], DirectX::Colors::Red);
	//vertices[4] = DirectX::VertexPositionColor(newpoints[4], DirectX::Colors::Aqua);
	//vertices[5] = DirectX::VertexPositionColor(newpoints[5], DirectX::Colors::Black);
	//vertices[6] = DirectX::VertexPositionColor(newpoints[6], DirectX::Colors::Yellow);
	//vertices[7] = DirectX::VertexPositionColor(newpoints[7], DirectX::Colors::Red);

	/*batch->DrawIndexed(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP, Cube::sideWalls, _countof(Cube::sideWalls), vertices, _countof(vertices));
	batch->DrawIndexed(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP, Cube::upWall, _countof(Cube::upWall), vertices, _countof(vertices));
	batch->DrawIndexed(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP, Cube::downWall, _countof(Cube::downWall), vertices, _countof(vertices));*/
}

void Plank::printToOutput() {}
