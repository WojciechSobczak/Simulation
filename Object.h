#pragma once
#include "pch.h"


class Object {
public:
	virtual void render() = 0;
	virtual void printToOutput() = 0;
};
