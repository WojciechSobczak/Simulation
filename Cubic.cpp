#include "pch.h"
#include "Cubic.h"
using namespace DirectX;

Cubic::Cubic(std::shared_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> batch, DirectX::XMVECTOR startPoint, float size) {
	this->batch = batch;
	this->startPoint = startPoint;
	this->size = size;
}

void Cubic::render() {
	DirectX::VertexPositionColor vertices[8];
	for (int i = 0; i < 8; i += 4) {
		vertices[i] = DirectX::VertexPositionColor(startPoint, DirectX::Colors::Aqua);
		XMVECTOR_Z(startPoint) = XMVECTOR_Z(startPoint) + size;
		vertices[i + 1] = DirectX::VertexPositionColor(startPoint, DirectX::Colors::Black);
		XMVECTOR_X(startPoint) = XMVECTOR_X(startPoint) + size;
		vertices[i + 2] = DirectX::VertexPositionColor(startPoint, DirectX::Colors::Yellow);
		XMVECTOR_Z(startPoint) = XMVECTOR_Z(startPoint) - size;
		vertices[i + 3] = DirectX::VertexPositionColor(startPoint, DirectX::Colors::Red);
		XMVECTOR_X(startPoint) = XMVECTOR_X(startPoint) - size;
		XMVECTOR_Y(startPoint) = XMVECTOR_Y(startPoint) + size;
	}

	const uint16_t sideWalls[10] = {
		3, 7, 0, 4, 1, 5, 2, 6, 3, 7
	};
	const uint16_t upWall[10] = {
		7,6,4,5
	};
	const uint16_t downWall[10] = {
		2, 3, 1, 0
	};
	batch->DrawIndexed(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP, sideWalls, _countof(sideWalls), vertices, _countof(vertices));
	batch->DrawIndexed(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP, upWall, _countof(upWall), vertices, _countof(vertices));
	batch->DrawIndexed(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP, downWall, _countof(downWall), vertices, _countof(vertices));
}

void Cubic::printToOutput() {}
