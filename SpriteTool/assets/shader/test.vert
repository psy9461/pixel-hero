#version 150

#ifdef GL_ES
precision mediump float;
#endif

uniform mat4 projMatrix, viewMatrix;

in vec4 position;
in vec4 color;
in vec2 texcoord;

out vec4 colorV;
out vec2 texcoordV;

void main()
{
	gl_Position = projMatrix * viewMatrix * position;
	colorV = color;
	texcoordV = texcoord;
}






























