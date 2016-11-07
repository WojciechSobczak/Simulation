#include "pch.h"
#include "Simulation.h"
#include "DXDebugDraw.h"
#include "Plank.h"
#include "scenarios.h"

using namespace DirectX;

// Initialize the Direct3D resources required to run.
void Simulation::initialize(HWND window, int width, int height) {
	windowHandler = window;
	windowWidth = std::max(width, 1);
	windowHeight = std::max(height, 1);
	createDevice();
	createResources();


	physicsBroadphase = new btDbvtBroadphase();
	physicsCollisionConfig = new btDefaultCollisionConfiguration();
	physicsCollisionDispatcher = new btCollisionDispatcher(physicsCollisionConfig);
	physicsSolver = new btSequentialImpulseConstraintSolver();
	bulletWorld = new btDiscreteDynamicsWorld(
		physicsCollisionDispatcher,
		physicsBroadphase,
		physicsSolver,
		physicsCollisionConfig
	);
	bulletWorld->setGravity(btVector3(0, -10, 0));
	DXDebugDraw* debugDraw = new DXDebugDraw(coloredBatch);
	debugDraw->setDebugMode(btIDebugDraw::DBG_DrawWireframe); // so does this
	bulletWorld->setDebugDrawer(debugDraw);
	timer.SetFixedTimeStep(true);
	timer.SetTargetElapsedSeconds(tickTime);

	std::shared_ptr<Floor> floor = std::make_shared<Floor>(coloredBatch, 2000);
	coloredShapes.push_back(floor);
	floor->registerCollisionObject(bulletWorld);
	//Scenario::boxTowers(texturedShapes, bulletWorld, texturedBatch);
	//Scenario::plankDomino(texturedShapes, bulletWorld, texturedBatch);
	Scenario::houseOfCards(texturedShapes, bulletWorld, texturedBatch);
	resetViewMatrix();
}