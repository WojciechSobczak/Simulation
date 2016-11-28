#include "pch.h"
#include "Plank.h"

void Scenario::boxTowers(std::vector<std::shared_ptr<Object<TEXTURED_VERTEX_TYPE>>> &shapes, btDiscreteDynamicsWorld* world, std::shared_ptr<DirectX::PrimitiveBatch<TEXTURED_VERTEX_TYPE>> batch) {
		
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



void Scenario::plankDomino(std::vector<std::shared_ptr<Object<TEXTURED_VERTEX_TYPE>>> &shapes, btDiscreteDynamicsWorld* world, std::shared_ptr<DirectX::PrimitiveBatch<TEXTURED_VERTEX_TYPE>> batch) {
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

void Scenario::houseOfCards(std::vector<std::shared_ptr<Object<TEXTURED_VERTEX_TYPE>>> &shapes, btDiscreteDynamicsWorld* world, std::shared_ptr<DirectX::PrimitiveBatch<TEXTURED_VERTEX_TYPE>> batch) {
	float spaceBetween = Plank::height;
	int elementsAmount = 16;
	int startSize = shapes.size();
	XMVECTOR yawPitchRoll = {M_PI_2, 0, 0, 0};
	XMVECTOR start = { -((float(elementsAmount)/2) * spaceBetween), Plank::height/2,0,0 };
	XMVECTOR cachedStart = start;

	for (int level = 0; level < 5; level++) {
		float heightStep = level * (Plank::height + Plank::depth);
		float elements = elementsAmount - level * 2;
		for (int i = 0; i < elements; i++) {
			yawPitchRoll = {M_PI_2, 0, 0, 0};
			std::shared_ptr<Plank> plank = std::make_shared<Plank>(batch, start, yawPitchRoll);
			shapes.push_back(plank);

			if (i != elements - 1) {
				float displacement = Plank::height / 2;
				XMVECTOR_X(start) += displacement;
				XMVECTOR_Y(start) += displacement + Plank::depth/2;
				yawPitchRoll = {0, M_PI_2, M_PI_2, 0};
				std::shared_ptr<Plank> roof = std::make_shared<Plank>(batch, start, yawPitchRoll);
				shapes.push_back(roof);
				XMVECTOR_X(start) -= displacement;
				XMVECTOR_Y(start) -= displacement + Plank::depth/2;;
			}
			XMVECTOR_X(start) += spaceBetween;
		}
		XMVECTOR_X(start) = -((float(elementsAmount - (level + 1)*2) / 2) * spaceBetween);
		XMVECTOR_Y(start) += Plank::height + Plank::depth;
	}
		

	for (int i = startSize; i < shapes.size(); i++) {
		shapes[i]->registerCollisionObject(world);
	}
}
