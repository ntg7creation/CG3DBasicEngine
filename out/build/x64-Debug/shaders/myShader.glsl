#version 330

//varying vec3 color0;
in vec2 texCoord0;
in vec3 normal0;
in vec3 color0;
in vec3 position0;

out vec4 Color;


//uniform sampler2D sampler1;
// uniform vec4 lightDirection;
uniform vec4 lightColor;

uniform float screenScaling;
uniform float xOffset;
uniform float yOffset;

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

uniform vec4 color1;
uniform vec4 color2;
uniform vec4 color3;

vec2 trueRoot1 = vec2(root1_x,root1_y);
vec2 trueRoot2 = vec2(root2_x,root2_y);	
vec2 trueRoot3 = vec2(root3_x,root3_y);


const float thresholdDistanceFromRoot = 0.3;//length(vec2(1,1));


bool isntTrueRoot(vec2 z) {
	return 
		length(z - trueRoot1) >= thresholdDistanceFromRoot &&
		length(z - trueRoot2) >= thresholdDistanceFromRoot &&
		length(z - trueRoot3) >= thresholdDistanceFromRoot;
}


vec3 newton_raphson(vec2 z, int iterationNum) {
	int k = 0;

	for( ; k < iterationNum && isntTrueRoot(z); k++)
	{
		float x = z.x;
		float y = z.y;

		vec2 z_f = vec2(
			a * (x*x*x -3*x*y*y) + 
			b * (x*x -y*y) +
			c * x +
			d,

			a * (3*x*x*y -y*y*y) + 
			b * (2*x*y) +
			c * y
		);	
				  
		vec2 z_f_tag = vec2(
			3*a * (x*x -y*y) +
			2*b * x +
			c,

			3*a * (2*x*y) +
			2*b * y
		);

		vec2 z1 = z_f;
		vec2 z2 = z_f_tag;
		float z2_abs = (z2.x*z2.x + z2.y*z2.y);

		vec2 z_div = vec2(
			(z1.x*z2.x + z1.y*z2.y)/z2_abs,
			(z1.y*z2.x - z1.x*z2.y)/z2_abs
		);
		
		z = z - z_div;
	}

	return vec3(z.x, z.y, k);
}


void setColorFromNewtonRaphson(vec2 z_k, int k) {
	float distToRoot1 = length(trueRoot1 - z_k);
	float distToRoot2 = length(trueRoot2 - z_k);
	float distToRoot3 = length(trueRoot3 - z_k);

	Color = color1; // assume closest is len1 and change accordingly
	if(distToRoot2 < distToRoot1)
		Color = color2;
	if(distToRoot3 < distToRoot2 && distToRoot3 < distToRoot1)
		Color = color3;

	Color *= clamp(pow(0.9, floor(k)), 0.5, 1);
}


void main() {
	vec2 z0 = screenScaling * vec2((texCoord0.x) + xOffset,(texCoord0.y) + yOffset);
	vec3 retVal = newton_raphson(z0, iterationNum);

	setColorFromNewtonRaphson(vec2(retVal.x, retVal.y), int(retVal.z));
}

