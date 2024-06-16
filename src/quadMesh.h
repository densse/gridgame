#pragma once
#include "config.h"

class quadMesh {
	public:
		quadMesh(float width, float height);
		void draw();
		void changePosition();
		~quadMesh();

	private:
		unsigned int VAO1, VBO1, vertexCount, VAO2, VBO2;
};
