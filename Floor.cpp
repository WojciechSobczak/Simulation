#include "pch.h"
#include "Floor.h"


Floor::Floor(std::shared_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> batch, float size) : Object(batch, {0, 0, 0, 0}, size) {
}

void Floor::render() {
	using namespace DirectX;
	size_t divisions = 20;
	Vector3 origin = Vector3::Zero;
	Vector3 xaxis(size, 0.f, 0.f);
	Vector3 yaxis(0.f, 0.f, 500.f);
	for (size_t i = 0; i <= divisions; ++i) {
		float fPercent = float(i) / float(divisions);
		fPercent = (fPercent * 2.0f) - 1.0f;

		Vector3 scale = xaxis * fPercent + origin;

		VertexPositionColor v1(scale - yaxis, DirectX::Colors::White);
		VertexPositionColor v2(scale + yaxis, DirectX::Colors::White);
		batch->DrawLine(v1, v2);
	}

	for (size_t i = 0; i <= divisions; i++) {
		float fPercent = float(i) / float(divisions);
		fPercent = (fPercent * 2.0f) - 1.0f;

		Vector3 scale = yaxis * fPercent + origin;

		VertexPositionColor v1(scale - xaxis, DirectX::Colors::White);
		VertexPositionColor v2(scale + xaxis, DirectX::Colors::White);
		batch->DrawLine(v1, v2);
	}
}

void Floor::printToOutput() {}
