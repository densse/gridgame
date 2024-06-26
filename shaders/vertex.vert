#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
  
out vec2 texCoord;

uniform mat4 model;

void main()
{
    gl_Position = model * vec4(aPos.x, aPos.y, aPos.z, 1.0);
	texCoord = 0.5+0.5*vec2(aTexCoord.x, aTexCoord.y);
}
