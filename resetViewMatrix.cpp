#include "pch.h"
#include "Simulation.h"
#define MOVE_MULTIPLYER 4.0f

void Simulation::resetViewMatrix() {
	using namespace DirectX;


	XMMATRIX yawMatrix = DirectX::XMMatrixRotationAxis(cameraUp, yaw); //Najpierw obrót wokó³ osi Y
	XMVECTOR rotatedLook = XMVector3TransformCoord(cameraLook, yawMatrix); //obrócona wokó³ Y
	XMVECTOR rotatedRight = XMVector3TransformCoord(cameraRight, yawMatrix); //obrócona wokó³ Y
	//nieobrócona w pozostaje up

	////Teraz obracamy wokó³ osi X
	XMMATRIX pitchMatrix = DirectX::XMMatrixRotationAxis(rotatedRight, pitch);
	rotatedLook = DirectX::XMVector3TransformCoord(rotatedLook, pitchMatrix);
	//XMVECTOR primalUp = DirectX::XMVector3TransformCoord(cameraUp, pitchMatrix);
	//MAMY DZIA£AJ¥CE OBRACANIE, Teraz przesuniêcie

	if (moveForward) {
		XMVECTOR move = {0.0f, 0.0f, moveForward, 0.0f};
		move = XMVector3TransformCoord(move, yawMatrix);
		position += move * MOVE_MULTIPLYER;
	} 

	if (moveRight) {
		XMVECTOR move = {moveRight, 0.0f, 0.0f, 0.0f};
		move = XMVector3TransformCoord(move, yawMatrix);
		position += move * MOVE_MULTIPLYER;
	}

	if (moveUp) {
		XMVECTOR_Y(position) += moveUp * MOVE_MULTIPLYER;
	}

	rotatedLook += position;
	basicEffect->SetView(DirectX::XMMatrixLookAtLH(position, rotatedLook, cameraUp));
#ifdef CAMERA_DEBUG
	Debug::printXMVECTOR(rotatedLook, "Look");
#endif // CAMERA_DEBUG
	moveForward = 0;
	moveRight = 0;
	moveUp = 0;
}