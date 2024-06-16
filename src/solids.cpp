#include "config.h"
#include "solids.h"
#include "quadMesh.h"
#include <boost/any.hpp>
#include "algebra.h"
#include "loadShader.h"
#include "generateTexture.h"

unsigned int (*loadLevelSolids())[5]
{
	std::cout << "loading level solids" << "\n";
	static unsigned int solids[5][5];
	std::ifstream data ("../assets/levels/solids.s");
	std::string d;
	if(data.is_open())
	{
		for(int i = 0; i < 5; i++)
		{
			for(int j = 0; j < 5; j++)
			{
				d = data.get();
				if(d == "\n")
				{
					std::cout << "\nnewline char skipping" << "\n";
					j--;
				}
				else
				{
					solids[i][j] = std::stoi(d);
					std::cout << solids[i][j];
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

solidController::solidController()
{
	roomWidth = 142;
	roomHeight = 80;
	scaleX = 2./roomWidth;
	scaleY = 2./roomHeight;
}

void solidController::setup()
{
	int levelSize = 5;
	solidsData = loadLevelSolids();

	//boost::any solids[5][5];

	for (int i = 0; i < levelSize; i++) 
	{
		for (int j = 0; j < levelSize; j++) 
		{
			if(solidsData[i][j] == 1)//sandpile
			{
				sandpile sandpileStruct;
				sandpileStruct.quad = new quadMesh(16.*scaleX, 16.*scaleY);
				solids[i][j] = sandpileStruct; //reverses becuase of a bug
				sandpile solidPointer = boost::any_cast<sandpile>(solids[i][j]);
				std::cout << solidPointer.id << " ";
			}
			else if(solidsData[i][j] == 2)//gate
			{
				gate gateStruct;
				gateStruct.quad = new quadMesh(16.*scaleX, 16.*scaleY);
				solids[i][j] = gateStruct;
				gate gatePointer = boost::any_cast<gate>(solids[i][j]);
				std::cout << gatePointer.id << " ";
			}
			else
			{
				std::cout << solidsData[i][j] << " ";
				solids[j][i] = 0;
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
	for(int i = 0; i < 5; i++)
	{
		for(int j = 0; j < 5; j++)
		{
			if(solids[i][j].type() == typeid(sandpile))
			{
				glBindTexture(GL_TEXTURE_2D, sandpileTexture);
				sandpile sandpileStruct = boost::any_cast<sandpile>(solids[i][j]);
				model.entries[12] = 16. * scaleX * i;
				model.entries[13] = 16. * scaleY * j;
				glUniformMatrix4fv(modelLocation, 1, GL_FALSE, model.entries);
				glUniform1i(textureLocation, 0);
				sandpileStruct.quad->draw();
			}

			if(solids[i][j].type() == typeid(gate))
			{
				glBindTexture(GL_TEXTURE_2D, gateTexture);
				gate gateStruct = boost::any_cast<gate>(solids[i][j]);
				model.entries[12] = 16. * scaleX * i;
				model.entries[13] = 16. * scaleY * j;
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
