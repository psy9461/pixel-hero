#version 150

#ifdef GL_ES
precision mediump float;
#endif

//uniform vec2 resolution;
uniform vec2 center;

uniform sampler2D tex;

in vec4 colorV;
in vec2 texcoordV;

out vec4 fragColor;

void main()
{
	//vec4 c = 
	
	// gl_FragCoord - device position

	//vec2 uv = gl_FragCoord.xy/resolution;
//	float a = length(center - gl_FragCoord.xy) / 50.0;
//
//	float g = clamp(1.0-a, 0.0, 1.0);
//	float b = clamp(a-1.0, 0.0, 1.0);
//	fragColor = vec4(0.0, g, b, 1.0) * colorV;
//	fragColor = colorV;
	fragColor = texture(tex, texcoordV) * colorV;
	//if( a < 1.0 )
	//	fragColor = vec4(0.0, 1.0, 0.0, 1.0);
	//else
	//	fragColor = vec4(0.0, 0.0, 1.0, 1.0);
	//
	//float g = clamp(a, 0.0, 1.0);

	//if( gl_FragCoord.x < 200 )
	//	fragColor = vec4(1.0, 0.0, 0.0, 1.0);
	//else
	//	fragColor = vec4(0.0, 0.0, 1.0, 1.0);
}
