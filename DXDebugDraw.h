#pragma once
#include "pch.h"

using namespace DirectX;
class DXDebugDraw : public btIDebugDraw {
	int m_debugMode;

public:

	std::shared_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> coloredBatch;

	DXDebugDraw(std::shared_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> coloredBatch) {
		this->coloredBatch = coloredBatch;
	}
	virtual ~DXDebugDraw() {};

	virtual void   drawLine(const btVector3& from, const btVector3& to, const btVector3& fromColor, const btVector3& toColor) {
		OutputDebugStringA("CallLine");
		coloredBatch->DrawLine(
			VertexPositionColor(from.get128(), fromColor.get128()),
			VertexPositionColor(to.get128(), toColor.get128())
		);
	}

	virtual void   drawLine(const btVector3& from, const btVector3& to, const btVector3& color) {
		OutputDebugStringA("CallLineB");
		coloredBatch->DrawLine(
			VertexPositionColor(from.get128(), color.get128()),
			VertexPositionColor(to.get128(), color.get128())
		);
	}

	virtual void   drawSphere(const btVector3& p, btScalar radius, const btVector3& color) {
		OutputDebugStringA("CallSphere");
	}

	virtual void   drawTriangle(const btVector3& a, const btVector3& b, const btVector3& c, const btVector3& color, btScalar alpha) {
		OutputDebugStringA("CallTriangle");
	}

	virtual void   drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color) {
		OutputDebugStringA("CallContactPoint");
	}

	virtual void   reportErrorWarning(const char* warningString) {
		OutputDebugStringA(warningString);
	}

	virtual void draw3dText(const btVector3& location, const char* textString) {
		OutputDebugStringA("draw3dText");
	}

	virtual void   setDebugMode(int debugMode) {
		this->m_debugMode = debugMode;
	}

	virtual int getDebugMode() const { return m_debugMode; }

};