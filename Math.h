#pragma once
#include "pch.h"
#include "Math.h"

#define xmvecfn(vec, n) vec.m128_f32[n]

#define XMVECTOR_X(vec) xmvecfn(vec, 0)
#define XMVECTOR_Y(vec) xmvecfn(vec, 1)
#define XMVECTOR_Z(vec) xmvecfn(vec, 2)
#define XMVECTOR_TO_BTVECTOR(vec) btVector3(XMVECTOR_X(vec), XMVECTOR_Y(vec), XMVECTOR_Z(vec))
#define BTVECTOR_TO_XMVECTOR(vec) vec.get128()
#define QUAT_VEC_FROM_BTQUAT(vec) {vec.get128().m128_f32[3], vec.get128().m128_f32[0], vec.get128().m128_f32[1], vec.get128().m128_f32[2]}
