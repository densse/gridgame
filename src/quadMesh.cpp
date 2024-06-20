#include "quadMesh.h"
#include "config.h"

quadMesh::quadMesh(float width, float height)
{
	//std::cout << "creating mesh\n";

	float i = 2.f/3.5f;
	float data1[] = {
		-i		, -1	   , 0.0f, -1.0f, -1.0f,
		-i+width, -1	   , 0.0f,  1.0f, -1.0f,
	    -i		, -1+height, 0.0f, -1.0f,  1.0f
	};

	float data2[] = {
	    -i+width , -1+height, 0.0f,  1.0f,  1.0f,
		-i+width , -1       , 0.0f,  1.0f, -1.0f,
		-i       , -1+height, 0.0f, -1.0f,  1.0f
	};	
	vertexCount = 3;

	glGenVertexArrays(1, &VAO1);
	glBindVertexArray(VAO1);

	glGenBuffers(1, &VBO1);
	glBindBuffer(GL_ARRAY_BUFFER, VBO1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(data1), data1, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	glGenVertexArrays(1, &VAO2);
	glBindVertexArray(VAO2);

	glGenBuffers(1, &VBO2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(data2), data2, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}

void quadMesh::draw()
{
	glBindVertexArray(VAO1);
	glDrawArrays(GL_TRIANGLES, 0, vertexCount);
	
	glBindVertexArray(VAO2);
	glDrawArrays(GL_TRIANGLES, 0, vertexCount);
	//std::cout << "drawing mesh" << std::endl;
}

quadMesh::~quadMesh()
{
	glDeleteVertexArrays(1, &VAO1);
	glDeleteVertexArrays(1, &VAO2);
	glDeleteBuffers(2, &VBO1);
	glDeleteBuffers(2, &VBO2);
}
