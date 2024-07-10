#include "walls.h"
#include <fstream>
#include "config.h"
#include "loadShader.h"
#include "generateTexture.h"


wallController::wallController(coreController* coreParameter)
{
	core = coreParameter;
	levelSize = core->levelSize;
	roomWidth = ((16*levelSize)/9)*16;
	roomHeight = 16*levelSize;
	scaleX = 2./roomWidth;
	scaleY = 2./roomHeight;
	core->walls = core->loadLevelWalls();

	std::cout << "\nprinting walls\n";
	for (int y = levelSize-1; y >= 0; y--) 
	{
		for (int x = 0; x < levelSize; x++) 
		{
			std::cout << core->walls[x][y]<<" ";
		}
		std::cout << "\n";
    }

}
void wallController::setup()
{
	std::cout << "\nsetting up walls";
	for(int i=0; i<levelSize; i++)
	{
		std::vector<quadMesh*> row;
		for(int j=0; j<levelSize; j++)
		{
			row.push_back(0);
		}
		wallQuads.push_back(row);
	}

	for(int y = levelSize-1; y >= 0; y--)
	{
		for(int x = 0; x < levelSize; x++)
		{
			if(core->walls[x][y] == 1)
			{
				wallQuads[x][y] = new quadMesh(16.*scaleX, 16.*scaleY);
				core->impassables[x+1][y+1] = true;
				std::cout << wallQuads[x][y] << " ";
			}
			else
			{
				wallQuads[x][y] = 0;
				std::cout << wallQuads[x][y] << " ";
			}
		}
		std::cout << "\n";
	}
	//wallQuads = new quadMesh(16.*scaleX, 16.*scaleY);

	for(int y = levelSize+1; y>=0; y--)
	{
		for(int x = 0; x < levelSize+2; x++)
		{
			std::cout << core->impassables[x][y] << " ";
		}
		std::cout << "\n";
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
	texture = generateTexture("../assets/grid/wall.png");

	modelLocation = glGetUniformLocation(shaderProgram, "model");
	textureLocation = glGetUniformLocation(shaderProgram, "drawTexture");
}

void wallController::step()
{
}

void wallController::draw()
{
	glBindTexture(GL_TEXTURE_2D, texture);	
	for(int y = levelSize-1; y >= 0; y--)
	{
		for(int x = 0; x < levelSize; x++)
		{
			if(wallQuads[x][y] != 0)
			{
				model.entries[12] = 16. * scaleX * x;
				model.entries[13] = 16. * scaleY * y;
				glUniformMatrix4fv(modelLocation, 1, GL_FALSE, model.entries);
				glUniform1i(textureLocation, 0);
				wallQuads[x][y]->draw();
				//std::cout << "1" << " ";
			}
			else
			{
				//std::cout << "0" << " ";
			}
			//std::cout << wallQuads[x][y] << " ";
			
		}
		//std::cout << "\n";	
	}
	//std::cout << "\n";
	//wallQuads->draw();
}

wallController::~wallController()
{
	glDeleteProgram(shaderProgram);
	//delete wallQuads[0][0];

	for(int i = 0; i < levelSize; i++)
	{
		for(int j = 0; j < levelSize; j++)
		{
			delete wallQuads[i][j];
		}
	}
}
