#pragma once

#include "quadMesh.h"
#include "algebra.h"
#include "core.h"
#include <vector>

class wallController {
	public:
		wallController(coreController* coreParameter);
		void draw();
		void step();
		void setup();
		~wallController();
		//bool (*walls)[5];
	private:
		unsigned int shaderProgram, modelLocation, textureLocation, texture;
		float scaleX, scaleY;
		int roomHeight, roomWidth;
		mat4 model;
		coreController* core;
		int levelSize;
		std::vector<std::vector<quadMesh*> > wallQuads;
};
