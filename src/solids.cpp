#include "config.h"
#include "solids.h"
#include "quadMesh.h"
#include <boost/any.hpp>
#include "algebra.h"
#include "loadShader.h"
#include "generateTexture.h"

unsigned int (*loadLevelSolids())[5]
{
	std::cout << "\n\nloading level solids" << "\n";
	static unsigned int solids[5][5];
	std::ifstream data ("../assets/levels/solids.s");
	std::string d;
	if(data.is_open())
	{
		for(int y = 4; y >= 0; y--)
		{
			for(int x = 0; x < 5; x++)
			{
				d = data.get();
				if(d == "\n")
				{
					std::cout << "newline char skipping" << "\n";
					x--;
				}
				else
				{
					solids[x][y] = std::stoi(d);
					std::cout << solids[x][y];
				}
			}
			std::cout << "\n";
		}
	}
	else
	{
		std::cout << "file not opened properly" << std::endl;
	}
	data.close();
	return solids;
}

solidController::solidController(coreController* coreParam)
{
	roomWidth = 142;
	roomHeight = 80;
	scaleX = 2./roomWidth;
	scaleY = 2./roomHeight;
	core = coreParam;
}

void solidController::setup()
{
	int levelSize = 5;
	solidsData = loadLevelSolids();

	//boost::any solids[5][5];

	for (int y = 4; y >= 0; y--) 
	{
		for (int x = 0; x < levelSize; x++) 
		{
			if(solidsData[x][y] == 1)//sandpile
			{
				sandpile sandpileStruct;
				sandpileStruct.quad = new quadMesh(16.*scaleX, 16.*scaleY);
				core->solids[x][y] = sandpileStruct; //reverses becuase of a bug
				sandpile solidPointer = boost::any_cast<sandpile>(core->solids[x][y]);
				std::cout << solidPointer.id << " ";
			}
			else if(solidsData[x][y] == 2)//gate
			{
				gate gateStruct;
				gateStruct.quad = new quadMesh(16.*scaleX, 16.*scaleY);
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
	for(int y = 4; y >= 0; y--)
	{
		for(int x = 0; x < 5; x++)
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
