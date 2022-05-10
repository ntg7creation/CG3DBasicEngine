#version 330

in vec2 texCoord0;
in vec3 normal0;
in vec3 color0;
in vec3 position0;

// uniform vec4 lightColor;
// uniform sampler2D sampler1;
// uniform vec4 lightDirection;

// uniform vec4 Proj;
// uniform vec4 View;
// uniform vec4 Model;

// uniform vec4 eye;
// uniform vec4 ambient;
// uniform vec4 objects;
// uniform vec4 objColors;
// uniform vec4 lightsPosition;
// uniform vec4 lightsDirection;
// uniform vec4 lightsIntensity;

out vec4 Color;

void main()
{
	// Color = texture2D(sampler1, texCoord0) * vec4(color0,1.0); // you must have gl_FragColor
	Color = vec4(1,0,0,1);
}
