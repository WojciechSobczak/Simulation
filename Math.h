#pragma once
#include "pch.h"
#include "Math.h"

#define xmvecfn(vec, n) vec.m128_f32[n]

#define XMVECTOR_X(vec) xmvecfn(vec, 0)
#define XMVECTOR_Y(vec) xmvecfn(vec, 1)
#define XMVECTOR_Z(vec) xmvecfn(vec, 2)


namespace Math {


	/*void rotateVector(Vector3 &vec, float angle) {

		D3DMATRIX rotationMatrix;
		DirectX::SimpleMath::Matrix::CreateRotationX(angle);
		DirectX::SimpleMath::Matrix::CreateRotationY(angle);
		DirectX::SimpleMath::Matrix::CreateRotationZ(angle);


	}
};*/
}