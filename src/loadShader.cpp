#include "config.h"
#include "loadShader.h"
#include "loadString.h" 


unsigned int loadVertexShader(const std::string filename)
{
	std::string shaderSrc = loadString(filename);
	const char* shaderSource = shaderSrc.c_str();

	unsigned int shader;
	shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(shader, 1, &shaderSource, NULL);
	glCompileShader(shader);
	
	int success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "error compiling vertex shader\n" << infoLog << std::endl;
	}
	return shader;
}


unsigned int loadFragmentShader(const std::string filename)
{
	std::string shaderSrc = loadString(filename);
	const char* shaderSource = shaderSrc.c_str();

	unsigned int shader;
	shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(shader, 1, &shaderSource, NULL);
	glCompileShader(shader);
	
	int success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "error compiling fragment shader\n" << infoLog << std::endl;
	}
	return shader;
}
