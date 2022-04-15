#version 330

//varying vec3 color0;
in vec2 texCoord0;
in vec3 normal0;
in vec3 color0;
in vec3 position0;

//uniform sampler2D sampler1;
uniform vec4 lightDirection;
uniform vec4 lightColor;

uniform float ScreenScaleX;
uniform float ScreenScaleY;

uniform float offsetx;
uniform float offsety;

uniform float root1_x;
uniform float root1_y;

uniform float root2_x;
uniform float root2_y;

uniform float root3_x;
uniform float root3_y;

uniform float a;
uniform float b;
uniform float c;
uniform float d;

uniform int iterationNum;

out vec4 Color;


vec2 newton_raphson(vec2 point,int iterationNum)
{
	vec2 z = point;
	
	
	for(int k = 1; k <= iterationNum; k++)
	{
		float x = z.x;
		float y = z.y;

		vec2 z_f = vec2(
			a * (pow(x,3) -3*x*pow(y,2)) + 
			b * (pow(x,2) -pow(y,2)) +
			c * x +
			d,

			a * (3*pow(x, 2)*y -pow(y,3)) + 
			b * (2*x*y) +
			c * y
		);	
				  
		vec2 z_f_tag = vec2(
			3*a * (pow(x,2) -pow(y,2)) +
			2*b * x +
			c,

			3*a * (2*x*y) +
			2*b * y
		);
		
		vec2 z_div = vec2(
						z_f.x*z_f_tag.x + z_f.y*z_f_tag.y,
						z_f.y*z_f_tag.x - z_f.x*z_f_tag.y
		) / (pow(z_f_tag.x,2) + pow(z_f_tag.y,2));
		
		z = z - z_div;
	}
	
	return z;
}



void main()
{
	vec4 red = vec4(1,0,0,0);
	vec4 green = vec4(0,1,0,0);
	vec4 blue = vec4(0,0,1,0);

	vec2 z0 = vec2((texCoord0.x),(texCoord0.y));
	z0 = newton_raphson(z0, iterationNum);

	vec2 trueRoot1 = vec2(root1_x,root1_y);
	vec2 trueRoot2 = vec2(root2_x,root2_y);	
	vec2 trueRoot3 = vec2(root3_x,root3_y);

	float len1 = length(trueRoot1 - z0);
	float len2 = length(trueRoot2 - z0);
	float len3 = length(trueRoot3 - z0);

	//Color = lightColor;
	Color = red; // assume closest is len1
	
	if(len2 <= len1)
		Color = green;
	if(len3 <= len2 && len3 <=len1)
		Color = blue;
}

