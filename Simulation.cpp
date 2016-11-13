//
// Game.cpp
//

#include "pch.h"
#include "Simulation.h"
#include "DXDebugDraw.h"
#include "Sphere.h"

using DirectX::SimpleMath::Matrix;
using DirectX::SimpleMath::Vector3;
using DirectX::VertexPositionColor;

using Microsoft::WRL::ComPtr;

Simulation::Simulation() : windowHandler(0), dxFeatureLevel(D3D_FEATURE_LEVEL_9_1) {}
Simulation::~Simulation() {}

// Executes the basic game loop.
void Simulation::tick() {
	timer.Tick([&]() {
		update(timer);
	});
	renderScene();
}

// Updates the world.
void Simulation::update(DX::StepTimer const& timer) {
	if (physicsEnabled) {
		static UINT64 lastTime = 20;
		bulletWorld->stepSimulation(lastTime, 1, float(1) / 10);
		lastTime = timer.GetElapsedTicks();
	}
}

void Simulation::clearBuffers() {
	// Czyszczenie widoków.
	deviceContext->ClearRenderTargetView(renderTargetView.Get(), DirectX::Colors::SlateGray);
	deviceContext->ClearDepthStencilView(depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	deviceContext->OMSetRenderTargets(1, renderTargetView.GetAddressOf(), depthStencilView.Get());
	//Tworzenie viewportu
	//TODO Dopracowaæ ¿eby siê skalowa³a szerokoœæ te¿
	if (float(windowWidth) / float(windowHeight) != aspectRatio) {
		viewport = CD3D11_VIEWPORT(0.0f, 0.0f, static_cast<float>(windowWidth), static_cast<float>(windowWidth / aspectRatio));
	} else {
		viewport = CD3D11_VIEWPORT(0.0f, 0.0f, static_cast<float>(windowWidth), static_cast<float>(windowHeight));
	}
	deviceContext->RSSetViewports(1, &viewport);
}

// Presents the back buffer contents to the screen.
void Simulation::presentBackBuffer() {
	// The first argument instructs DXGI to block until VSync, putting the application
	// to sleep until the next VSync. This ensures we don't waste any cycles rendering
	// frames that will never be displayed to the screen.
	HRESULT hr = swapChain->Present(1, 0);

	// If the device was reset we must completely reinitialize the renderer.
	if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET) {
		onDeviceLost();
	} else {
		ThrowIfFailed(hr);
	}
}

void Simulation::onWindowSizeChanged(int width, int height) {
	windowWidth = std::max(width, 1);
	windowHeight = std::max(height, 1);
	createResources();
	clearBuffers();
	// TODO: Game window is being resized.
}

// These are the resources that depend on the device.
void Simulation::createDevice() {
	static const D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1,
	};

	// Create the DX11 API device object, and get a corresponding context.
	UINT creationFlags = 0;
#ifdef DEBUG
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	HRESULT hr = D3D11CreateDevice(
		nullptr,                                // Pobiera domyœlny adapter. Znaczy to ¿e bêdzie to pierwsze dostêpne urz¹dzenie, jakie zwróci IDXGIFactory1::EnumAdapters
		D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_HARDWARE, //Tworzy typ sterownika, w tym przypadku bêdzie to sterownik sprzêtowy, mo¿e byæ WARP lub odniesienia.
		nullptr,								// WskaŸnik na sterownik programowy, je¿eli wybraliœmy sterownik programowy wy¿ej.
		creationFlags,
		featureLevels,
		_countof(featureLevels),
		D3D11_SDK_VERSION,
		device.ReleaseAndGetAddressOf(),   // returns the Direct3D device created
		&dxFeatureLevel,                        // returns feature level of device created
		deviceContext.ReleaseAndGetAddressOf()   // returns the device immediate context
	);

	if (hr == E_INVALIDARG) {
		// DirectX 11.0 platforms will not recognize D3D_FEATURE_LEVEL_11_1 so we need to retry without it.
		hr = D3D11CreateDevice(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			creationFlags,
			&featureLevels[1],
			_countof(featureLevels) - 1,
			D3D11_SDK_VERSION,
			device.ReleaseAndGetAddressOf(),
			&dxFeatureLevel,
			deviceContext.ReleaseAndGetAddressOf()
		);
	}

	ThrowIfFailed(hr);

#ifdef DEBUG
	ComPtr<ID3D11Debug> d3dDebug;
	if (SUCCEEDED(device.As(&d3dDebug))) {
		d3dDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
		ComPtr<ID3D11InfoQueue> d3dInfoQueue;
		if (SUCCEEDED(d3dDebug.As(&d3dInfoQueue))) {
		#ifdef DEBUG
			d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true);
			d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, true);
		#endif
			D3D11_MESSAGE_ID hide[] =
			{
				D3D11_MESSAGE_ID_SETPRIVATEDATA_CHANGINGPARAMS,
				// TODO: Add more message IDs here as needed.
			};
			D3D11_INFO_QUEUE_FILTER filter = {};
			filter.DenyList.NumIDs = _countof(hide);
			filter.DenyList.pIDList = hide;
			d3dInfoQueue->AddStorageFilterEntries(&filter);
		}
	}
#endif

	// DirectX 11.1 if present
	if (SUCCEEDED(device.As(&device1)))
		(void)deviceContext.As(&deviceContext1);

	// TODO: Initialize device dependent objects here (independent of window size).

	
	ThrowIfFailed(CreateWICTextureFromFile(device.Get(), L"plank.png", nullptr, plankTexture.GetAddressOf()));
	ThrowIfFailed(CreateWICTextureFromFile(device.Get(), L"box.png", nullptr, boxTexture.GetAddressOf()));
	

	states = std::make_unique<DirectX::CommonStates>(device.Get());
	coloredBasicEffect = std::make_unique<DirectX::BasicEffect>(device.Get());
	coloredBasicEffect->SetProjection(projectionMatrix);
	coloredBasicEffect->SetView(viewMatrix);
	coloredBasicEffect->SetWorld(worldMatrix);
	coloredBasicEffect->SetVertexColorEnabled(true);

	void const* shaderByteCode;
	size_t byteCodeLength;

	coloredBasicEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);

	ThrowIfFailed(
		device->CreateInputLayout(
			VertexPositionColor::InputElements,
			VertexPositionColor::InputElementCount,
			shaderByteCode, byteCodeLength,
			coloredInputLayout.ReleaseAndGetAddressOf()
		)
	);
	coloredBatch = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>>(deviceContext.Get());

	#ifdef TEXTURES_ENABLED
	void const* shaderByteCodetx;
	size_t byteCodeLengthtx;
	texturedBasicEffect = std::make_unique<DirectX::BasicEffect>(device.Get());
	texturedBasicEffect->SetProjection(projectionMatrix);
	texturedBasicEffect->SetView(viewMatrix);
	texturedBasicEffect->SetWorld(worldMatrix);
	texturedBasicEffect->SetTextureEnabled(true);
	texturedBasicEffect->SetTexture(boxTexture.Get());
	#ifdef LIGHTING_TEXTURES_ENABLED
	//texturedBasicEffect->SetAmbientLightColor(Colors::White);
	//texturedBasicEffect->SetDiffuseColor(Colors::White);
	//texturedBasicEffect->SetEmissiveColor({163.f/255.f, 126.f/255.f, 74.f/255.f, 0});
	//texturedBasicEffect->SetLightSpecularColor(0, Colors::White);
	texturedBasicEffect->SetLightDirection(0, {0.f, 1.0f, 0.f, 0.f});//Dont ask why
	texturedBasicEffect->SetLightEnabled(0, true);
	texturedBasicEffect->SetLightingEnabled(true);
	#endif // LIGHTING_TEXTURES_ENABLED
	texturedBasicEffect->GetVertexShaderBytecode(&shaderByteCodetx, &byteCodeLengthtx);
	ThrowIfFailed(
		device->CreateInputLayout(
			TEXTURED_VERTEX_TYPE::InputElements,
			TEXTURED_VERTEX_TYPE::InputElementCount,
			shaderByteCodetx, byteCodeLengthtx,
			texturedInputLayout.ReleaseAndGetAddressOf()
		)
	);
	texturedBatch = std::make_unique<DirectX::PrimitiveBatch<TEXTURED_VERTEX_TYPE>>(deviceContext.Get());
	#endif

	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = D3D11_FILTER::D3D11_FILTER_ANISOTROPIC;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MaxAnisotropy = (device->GetFeatureLevel() > D3D_FEATURE_LEVEL_9_1) ? D3D11_MAX_MAXANISOTROPY : 2;
	samplerDesc.MaxLOD = FLT_MAX;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;

	ThrowIfFailed(device->CreateSamplerState(&samplerDesc, samplerState.ReleaseAndGetAddressOf()));

}

// Allocate all memory resources that change on a window SizeChanged event.
void Simulation::createResources() {
	// Clear the previous window size specific context.
	ID3D11RenderTargetView* nullViews[] = {nullptr};
	deviceContext->OMSetRenderTargets(_countof(nullViews), nullViews, nullptr);
	renderTargetView.Reset();
	depthStencilView.Reset();
	deviceContext->Flush();

	UINT backBufferWidth = static_cast<UINT>(windowWidth);
	UINT backBufferHeight = static_cast<UINT>(windowHeight);
	DXGI_FORMAT backBufferFormat = DXGI_FORMAT_B8G8R8A8_UNORM;
	DXGI_FORMAT depthBufferFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	UINT backBufferCount = 2;
	device->CheckMultisampleQualityLevels(backBufferFormat, sampleCount, &maxQualityLevel);
	if (maxQualityLevel > 0) {
		maxQualityLevel--;
	}

	// If the swap chain already exists, resize it, otherwise create one.
	if (/*swapChain*/ false) {
		HRESULT hr = swapChain->ResizeBuffers(backBufferCount, backBufferWidth, backBufferHeight, backBufferFormat, 0);

		if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET) {
			// If the device was removed for any reason, a new device and swap chain will need to be created.
			onDeviceLost();

			// Everything is set up now. Do not continue execution of this method. OnDeviceLost will reenter this method 
			// and correctly set up the new device.
			return;
		} else {
			ThrowIfFailed(hr);
		}
	} else {
		swapChain.Reset();
		// First, retrieve the underlying DXGI Device from the D3D Device.
		ComPtr<IDXGIDevice1> dxgiDevice;
		ThrowIfFailed(device.As(&dxgiDevice));

		// Identify the physical adapter (GPU or card) this device is running on.
		ComPtr<IDXGIAdapter> dxgiAdapter;
		ThrowIfFailed(dxgiDevice->GetAdapter(dxgiAdapter.GetAddressOf()));

		// And obtain the factory object that created it.
		ComPtr<IDXGIFactory1> dxgiFactory;
		ThrowIfFailed(dxgiAdapter->GetParent(IID_PPV_ARGS(dxgiFactory.GetAddressOf())));

		ComPtr<IDXGIFactory2> dxgiFactory2;
		if (SUCCEEDED(dxgiFactory.As(&dxgiFactory2))) {
			// DirectX 11.1 or later
			//UINT qualityLevel = 0;
			//device->CheckMultisampleQualityLevels(backBufferFormat, 4, &qualityLevel);

			// Create a descriptor for the swap chain.
			DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {0};
			swapChainDesc.Width = backBufferWidth;
			swapChainDesc.Height = backBufferHeight;
			swapChainDesc.Format = backBufferFormat;
			swapChainDesc.SampleDesc.Count = sampleCount; //TODO pokombinowaæ tutaj z jakoœci¹
			swapChainDesc.SampleDesc.Quality = maxQualityLevel;
			swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			swapChainDesc.BufferCount = backBufferCount;

			DXGI_SWAP_CHAIN_FULLSCREEN_DESC fsSwapChainDesc = {0};
			fsSwapChainDesc.Windowed = TRUE;

			// Create a SwapChain from a Win32 window.
			ThrowIfFailed(dxgiFactory2->CreateSwapChainForHwnd(
				device.Get(),
				windowHandler,
				&swapChainDesc,
				&fsSwapChainDesc,
				nullptr,
				swapChain1.ReleaseAndGetAddressOf()
			));

			ThrowIfFailed(swapChain1.As(&swapChain));
		} else {
			DXGI_SWAP_CHAIN_DESC swapChainDesc = {0};
			swapChainDesc.BufferCount = backBufferCount;
			swapChainDesc.BufferDesc.Width = backBufferWidth;
			swapChainDesc.BufferDesc.Height = backBufferHeight;
			swapChainDesc.BufferDesc.Format = backBufferFormat;
			swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			swapChainDesc.OutputWindow = windowHandler;
			swapChainDesc.SampleDesc.Count = sampleCount;
			swapChainDesc.SampleDesc.Quality = maxQualityLevel;
			swapChainDesc.Windowed = TRUE;

			ThrowIfFailed(dxgiFactory->CreateSwapChain(device.Get(), &swapChainDesc, swapChain.ReleaseAndGetAddressOf()));
		}

		// This template does not support exclusive fullscreen mode and prevents DXGI from responding to the ALT+ENTER shortcut.
		ThrowIfFailed(dxgiFactory->MakeWindowAssociation(windowHandler, DXGI_MWA_NO_ALT_ENTER));
	}

	// Obtain the backbuffer for this window which will be the final 3D rendertarget.
	ComPtr<ID3D11Texture2D> backBuffer;
	ThrowIfFailed(swapChain->GetBuffer(0, IID_PPV_ARGS(backBuffer.GetAddressOf())));

	// Create a view interface on the rendertarget to use on bind.
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.Format = DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;
	rtvDesc.Texture2D.MipSlice = 0;
	ThrowIfFailed(device->CreateRenderTargetView(backBuffer.Get(), &rtvDesc, renderTargetView.ReleaseAndGetAddressOf()));

	// Allocate a 2-D surface as the depth/stencil buffer and
	// create a DepthStencil view on this surface to use on bind.
	CD3D11_TEXTURE2D_DESC depthStencilDesc(depthBufferFormat, backBufferWidth, backBufferHeight, 1, 1, D3D11_BIND_DEPTH_STENCIL);
	depthStencilDesc.SampleDesc.Count = sampleCount;
	depthStencilDesc.SampleDesc.Quality = maxQualityLevel;

	ComPtr<ID3D11Texture2D> depthStencil;
	ThrowIfFailed(device->CreateTexture2D(&depthStencilDesc, nullptr, depthStencil.GetAddressOf()));

	CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2DMS);
	ThrowIfFailed(device->CreateDepthStencilView(depthStencil.Get(), &depthStencilViewDesc, depthStencilView.ReleaseAndGetAddressOf()));

	CD3D11_RASTERIZER_DESC rastDesc(D3D11_FILL_SOLID, 
									D3D11_CULL_BACK, 
									FALSE,
									D3D11_DEFAULT_DEPTH_BIAS, 
									D3D11_DEFAULT_DEPTH_BIAS_CLAMP,
									D3D11_DEFAULT_SLOPE_SCALED_DEPTH_BIAS, 
									TRUE, 
									FALSE, 
									TRUE, 
									TRUE);
	ThrowIfFailed(device->CreateRasterizerState(&rastDesc, rasterizerStage.ReleaseAndGetAddressOf()));

	D3D11_DEPTH_STENCIL_DESC dsDesc;
	// Depth test parameters
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;

	// Stencil test parameters
	dsDesc.StencilEnable = true;
	dsDesc.StencilReadMask = 0xFF;
	dsDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing
	dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing
	dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	device->CreateDepthStencilState(&dsDesc, depthStencilStage.GetAddressOf());

	// TODO: Initialize windows-size dependent objects here.
}

void Simulation::onDeviceLost() {
	// TODO: Add Direct3D resource cleanup here.

	depthStencilView.Reset();
	renderTargetView.Reset();
	swapChain1.Reset();
	swapChain.Reset();
	deviceContext1.Reset();
	deviceContext.Reset();
	device1.Reset();
	device.Reset();

	states.reset();
	coloredBasicEffect.reset();
	texturedBasicEffect.reset();
	coloredBatch.reset();
	coloredInputLayout.Reset();
	rasterizerStage.Reset();

	createDevice();
	createResources();
}

void Simulation::removeAllWorldObjects() {
	for (int i = 0; i < texturedShapes.size(); i++) {
		texturedShapes[i]->unregisterCollisionObject(bulletWorld);
	}
	texturedShapes.clear();
}

void Simulation::initScenario(void(*func)(std::vector<std::shared_ptr<Object<TEXTURED_VERTEX_TYPE>>>&, btDiscreteDynamicsWorld*, std::shared_ptr<DirectX::PrimitiveBatch<TEXTURED_VERTEX_TYPE>>)) {
	removeAllWorldObjects();
	func(this->texturedShapes, this->bulletWorld, this->texturedBatch);
}

void Simulation::onLeftMouseButtonDown(int x, int y) {
	XMVECTOR boxPosition = observerPosition;
	XMVECTOR rotationQuat = XMQuaternionRotationRollPitchYaw(pitch, yaw, roll);
	XMVECTOR boxPositionDiplacement = XMVector3Rotate({0, 0, 1}, rotationQuat);
	boxPosition += (XMVector3Normalize(boxPositionDiplacement) * 100);

	btVector3 force = XMVECTOR_TO_BTVECTOR(boxPositionDiplacement);
	force *= 1000;

	std::shared_ptr<Cube> cube = std::make_shared<Cube>(texturedBatch, boxPosition, 20, 10);
	cube->rigidBody->applyCentralImpulse(force);
	cube->registerCollisionObject(bulletWorld);
	texturedShapes.push_back(cube);
	leftMouseButtonDown = true;
}

void Simulation::onLeftMouseButtonUp(int x, int y) {
	leftMouseButtonDown = false;
}


