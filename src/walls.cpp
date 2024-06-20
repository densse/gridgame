#include "walls.h"
#include <fstream>
#include "config.h"
#include "loadShader.h"
#include "generateTexture.h"

int levelSize = 5;
bool (*loadLevelWalls())[5]
{
	std::cout << "\n\nloading level walls" << "\n";
	static bool walls[5][5];
	std::ifstream data ("../assets/levels/walls.w");
	std::string d;
	if(data.is_open())
	{
		for(int y = 4; y >= 0; y--)
		{
			for(int x = 0; x < 5; x++)
			{
				d = data.get();
				if(d == "1")
				{
					walls[x][y] = true;
					std::cout << walls[x][y];
				}
				else if(d == "0")
				{
					walls[x][y] = false;
					std::cout << walls[x][y];
				}
				else if(d == "\n")
				{
					std::cout << "newline char skipping\n";
					x--;
				}
				else
				{
					std::cout << "undefined symbol: " << d << " continuing" << "\n";
					x--;
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
	return walls;
}

wallController::wallController(coreController* coreParameter)
{
	core = coreParameter;
	roomWidth = 142;
	roomHeight = 80;
	scaleX = 2./roomWidth;
	scaleY = 2./roomHeight;
	core->walls = loadLevelWalls();

	//for (int y = 4; y >= 0; y--) 
	//{
	//	for (int x = 0; x < 5; x++) 
	//	{
	//		std::cout << walls[x][y]<<" ";
	//	}
	//	std::cout << "\n";
    //}

}
void wallController::setup()
{
	for(int y = 4; y >= 0; y--)
	{
		for(int x = 0; x < 5; x++)
		{
			if(core->walls[x][y] == 1)
			{
				wallQuads[x][y] = new quadMesh(16.*scaleX, 16.*scaleY);
				core->impassables[x+1][y+1] = true;
				//std::cout << core->impassables[i+1][j+1] << " ";
			}
			else
			{
				wallQuads[x][y] = 0;
				core->impassables[x+1][y+1] = false;
				//std::cout << core->impassables[i+1][j+1] << " ";
			}
		}
		//std::cout << "\n";
	}
	//wallQuads = new quadMesh(16.*scaleX, 16.*scaleY);

	for(int y = 6; y>=0; y--)
	{
		for(int x = 0; x < 7; x++)
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
	for(int y = 4; y >= 0; y--)
	{
		for(int x = 0; x < 5; x++)
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

	for(int i = 0; i < 5; i++)
	{
		for(int j = 0; j < 5; j++)
		{
			delete wallQuads[i][j];
		}
	}
}
