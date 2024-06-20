#include "quadMesh.h"
#include "algebra.h"
#include "core.h"
class player {
	public:
		float x, y;
		player(coreController* coreParma);
		void draw();
		void step();
		void up();
		void left();
		void down();
		void right();
		void setup();
		~player();
	private:
		float vspd, hspd, scaleX, scaleY;
		unsigned int shaderProgram, modelLocation, texture;
		quadMesh* quad;
		vec3 translation;
		mat4 model;
		int roomWidth, roomHeight;
		coreController* core;
};
