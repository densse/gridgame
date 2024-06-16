#include "player.h"
#include "config.h"
#include "quadMesh.h"
#include "loadShader.h"
#include "generateTexture.h"
#include <cmath>

player::player()
{
	x = 0.f;
	y = 0.f;
	vspd = 0.0f;
	hspd = 0.0f;
	roomWidth = 142;
	roomHeight = 80;
	scaleX = 2./roomWidth;
	scaleY = 2./roomHeight;
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

//input
void player::up()
{
	//vspd = 1.f;
	y+=16;
}

void player::left()
{
	//hspd = -1.f;
	x-=16;
}

void player::down()
{
	//vspd = -1.f;
	y-=16;
}

void player::right()
{
	//hspd += 1.f;
	x+=16;
}


player::~player()
{
	glDeleteProgram(shaderProgram);
	delete quad;
}
