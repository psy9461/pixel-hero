#version 150

#ifdef GL_ES
precision mediump float;
#endif

uniform vec4 rect;
uniform float radius;
uniform float lineWidth;
uniform vec4 color;

out vec4 fragColor;

float rectDist(vec2 p, vec2 size, float radius)
{
	size -= vec2(radius);
	vec2 d = abs(p) - size;
	return min(max(d.x, d.y), 0.0) + length(max(d, 0.0)) - radius;
}

void main()
{
	float d = rectDist(gl_FragCoord.xy - rect.xy, rect.zw, radius);
	float a = clamp(d + lineWidth, 0.0, 1.0) - clamp(d, 0.0, 1.0);
	fragColor = vec4(color.rgb, color.a * a);
}
