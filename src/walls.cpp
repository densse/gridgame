#include "walls.h"
#include <fstream>
#include "config.h"
#include "loadShader.h"
#include "generateTexture.h"

int levelSize = 5;
bool (*loadLevelWalls())[5]
{
	std::cout << "loading level walls" << "\n";
	static bool walls[5][5];
	std::ifstream data ("../assets/levels/walls.w");
	std::string d;
	if(data.is_open())
	{
		for(int i = 0; i < 5; i++)
		{
			for(int j = 0; j < 5; j++)
			{
				d = data.get();
				if(d == "1")
				{
					walls[i][j] = true;
					std::cout << walls[i][j];
				}
				else if(d == "0")
				{
					walls[i][j] = false;
					std::cout << walls[i][j];
				}
				else if(d == "\n")
				{
					std::cout << "\nnewline char skipping" << "\n";
					j--;
				}
				else
				{
					std::cout << "undefined symbol: " << d << " continuing" << "\n";
					j--;
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

wallController::wallController()
{
	roomWidth = 142;
	roomHeight = 80;
	scaleX = 2./roomWidth;
	scaleY = 2./roomHeight;
	//levelSize = 5;
	//bool(*walls)[5];
	walls = loadLevelWalls();

	for (int i = 0; i < levelSize; ++i) {
		for (int j = 0; j < levelSize; ++j) {
			std::cout << walls[i][j]<<" ";
		}
		std::cout << std::endl;
    }

}
void wallController::setup()
{
	
	for(int i = 0; i < 5; i++)
	{
		for(int j = 0; j < 5; j++)
		{
			if(walls[i][j] == 1)
			{
				wallQuads[i][j] = new quadMesh(16.*scaleX, 16.*scaleY);
			}
			else
			{
				wallQuads[i][j] = 0;
			}
		}
	}
	//wallQuads = new quadMesh(16.*scaleX, 16.*scaleY);

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
	for(int i = 0; i < 5; i++)
	{
		for(int j = 0; j < 5; j++)
		{
			if(wallQuads[i][j] != 0)
			{
				model.entries[12] = 16. * scaleX * i;
				model.entries[13] = 16. * scaleY * j;
				glUniformMatrix4fv(modelLocation, 1, GL_FALSE, model.entries);
				glUniform1i(textureLocation, 0);
				wallQuads[i][j]->draw();
			}
		}
	}
	//wallQuads->draw();
}

wallController::~wallController()
{
	glDeleteProgram(shaderProgram);
	//delete wallQuads[0][0];

	for(int i = 0; i< 5; i++)
	{
		for(int j = 0; j < 5; j++)
		{
			delete wallQuads[i][j];
		}
	}
}
