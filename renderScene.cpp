#include "pch.h"
#include "Simulation.h"
#include <time.h>
#undef TIME_EFF_TEST
using namespace DirectX;
void renderAxes(std::shared_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> batch) {
	batch->DrawLine(
		VertexPositionColor({-500, 0, 0, 0}, Colors::Green),
		VertexPositionColor({500, 0, 0, 0}, Colors::Green)
	);
	batch->DrawLine(
		VertexPositionColor({0, -500, 0, 0}, Colors::Red),
		VertexPositionColor({0, 500, 0, 0}, Colors::Red)
	);
	batch->DrawLine(
		VertexPositionColor({0, 0, -500, 0}, Colors::Blue),
		VertexPositionColor({0, 0, 500, 0}, Colors::Blue)
	);
}


void Simulation::renderScene() {
	//Przed renderowaniem nastêpuje czyszczenie buforów
#ifdef TIME_EFF_TEST
	clock_t before = clock();
#endif // TIME_EFF_TEST

	
	clearBuffers();
	deviceContext->OMSetBlendState(states->Opaque(), nullptr, 0xFFFFFFFF);
	deviceContext->OMSetDepthStencilState(depthStencilStage.Get(), 0);
	deviceContext->RSSetState(rasterizerStage.Get());
	deviceContext->PSSetSamplers(0, 1, samplerState.GetAddressOf());

	deviceContext->IASetInputLayout(coloredInputLayout.Get());
	coloredBasicEffect->Apply(deviceContext.Get());
	coloredBatch->Begin();
	renderAxes(coloredBatch);
	for (int i = 0; i < coloredShapes.size(); i++) {
		coloredShapes[i]->render();
	}
#ifdef SIMULATION_DRAW_DEBUG
	bulletWorld->debugDrawWorld();
#endif //SIMULATION_DRAW_DEBUG
	coloredBatch->End();


#ifdef TEXTURES_ENABLED
	deviceContext->IASetInputLayout(texturedInputLayout.Get());
	texturedBasicEffect->Apply(deviceContext.Get());
	texturedBatch->Begin();
	for (int i = 0; i < texturedShapes.size(); i++) {
		texturedShapes[i]->render();
	}
	texturedBatch->End();
#endif // TEXTURES_ENABLED
#ifdef TIME_EFF_TEST
	Debug::printTime(clock() - before);
#endif //TIME_EFF_TEST

//Zwalnianie bufora do renderowania
	presentBackBuffer();

}

