#include "pch.h"
#include "Pyramid.h"


using namespace DirectX;
Pyramid::Pyramid(std::shared_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> batch, DirectX::XMVECTOR startPoint, float size) : Object(batch, startPoint, size) {
	this->points = std::vector<XMVECTOR>(5);
	VertexPositionColor vertices[5];
	points[0] = startPoint;
	XMVECTOR_Z(startPoint) = XMVECTOR_Z(startPoint) + size;
	points[1] = startPoint;
	XMVECTOR_X(startPoint) = XMVECTOR_X(startPoint) + size;
	points[2] = startPoint;
	XMVECTOR_Z(startPoint) = XMVECTOR_Z(startPoint) - size;
	points[3] = startPoint;
	XMVECTOR_X(startPoint) = XMVECTOR_X(startPoint) - size / 2;
	XMVECTOR_Z(startPoint) = XMVECTOR_Z(startPoint) + size / 2;
	XMVECTOR_Y(startPoint) = XMVECTOR_Y(startPoint) + sin(M_PI / 3) * size;
	points[4] = startPoint;
};

void Pyramid::render() {

	VertexPositionColor vertices[5];
	vertices[0] = DirectX::VertexPositionColor(points[0], DirectX::Colors::Aqua);
	vertices[1] = DirectX::VertexPositionColor(points[1], DirectX::Colors::Black);
	vertices[2] = DirectX::VertexPositionColor(points[2], DirectX::Colors::Yellow);
	vertices[3] = DirectX::VertexPositionColor(points[3], DirectX::Colors::Red);
	vertices[4] = DirectX::VertexPositionColor(points[4], DirectX::Colors::Green);

	const uint16_t indicies[] = {
		3, 4, 0,
		2, 4, 3,
		1, 4, 2,
		0, 4, 1,
		2, 3, 0,
		2, 0, 1
	};

	batch->DrawIndexed(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
							  indicies,
							  _countof(indicies),
							  vertices,
							  _countof(vertices)
	);
}

void Pyramid::printToOutput() {

}
