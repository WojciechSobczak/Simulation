#pragma once
#include "pch.h"
#include "Plank.h"

namespace Scenario {
	void plankTowers(std::vector<std::shared_ptr<Object<TEXTURED_VERTEX_TYPE>>> &shapes, btDiscreteDynamicsWorld* world, std::shared_ptr<DirectX::PrimitiveBatch<TEXTURED_VERTEX_TYPE>> batch) {
		
		XMVECTOR start = {-Plank::height / 2, Plank::height / 2, Plank::height / 2 ,0};
		XMVECTOR cacheStart = start;
		int startSize = shapes.size();
		XMVECTOR yawPitchRoll;

		float objectHeight = 50;
		float radius = 250;

		for (int level = 0; level < 7; level++) {
			yawPitchRoll = {0,0,0,0};
			float floorHeight = level * (objectHeight);
			start = { radius, objectHeight / 2 ,radius ,0 };
			XMVECTOR_Y(start) += floorHeight;
			
			float columns = 15;
			for (int i = 0; i < columns; i++) {
				float angle = i * ((2*M_PI) / columns);
				XMVECTOR rotQuaterion = XMQuaternionRotationRollPitchYaw(0, angle, 0);
				XMVECTOR rotatedStart = XMVector3Rotate(start, rotQuaterion);
				std::shared_ptr<Cube> plank = std::make_shared<Cube>(batch, rotatedStart, objectHeight);
				shapes.push_back(plank);
			}

			start = cacheStart;
		}

		for (int i = startSize; i < shapes.size(); i++) {
			shapes[i]->registerCollisionObject(world);
		}
		
	}



	void plankDomino(std::vector<std::shared_ptr<Object<TEXTURED_VERTEX_TYPE>>> &shapes, btDiscreteDynamicsWorld* world, std::shared_ptr<DirectX::PrimitiveBatch<TEXTURED_VERTEX_TYPE>> batch) {
		float x = 0;
		float z = 0;
		int a = 20, b = 10;
		int startSize = shapes.size();
		XMVECTOR start = { 0, Plank::height / 2, 0, 0 };
		XMVECTOR yawPitchRoll = {0, 0, 0, 0};
		for (int i = 0; i < 150; i++) {
			float angle = 0.2 * i;
			x = (a + b * angle) * cos(angle);
			z = (a + b * angle) * sin(angle);
			XMVECTOR_X(yawPitchRoll) = -angle;
			XMVECTOR_X(start) = x;
			XMVECTOR_Z(start) = z;
			std::shared_ptr<Plank> plank = std::make_shared<Plank>(batch, start, yawPitchRoll);
			shapes.push_back(plank);
		}

		for (int i = startSize; i < shapes.size(); i++) {
			shapes[i]->registerCollisionObject(world);
		}
	}

	void houseOfCards(std::vector<std::shared_ptr<Object<TEXTURED_VERTEX_TYPE>>> &shapes, btDiscreteDynamicsWorld* world, std::shared_ptr<DirectX::PrimitiveBatch<TEXTURED_VERTEX_TYPE>> batch) {

	}

}
