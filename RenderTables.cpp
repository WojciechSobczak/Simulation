#include "pch.h"
#include "RenderTables.h"


void RenderTables::add(std::shared_ptr<Object> object, RenderBatches batch) {
}

void RenderTables::recreate(UINT size) {
	RenderTable objects_buff = RenderTable(size);
	for (int i = 0; i < RENDER_BATCHES_SIZE; i++) {
		objects_buff[i] = RenderRow(size);
		for (int j = 0; j < size; j++) {
			objects_buff[i][j] = objects[i][j];
		}
		for (int j = size; j < this->size; j++) {
			objects_buff[i][j].reset();
		}
	}

	this->size = size;
	this->objects.clear();
	this->objects = objects_buff;
}

RenderTables::RenderTables(UINT size) {
	this->size = size;
	objects = RenderTable(RENDER_BATCHES_SIZE);
	for (int i = 0; i < RENDER_BATCHES_SIZE; i++) {
		objects[i] = RenderRow(size);
	}
}

RenderTables::~RenderTables() {
	for (int i = 0; i < RENDER_BATCHES_SIZE; i++) {
		for (int j = 0; j < size; j++) {
			objects[i][j].reset();
		}
		objects[i].clear();
	}
	objects.clear();
}

