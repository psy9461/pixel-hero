#version 150

#ifdef GL_ES
precision mediump float;
#endif

uniform sampler2D tex;

in vec4 colorV;
in vec2 texcoordV;

out vec4 fragColor;

void main()
{
	fragColor = texture(tex, texcoordV) * colorV;
}
