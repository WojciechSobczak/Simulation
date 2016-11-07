#include "pch.h"
#include "Simulation.h"
#define MOVE_MULTIPLYER 4.0f

void Simulation::resetViewMatrix() {
	using namespace DirectX;


	XMMATRIX yawMatrix = DirectX::XMMatrixRotationAxis(cameraUp, yaw); //Najpierw obr�t wok� osi Y
	XMVECTOR rotatedLook = XMVector3TransformCoord(cameraLook, yawMatrix); //obr�cona wok� Y
	XMVECTOR rotatedRight = XMVector3TransformCoord(cameraRight, yawMatrix); //obr�cona wok� Y
	//nieobr�cona w pozostaje up

	////Teraz obracamy wok� osi X
	XMMATRIX pitchMatrix = DirectX::XMMatrixRotationAxis(rotatedRight, pitch);
	rotatedLook = DirectX::XMVector3TransformCoord(rotatedLook, pitchMatrix);
	//MAMY DZIA�AJ�CE OBRACANIE, Teraz przesuni�cie

	if (moveForward) {
		XMVECTOR move = {0.0f, 0.0f, moveForward, 0.0f};
		move = XMVector3TransformCoord(move, yawMatrix);
		observerPosition += move * MOVE_MULTIPLYER;
	} 

	if (moveRight) {
		XMVECTOR move = {moveRight, 0.0f, 0.0f, 0.0f};
		move = XMVector3TransformCoord(move, yawMatrix);
		observerPosition += move * MOVE_MULTIPLYER;
	}

	if (moveUp) {
		XMVECTOR_Y(observerPosition) += moveUp * MOVE_MULTIPLYER;
	}

	rotatedLook += observerPosition;
	viewMatrix = DirectX::XMMatrixLookAtLH(observerPosition, rotatedLook, cameraUp);
	coloredBasicEffect->SetView(viewMatrix);
	#ifdef TEXTURES_ENABLED
	texturedBasicEffect->SetView(viewMatrix);
	#endif
#ifdef CAMERA_DEBUG
	Debug::printXMVECTOR(rotatedLook, "Look");
#endif // CAMERA_DEBUG
	moveForward = 0;
	moveRight = 0;
	moveUp = 0;
}