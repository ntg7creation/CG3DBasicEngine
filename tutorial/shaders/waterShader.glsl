#version 330

in vec2 texCoord0;
in vec3 normal0;
in vec3 color0;
in vec3 position0;

uniform vec4 lightColor;
uniform sampler2D sampler1;
uniform vec4 lightDirection;

out vec4 Color;



void main()
{
	//vec3 temp = vec3(3,1,2)+vec3(1,2,3);
	Color = texture2D(sampler1, texCoord0)* vec4(color0,0.5); //you must have gl_FragColor
}
