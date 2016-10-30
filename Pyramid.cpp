#include "pch.h"
#include "Pyramid.h"

const uint16_t Pyramid::indicies[] = {
	3, 4, 0,
	2, 4, 3,
	1, 4, 2,
	0, 4, 1,
	2, 3, 0,
	2, 0, 1
};



using namespace DirectX;
Pyramid::Pyramid(std::shared_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> batch, DirectX::XMVECTOR startPoint, float size) : Object(batch, startPoint, size) {

	btConvexHullShape * shape = new btConvexHullShape();
	//btTriangleMesh* mesh = new btTriangleMesh();
	points = std::vector<XMVECTOR>(5);
	VertexPositionColor vertices[5];
	XMVECTOR startPointCopy ={0,0,0,0};
	points[0] = startPointCopy;
	XMVECTOR_Z(startPointCopy) = XMVECTOR_Z(startPointCopy) + size;
	points[1] = startPointCopy;
	XMVECTOR_X(startPointCopy) = XMVECTOR_X(startPointCopy) + size;
	points[2] = startPointCopy;
	XMVECTOR_Z(startPointCopy) = XMVECTOR_Z(startPointCopy) - size;
	points[3] = startPointCopy;
	XMVECTOR_X(startPointCopy) = XMVECTOR_X(startPointCopy) - size / 2;
	XMVECTOR_Z(startPointCopy) = XMVECTOR_Z(startPointCopy) + size / 2;
	XMVECTOR_Y(startPointCopy) = XMVECTOR_Y(startPointCopy) + sin(M_PI / 3) * size;
	points[4] = startPointCopy;

	/*for (size_t i = 0; i < _countof(indicies); i += 3) {
		mesh->addTriangle(
			XMVECTOR_TO_BTVECTOR(points[indicies[i]]), 
			XMVECTOR_TO_BTVECTOR(points[indicies[i + 1]]), 
			XMVECTOR_TO_BTVECTOR(points[indicies[i + 2]])
		);
	}*/
	for (size_t i = 0; i < points.size(); i += 3) {
		shape->addPoint(XMVECTOR_TO_BTVECTOR(points[i]));
	}
	btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), XMVECTOR_TO_BTVECTOR(startPoint)));
	btVector3 inertia(0, 0, 0);
	shape->calculateLocalInertia(mass, inertia);
	btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(
		mass,                  // mass
		motionState,        // initial position
		shape,              // collision shape of body
		inertia    // local inertia
	);
	rigidBody = new btRigidBody(rigidBodyCI);
};

void Pyramid::render() {

	std::vector<XMVECTOR> newpoints = this->relocatePoints();
	VertexPositionColor vertices[5];
	vertices[0] = DirectX::VertexPositionColor(newpoints[0], DirectX::Colors::Aqua);
	vertices[1] = DirectX::VertexPositionColor(newpoints[1], DirectX::Colors::Black);
	vertices[2] = DirectX::VertexPositionColor(newpoints[2], DirectX::Colors::Yellow);
	vertices[3] = DirectX::VertexPositionColor(newpoints[3], DirectX::Colors::Red);
	vertices[4] = DirectX::VertexPositionColor(newpoints[4], DirectX::Colors::Green);

	batch->DrawIndexed(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
							  indicies,
							  _countof(indicies),
							  vertices,
							  _countof(vertices)
	);
}

void Pyramid::printToOutput() {

}
