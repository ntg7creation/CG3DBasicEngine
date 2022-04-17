#version 330

//varying vec3 color0;
in vec2 texCoord0;
in vec3 normal0;
in vec3 color0;
in vec3 position0;

out vec4 Color;


//uniform sampler2D sampler1;
//uniform sampler2D sampler2;
// uniform vec4 lightDirection;
// uniform vec4 lightColor;

uniform float screenHeight;
uniform float screenWidth;

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

vec2 coords = vec2(position0.x, position0.y);


bool closeEnoughToTrueRoot(vec2 z, int k) {
	float thresholdDistanceFromRoot = pow(length(vec2(1.0/screenWidth, 1.0/screenHeight)*screenScaling), 0.5);

	return 
		length(z - trueRoot1) <= k * thresholdDistanceFromRoot ||
		length(z - trueRoot2) <= k * thresholdDistanceFromRoot ||
		length(z - trueRoot3) <= k * thresholdDistanceFromRoot;
		//abs(z.x - trueRoot1.x) <= thresholdDistanceFromRoot &&
		//abs(z.y - trueRoot1.y) <= thresholdDistanceFromRoot ||
		//abs(z.x - trueRoot2.x) <= thresholdDistanceFromRoot &&
		//abs(z.y - trueRoot2.y) <= thresholdDistanceFromRoot ||
		//abs(z.x - trueRoot3.x) <= thresholdDistanceFromRoot &&
		//abs(z.y - trueRoot3.y) <= thresholdDistanceFromRoot;

}


vec2 divideComplex(vec2 z1, vec2 z2) {
	float z2_abs = (z2.x*z2.x + z2.y*z2.y);
	
	return vec2(
			(z1.x*z2.x + z1.y*z2.y)/z2_abs,
			(z1.y*z2.x - z1.x*z2.y)/z2_abs
		);
}


vec2 f(vec2 z) {
	float x = z.x;
	float y = z.y;

	return vec2(
		a * (x*x*x -3*x*y*y) + 
		b * (x*x -y*y) +
		c * x +
		d,

		a * (3*x*x*y -y*y*y) + 
		b * (2*x*y) +
		c * y
	);	
}


vec2 df(vec2 z) {
	float x = z.x;
	float y = z.y;
	
	return vec2(
		3*a * (x*x -y*y) +
		2*b * x +
		c,

		3*a * (2*x*y) +
		2*b * y
	);
}


vec3 newton_raphson(vec2 z, int iterationNum) {
	int k = 0;
	int k_coloring = 0;

	for( ; k < iterationNum; k++) {
		if ( ! closeEnoughToTrueRoot(z, k + 1)) {
			k_coloring++;
		}

		vec2 z_f = f(z);
		vec2 z_df = df(z);
		z = z - divideComplex(z_f, z_df);
	}

	return vec3(z.x, z.y, k_coloring);
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

	Color *= clamp(pow(0.95, k), 0, 1);
}


void main() {
	vec2 z0 = coords*screenScaling + 2*vec2(-xOffset ,yOffset);

	vec3 retVal = newton_raphson(z0, iterationNum);

	setColorFromNewtonRaphson(vec2(retVal.x, retVal.y), int(retVal.z));

	// if (texCoord0.x < 0.5 && texCoord0.y < 0.5)
	// 	Color = vec4(1,0,0,0);
}

