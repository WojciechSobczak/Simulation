#include "pch.h"
#include "Simulation.h"
#include <time.h>
#define TIME_EFF_TEST
void Simulation::renderScene() {
	//Przed renderowaniem nastêpuje czyszczenie buforów
#ifdef TIME_EFF_TEST
	clock_t before = clock();
#endif // TIME_EFF_TEST

	
	clearBuffers();
	deviceContext->OMSetBlendState(states->Opaque(), nullptr, 0xFFFFFFFF);
	deviceContext->OMSetDepthStencilState(depthStencilStage.Get(), 0);
	deviceContext->RSSetState(rasterizerStage.Get());
	basicEffect->Apply(deviceContext.Get());
	deviceContext->IASetInputLayout(inputLayout.Get());

	
	using namespace DirectX;
	coloredBatch->Begin();
	for (int i = 0; i < coloredShapes.size(); i++) {
		coloredShapes[i]->render();
	}
	coloredBatch->End();


	//Zwalnianie bufora do renderowania
	presentBackBuffer();
#ifdef TIME_EFF_TEST
	Debug::printTime(clock() - before);
#endif //TIME_EFF_TEST
}

