#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 tPos;

out vec2 texturePos;

uniform mat4 projection;
uniform mat4 model;

void main()
{
	texturePos = tPos;
 	gl_Position = projection * model * vec4(aPos, 0, 1.0);
}

