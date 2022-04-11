#version 330

//varying vec3 color0;
in vec2 texCoord0;
in vec3 normal0;
in vec3 color0;
in vec3 position0;

uniform sampler2D sampler1;
uniform vec4 lightDirection;
uniform vec4 lightColor;

uniform float ScreenScaleX;
uniform float ScreenScaleY;

uniform float offsetx;
uniform float offsety;

uniform float root1_x;
uniform float root2_x;
uniform float root3_x;

uniform float root1_y;
uniform float root2_y;
uniform float root3_y;

uniform float a;
uniform float b;
uniform float c;
uniform float d;
uniform float n;
//uniform vec2 root1
out vec4 Color;


vec2 newton_raphson(vec2 point,int n)
{
	vec2 z0 = point;
	
	
	for(int k =0;k<n;k++)
	{
	
		//z0= z0 -
		//x^3 - 3xy *a
		//x^2-y^2 * b
		//xc
		
		//yi
		//3x^2y - y^3 * a
		//2yx *b
		//yc
		vec2 z_f = vec2(	//x
				a * (z0.x*z0.x*z0.x -3*z0.x*z0.y*z0.y) + 
				b * (z0.x*z0.x -z0.y*z0.y) +
				c * (z0.x)+
				0
				,
				//yi
				a * (3*z0.x*z0.x*z0.y - z0.y*z0.y*z0.y)+
				b * (2*z0.y*z0.x) +
				c * (z0.y));
				
				//xcords - 3ax^2 - 3ay^2 +2bx +c  
				// ycords - 6ayx + 2by +c				
				  
		vec2 z_f_tag = vec2(	
						//x
						3*a*(z0.x*z0.x - z0.y*z0.y)+
						2*b*(z0.x)+
						c
						,
						//yi
						6*a*z0.y*z0.x+
						2*b* z0.y +
						c);
		
		vec2 z_div = vec2(
						(z_f.x*z_f_tag.x + z_f.y*z_f_tag.y)/
						(z_f_tag.x*z_f_tag.x + z_f_tag.y*z_f_tag.y)
						,
						(z_f.y*z_f_tag.x - z_f.x*z_f_tag.y)/
						(z_f_tag.x*z_f_tag.x + z_f_tag.y*z_f_tag.y)
						); 
		
		z0 = z0 - z_div;
	}
	
	return z0;
}



void main()
{
	int N = int(n);
	vec4 red = vec4(1,0,0,0);
	vec4 green = vec4(0,1,0,0);
	vec4 blue = vec4(0,0,1,0);
	vec2 point1 = vec2(root1_x,root1_y);
	vec2 point2 = vec2(root2_x,root2_y);
	vec2 point3 = vec2(root3_x,root3_y);

	vec2 z0 = vec2((texCoord0.x+offsetx)*ScreenScaleX,(texCoord0.y+offsety)*ScreenScaleY);

	z0 =  newton_raphson(z0,N);

	float len1 = length(point1 - z0);
	float len2 = length(point2 - z0);
	float len3 = length(point3 - z0);

	//Color = lightColor;
	Color = red; // assume closest is len1
	
	if(len2 <= len1)
		Color = green;
	if(len3 <= len2 && len3 <=len1)
		Color = blue;
}

