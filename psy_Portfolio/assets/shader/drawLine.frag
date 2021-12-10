#version 150

#ifdef GL_ES
precision mediump float;
#endif

uniform vec2 sp, ep;
uniform float lineWidth;
uniform vec4 color;

out vec4 fragColor;

float lineDist(vec2 p, vec2 start, vec2 end, float width)
{
	vec2 n = end - start;
	float len = length(n);
	n /= len;

	vec2 m = p - start;
	vec2 proj = clamp( dot(m, n), 0.0, len ) * n;
	return length(m-proj) - width / 2.0;
}

void main()
{
	float d = lineDist(gl_FragCoord.xy, sp, ep, lineWidth);
	d = d / (lineWidth/2);
	float a = clamp(-d, 0.0, 1.0);
	a = a * a * a;
	fragColor = vec4(color.rgb, color.a * a);
}
