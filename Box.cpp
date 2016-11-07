#include "pch.h"
#include "Box.h"
using namespace DirectX;

const uint16_t Box::sideWalls[] = {
	7, 3, 4, 0, 5, 1, 6, 2, 7, 3
};
const uint16_t Box::upWall[] = {
	3, 2, 0, 1
};
const uint16_t Box::downWall[] = {
	6, 7, 5, 4
};

Box::Box(std::shared_ptr<DirectX::PrimitiveBatch<TEXTURED_VERTEX_TYPE>> batch, XMVECTOR startPoint, float width, float height, float depth, XMVECTOR yawPitchRoll, float mass) : Object(batch, startPoint, width) {
	btBoxShape* boxShape = new btBoxShape(btVector3(width/2, height/2, depth/2));
	btTransform playerWorld;
	btQuaternion quaterion = btQuaternion(XMVECTOR_X(yawPitchRoll), XMVECTOR_Y(yawPitchRoll), XMVECTOR_Z(yawPitchRoll));
	btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(quaterion, XMVECTOR_TO_BTVECTOR(startPoint)));

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
	rigidBody->setFriction(0.6);
	rigidBody->setUserIndex(-1);

	XMVECTOR origin = {-width/2, height/2, depth/2};
	points = std::vector<XMVECTOR>(8);
	for (int i = 0; i < 8; i += 4) {
		points[i] = origin;
		XMVECTOR_X(origin) = XMVECTOR_X(origin) + width;
		points[i + 1] = origin;
		XMVECTOR_Z(origin) = XMVECTOR_Z(origin) - depth;
		points[i + 2] = origin;
		XMVECTOR_X(origin) = XMVECTOR_X(origin) - width;
		points[i + 3] = origin;
		XMVECTOR_Z(origin) = XMVECTOR_Z(origin) + depth;
		XMVECTOR_Y(origin) = XMVECTOR_Y(origin) - height;
	}

	cachedPoints = std::vector<XMVECTOR>(8);
	for (int i = 0; i < points.size(); i++) {
		cachedPoints[i] = points[i];
	}
}

void Box::render() {

	relocatePoints();
	
	#ifdef TEXTURES_ENABLED
	#ifdef LIGHTING_TEXTURES_ENABLED
		#define TEXTURE(x, tx, ty) cachedPoints[x], { tx, ty }
	#else
		#define TEXTURE(x, tx, ty) { tx, ty }
	#endif

	TEXTURED_VERTEX_TYPE vertices[24];
	//UP
	vertices[0] = TEXTURED_VERTEX_TYPE(cachedPoints[0], TEXTURE(0, 0.f, 0.f));
	vertices[1] = TEXTURED_VERTEX_TYPE(cachedPoints[1], TEXTURE(1, 1.0f, 0.f));
	vertices[2] = TEXTURED_VERTEX_TYPE(cachedPoints[2], TEXTURE(2, 1.0f, 1.0f));
	vertices[3] = TEXTURED_VERTEX_TYPE(cachedPoints[3], TEXTURE(3, 0.f, 1.0f));
	//DOWN
	vertices[4] = TEXTURED_VERTEX_TYPE(cachedPoints[7], TEXTURE(7, 0.f, 0.f));
	vertices[5] = TEXTURED_VERTEX_TYPE(cachedPoints[6], TEXTURE(6, 1.0f, 0.f));
	vertices[6] = TEXTURED_VERTEX_TYPE(cachedPoints[5], TEXTURE(5, 1.0f, 1.0f));
	vertices[7] = TEXTURED_VERTEX_TYPE(cachedPoints[4], TEXTURE(4, 0.f, 1.0f));
	//Front
	vertices[8] = TEXTURED_VERTEX_TYPE(cachedPoints[3], TEXTURE(3, 0.f, 0.f));
	vertices[9] = TEXTURED_VERTEX_TYPE(cachedPoints[2], TEXTURE(2, 1.0f, 0.f));
	vertices[10] = TEXTURED_VERTEX_TYPE(cachedPoints[6], TEXTURE(6, 1.0f, 1.0f));
	vertices[11] = TEXTURED_VERTEX_TYPE(cachedPoints[7], TEXTURE(7, 0.f, 1.0f));
	//Back
	vertices[12] = TEXTURED_VERTEX_TYPE(cachedPoints[1], TEXTURE(1, 0.f, 0.f));
	vertices[13] = TEXTURED_VERTEX_TYPE(cachedPoints[0], TEXTURE(0, 1.0f, 0.f));
	vertices[14] = TEXTURED_VERTEX_TYPE(cachedPoints[4], TEXTURE(4, 1.0f, 1.0f));
	vertices[15] = TEXTURED_VERTEX_TYPE(cachedPoints[5], TEXTURE(5, 0.f, 1.0f));
	//Left
	vertices[16] = TEXTURED_VERTEX_TYPE(cachedPoints[0], TEXTURE(0, 0.f, 0.f));
	vertices[17] = TEXTURED_VERTEX_TYPE(cachedPoints[3], TEXTURE(3, 1.0f, 0.f));
	vertices[18] = TEXTURED_VERTEX_TYPE(cachedPoints[7], TEXTURE(7, 1.0f, 1.0f));
	vertices[19] = TEXTURED_VERTEX_TYPE(cachedPoints[4], TEXTURE(4, 0.f, 1.0f));
	//Right
	vertices[20] = TEXTURED_VERTEX_TYPE(cachedPoints[2], TEXTURE(2, 0.f, 0.f));
	vertices[21] = TEXTURED_VERTEX_TYPE(cachedPoints[1], TEXTURE(1, 1.0f, 0.f));
	vertices[22] = TEXTURED_VERTEX_TYPE(cachedPoints[5], TEXTURE(5, 1.0f, 1.0f));
	vertices[23] = TEXTURED_VERTEX_TYPE(cachedPoints[6], TEXTURE(6, 0.f, 1.0f));

	static uint16_t indices[36] = {
		3, 2, 0, 2, 1, 0,			//UP
		7, 6, 4, 6, 5, 4,			//DOWN
		11, 10, 8, 10, 9, 8,		//FRONT
		15, 14, 12, 14, 13, 12,		//BACK
		19, 18, 16, 18, 17, 16,		//RIGHT
		23, 22, 20, 22, 21, 20		//LEFT
	};
	batch->DrawIndexed(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, indices, _countof(indices), vertices, _countof(vertices));
	#else
	TEXTURED_VERTEX_TYPE vertices[8];
	for (int i = 0; i < 8; i += 4) {
		vertices[i] = TEXTURED_VERTEX_TYPE(cachedPoints[i], DirectX::Colors::Aqua);
		vertices[i + 1] = TEXTURED_VERTEX_TYPE(cachedPoints[i + 1], DirectX::Colors::Black);
		vertices[i + 2] = TEXTURED_VERTEX_TYPE(cachedPoints[i + 2], DirectX::Colors::Yellow);
		vertices[i + 3] = TEXTURED_VERTEX_TYPE(cachedPoints[i + 3], DirectX::Colors::Red);
	}
	batch->DrawIndexed(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP, sideWalls, _countof(sideWalls), vertices, _countof(vertices));
	batch->DrawIndexed(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP, upWall, _countof(upWall), vertices, _countof(vertices));
	batch->DrawIndexed(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP, downWall, _countof(downWall), vertices, _countof(vertices));
	#endif
}

void Box::printToOutput() {}
