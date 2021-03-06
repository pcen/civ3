#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 vbTexCoord;

out vec2 texCoord;

uniform mat4 u_MVP;

void main()
{
	gl_Position = u_MVP * position;
	texCoord = vbTexCoord;
}
