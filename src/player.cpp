#include "player.h"
#include "config.h"
#include "quadMesh.h"
#include "loadShader.h"
#include "generateTexture.h"
#include <cmath>
#include "solids.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods, coreController* core);

void moveSandpile(int startX, int startY, int offsetX, int offsetY, coreController* core)
{
	if(core->solids[startX + offsetX][startY + offsetY].type() == typeid(solidController::sandpile))
	{
		std::cout << "pushing sandpile" << "\n";
		moveSandpile(startX + offsetX, startY + offsetY, offsetX, offsetY, core);
	}

	const solidController::sandpile sandpile = boost::any_cast<solidController::sandpile>(core->solids[startX][startY]);
	core->solids[startX][startY] = 0;
	core->solids[startX + offsetX][startY + offsetY] = sandpile;
	std::cout << "moving sandpile" << "\n";

}

bool checkSandpileCollisions(int x, int y, int offsetX, int offsetY, coreController* core)
{
	bool move = true;
	
	if(core->solids[x+offsetX][y+offsetY].type() == typeid(solidController::sandpile))
	{
		move = checkSandpileCollisions(x+offsetX, y+offsetY, offsetX, offsetY, core);
		std::cout << "checking collisions" << "\n";
	}
	if(x+offsetX < 0 or x+offsetX >= 5 or y+offsetY < 0 or y+offsetY >= 5 or core->walls[x+offsetX][y+offsetY] != 0)
	{
		move = false;
	}
	return move;
}


player::player(coreController* coreParam)
{
	x = 0.f;
	y = 0.f;
	vspd = 0.0f;
	hspd = 0.0f;
	roomWidth = 142;
	roomHeight = 80;
	scaleX = 2./roomWidth;
	scaleY = 2./roomHeight;
	core = coreParam;

}

void player::setup()
{
	quad = new quadMesh(16.*scaleX, 16.*scaleY);

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
	texture = generateTexture("../assets/player/idle.png");

	modelLocation = glGetUniformLocation(shaderProgram, "model");

}

void player::step()
{
	//scaleX = 2./roomWidth;
	//scaleY = 2./roomHeight;
	x += hspd;
	y += vspd;
	model.entries[12] = x * scaleX;
	model.entries[13] = y * scaleY;
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, model.entries);

	if(std::remainderf(y, 16.f) == 0)
	{
		vspd /= 1.05;
		if(vspd < 0)
		{
			vspd = 0;
		}
	}

	if(hspd > 0)
	{
		hspd /= 1.05;
		if(std::remainderf(x, 16.f) == 0)
		{
			hspd = 0;
		}
	}
}

void player::draw()
{
	glBindTexture(GL_TEXTURE_2D, texture);
	quad->draw();
}


player::~player()
{
	glDeleteProgram(shaderProgram);
	delete quad;
}

void player::up()
{
	int xi = static_cast<int>(x/16.f);
	int yi = static_cast<int>(y/16.f);
	//std::cout << playerObject->x << "\n";
	//std::cout << playerObject->y << "\n";
	//std::cout << x << "\n";
	//std::cout << y << "\n";
	if (not(core->walls[xi][yi+1]) && yi+1 < 5)
	{
		bool move = checkSandpileCollisions(xi, yi, 0, 1, core);

		if(core->solids[xi][yi+1].type() == typeid(solidController::sandpile) && yi+2 < 5 && move)
		{
			moveSandpile(xi, yi+1, 0, 1, core);
			y+=16;
			//playerObject2->up();
		}
		else if(move)
		{
			y+=16;
			//playerObject2->up();
		}
	}
}
void player::left()
{
	int xi = static_cast<int>(x/16.f);
	int yi = static_cast<int>(y/16.f);
	if (not(core->walls[xi-1][yi]) && xi-1 >= 0)
	{
		bool move = checkSandpileCollisions(xi, yi, -1, 0, core);

		if(core->solids[xi-1][yi].type() == typeid(solidController::sandpile) && xi-2 >= 0 && move)
		{
			moveSandpile(xi-1, yi, -1, 0, core);
			x-=16;
			//playerObject2->left();
		}
		else if(move)
		{
			x-=16;
			//playerObject2->left();
		}
	}
}

void player::down()
{
	int xi = static_cast<int>(x/16.f);
	int yi = static_cast<int>(y/16.f);
	if (not(core->walls[xi][yi-1]) && yi-1 >= 0)
	{
		bool move = checkSandpileCollisions(xi, yi, 0, -1, core);

		if(core->solids[xi][yi-1].type() == typeid(solidController::sandpile) && yi-2 >= 0 && move)
		{
			moveSandpile(xi, yi-1, 0, -1, core);
			y-=16;
			//playerObject2->down();
		}
		else if(move)
		{
			y-=16;
			//playerObject2->down();
		}
	}
}
void player::right()
{
	int xi = static_cast<int>(x/16.f);
	int yi = static_cast<int>(y/16.f);
	if (not(core->walls[xi+1][yi]) && xi+1 < 5)
	{
		bool move = checkSandpileCollisions(xi, yi, 1, 0, core);

		if(core->solids[xi+1][yi].type() == typeid(solidController::sandpile) && xi+1 < 5 && move)
		{
			moveSandpile(xi+1, yi, 1, 0, core);
			x+=16;
			//playerObject2->right();
		}
		else if(move)
		{
			x+=16;
			//playerObject2->right();
		}
	}
}
