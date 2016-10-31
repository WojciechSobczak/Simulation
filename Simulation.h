//
// Game.h
//

#pragma once

#include "StepTimer.h"


// A basic game implementation that creates a D3D11 device and
// provides a game loop.
using DirectX::SimpleMath::Vector3;
using DirectX::SimpleMath::Matrix;

class Simulation
{
public:
    Simulation();
	~Simulation();

	const float tickTime = float(1) / float(60);
	const int defaultWidth = 1280;
	const int defaultHeight = 720;
	const float aspectRatio = float(defaultWidth) / float(defaultHeight);

    // Initialization and management
    void initialize(HWND window, int width, int height);

    // Basic game loop
    void tick();

    // Messages
    void onWindowSizeChanged(int width, int height);
	void onMouseOver(int x, int y);
	void onLeftMouseButtonDown(int x, int y);
	void onLeftMouseButtonUp(int x, int y);
	void onKeyboardKeyPress();

	bool leftMouseButtonDown = false;
	bool rightMouseButtonUp = false;

private:

	std::vector<std::shared_ptr<Object<DirectX::VertexPositionColor>>> coloredShapes;


    void update(DX::StepTimer const& timer);
	void resetViewMatrix();
    void renderScene();
    void clearBuffers();
    void presentBackBuffer();
    void createDevice();
    void createResources();
    void onDeviceLost();
	int prevMousex = 0;
	int prevMousey = 0;

	float moveForward = 0;
	float moveRight = 0;
	float moveUp = 0;

	float yaw = 0;
	float roll = 0;
	float pitch = 1;

	bool physicsEnabled = false;

	const Matrix worldProjection = Matrix::CreatePerspectiveFieldOfView(M_PI_2, aspectRatio, 1, 2000);
	//const Matrix worldProjection = Matrix::CreateOrthographicOffCenter(-500, 500, -500, 500, -500, 500);
	DirectX::XMMATRIX cameraProjection = DirectX::XMMatrixSet(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	);
	DirectX::XMVECTOR position = {-29.853180, 700.589142, -800.206512};
	const DirectX::XMVECTOR cameraPosition = {0.0f, 0.0f, 0.0f};
	const DirectX::XMVECTOR cameraUp = {0.0f, 1.0f, 0.0f};
	const DirectX::XMVECTOR cameraLook = {0.0f, 0.0f, -1.0f};
	const DirectX::XMVECTOR cameraRight = {1.0f, 0.0f, 0.0f};

    // Device resources.
    HWND                                            windowHandler;
    int                                             windowWidth = defaultWidth;
    int                                             windowHeight = defaultHeight;

    D3D_FEATURE_LEVEL                               dxFeatureLevel;
    Microsoft::WRL::ComPtr<ID3D11Device>            device;
    Microsoft::WRL::ComPtr<ID3D11Device1>           device1;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext>     deviceContext;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext1>    deviceContext1;

    Microsoft::WRL::ComPtr<IDXGISwapChain>          swapChain;
    Microsoft::WRL::ComPtr<IDXGISwapChain1>         swapChain1;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView>  renderTargetView;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView>  depthStencilView;

	//Stages
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>	rasterizerStage;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilStage;

    // Rendering loop timer.
    DX::StepTimer timer;

	std::unique_ptr<DirectX::CommonStates>										states;
	std::unique_ptr<DirectX::BasicEffect>										basicEffect;
	std::shared_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>>		coloredBatch;
	std::shared_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>>	texturedBatch;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>									coloredInputLayout;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>									texturedInputLayout;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>							plankTexture;


	btDiscreteDynamicsWorld*								bulletWorld;
	btDefaultCollisionConfiguration*						physicsCollisionConfig;
	btCollisionDispatcher*									physicsCollisionDispatcher;
	btBroadphaseInterface*									physicsBroadphase;
	btSequentialImpulseConstraintSolver*					physicsSolver;
	btAlignedObjectArray<btCollisionShape*>					physicsShapes;
};