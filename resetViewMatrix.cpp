#include "pch.h"
#include "Simulation.h"

#define xmvecfn(vec, n) vec.m128_f32[n]

#define XMVECTOR_X(vec) xmvecfn(vec, 0)
#define XMVECTOR_Y(vec) xmvecfn(vec, 1)
#define XMVECTOR_Z(vec) xmvecfn(vec, 2)

void Simulation::resetViewMatrix() {
	using namespace DirectX;


	XMMATRIX yawMatrix = DirectX::XMMatrixRotationAxis(cameraUp, yaw); //Najpierw obr�t wok� osi Y
	XMVECTOR rotatedLook = XMVector3TransformCoord(cameraLook, yawMatrix); //obr�cona wok� Y
	XMVECTOR rotatedRight = XMVector3TransformCoord(cameraRight, yawMatrix); //obr�cona wok� Y
	//nieobr�cona w pozostaje up

	////Teraz obracamy wok� osi X
	XMMATRIX pitchMatrix = DirectX::XMMatrixRotationAxis(rotatedRight, pitch);
	rotatedLook = DirectX::XMVector3TransformCoord(rotatedLook, pitchMatrix);
	//XMVECTOR primalUp = DirectX::XMVector3TransformCoord(cameraUp, pitchMatrix);
	//MAMY DZIA�AJ�CE OBRACANIE, Teraz przesuni�cie


	rotatedLook += position;
	Debug::printXMVECTOR(rotatedLook, "Look");
	basicEffect->SetView(DirectX::XMMatrixLookAtLH(position, rotatedLook, cameraUp));
}