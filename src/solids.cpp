#include "config.h"
#include "solids.h"
#include "quadMesh.h"
#include <boost/any.hpp>
#include "algebra.h"
#include "loadShader.h"
#include "generateTexture.h"

const int levelSize = 6;

solidController::solidController(coreController* coreParam)
{
	core = coreParam;
	levelSize = core->levelSize;
	roomWidth = ((16*levelSize)/9)*16;
	roomHeight = 16*levelSize;
	scaleX = 2./roomWidth;
	scaleY = 2./roomHeight;
	core = coreParam;
}

void solidController::setup()
{
	std::cout << "\nsetting up solids\n";
	solidsData = core->loadLevelSolids();

	for (int y = levelSize-1; y >= 0; y--)
	{
		for (int x = 0; x < levelSize; x++)
		{
			std::cout << solidsData[x][y];
			if(solidsData[x][y] == 1)//sandpile
			{
				sandpile sandpileStruct;
				sandpileStruct.quad = new quadMesh(16.*scaleX, 16.*scaleY);
				core->solids[x][y] = sandpileStruct;
				sandpile solidPointer = boost::any_cast<sandpile>(core->solids[x][y]);
				std::cout << solidPointer.id << " ";
			}
			else if(solidsData[x][y] == 2)//gate
			{
				gate gateStruct;
				gateStruct.quad = new quadMesh(16.*scaleX, 16.*scaleY);
				gateStruct.wavelength = 0;
				core->solids[x][y] = gateStruct;
				gate gatePointer = boost::any_cast<gate>(core->solids[x][y]);
				std::cout << gatePointer.id << " ";
			}
			else
			{
				std::cout << solidsData[x][y] << " ";
				core->solids[x][y] = 0;
			}
		}
		std::cout << std::endl;
	}


	vec3 transformation = {0.0, 0.0, 0.0};
	model = matrixCreateTransform(transformation);

	unsigned int vertexShader = loadVertexShader("../shaders/vertex.vert");
	unsigned int fragmentShader = loadFragmentShader("../shaders/fragment.frag");

	//create program
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	int success;
	char infoLog[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if(!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "error linking shader program\n" << infoLog << std::endl;
	}
	else
	{
		std::cout << "linked shaders" << std::endl;
	}
	
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glUseProgram(shaderProgram);
	sandpileTexture = generateTexture("../assets/solid/sandpile.png");
	gateTexture = generateTexture("../assets/solid/redGate.png");

	modelLocation = glGetUniformLocation(shaderProgram, "model");
	textureLocation = glGetUniformLocation(shaderProgram, "drawTexture");
}

void solidController::step()
{

}

void solidController::draw()
{
	glBindTexture(GL_TEXTURE_2D, texture);	
	for(int y = levelSize-1; y >= 0; y--)
	{
		for(int x = 0; x < levelSize; x++)
		{
			if(core->solids[x][y].type() == typeid(sandpile))
			{
				glBindTexture(GL_TEXTURE_2D, sandpileTexture);
				sandpile sandpileStruct = boost::any_cast<sandpile>(core->solids[x][y]);
				model.entries[12] = 16. * scaleX * x;
				model.entries[13] = 16. * scaleY * y;
				glUniformMatrix4fv(modelLocation, 1, GL_FALSE, model.entries);
				glUniform1i(textureLocation, 0);
				sandpileStruct.quad->draw();
			}

			if(core->solids[x][y].type() == typeid(gate))
			{
				glBindTexture(GL_TEXTURE_2D, gateTexture);
				gate gateStruct = boost::any_cast<gate>(core->solids[x][y]);
				model.entries[12] = 16. * scaleX * x;
				model.entries[13] = 16. * scaleY * y;
				glUniformMatrix4fv(modelLocation, 1, GL_FALSE, model.entries);
				glUniform1i(textureLocation, 0);
				gateStruct.quad->draw();
			}
		}
	}
}

solidController::~solidController()
{

}
