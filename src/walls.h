#pragma once

#include "quadMesh.h"
#include "algebra.h"
#include "core.h"
class wallController {
	public:
		wallController(coreController* coreParameter);
		void draw();
		void step();
		void setup();
		~wallController();
		//bool (*walls)[5];
		coreController* core;
	private:
		unsigned int shaderProgram, modelLocation, textureLocation, texture;
		float scaleX, scaleY;
		int roomHeight, roomWidth;
		mat4 model;
		quadMesh* wallQuads[5][5];
};
