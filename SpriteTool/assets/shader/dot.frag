#version 150

#ifdef GL_ES
precision mediump float;
#endif

uniform vec2 center;
uniform float radius;

in vec4 colorV;

out vec4 fragColor;

void main()
{
	fragColor = vec4(colorV.xyz, clamp(radius - length(center - gl_FragCoord.xy), 0.0f, 1.0f));
}
