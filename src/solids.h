#include "config.h"
#include "algebra.h"
#include "boost/any.hpp"
#include "quadMesh.h"
#include "core.h"
class solidController {
	public:
		solidController(coreController* coreParam);
		void setup();
		void step();
		void draw();
		~solidController();

		struct sandpile
		{
			unsigned int id = 1;
			float offsetX = 0.f;
			float offsetY = 0.f;
			quadMesh* quad;
		};

		struct gate
		{
			unsigned int id = 2;
			float animation = 0.f;
			int wavelength = 0;
			quadMesh* quad;
		};
		//boost::any solids[5][5];

	private:
		unsigned int shaderProgram, textureLocation, texture, modelLocation, sandpileTexture, gateTexture;
		float scaleX, scaleY;
		int roomWidth, roomHeight;
		mat4 model;
		coreController* core;
		std::vector<std::vector<unsigned int>> solidsData;
		int levelSize;
};
