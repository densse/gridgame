#include "quadMesh.h"
#include "algebra.h"
class wallController {
	public:
		wallController();
		void draw();
		void step();
		void setup();
		~wallController();
		bool (*walls)[5];
	private:
		
		unsigned int shaderProgram, modelLocation, textureLocation, texture;
		float scaleX, scaleY;
		int roomHeight, roomWidth;
		mat4 model;
		quadMesh* wallQuads[5][5];
};
