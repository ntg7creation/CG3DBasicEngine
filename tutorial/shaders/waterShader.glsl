#version 330

in vec2 texCoord0;

in vec3 normal0;
in vec3 color0;
in vec3 position0;

uniform vec4 lightColor;
uniform sampler2D sampler1;
uniform vec4 lightDirection;

out vec4 Color;

float time = 2;

vec4 tranlasion;

vec3 newnormal = vec3(0);
float amplitude = 1; // not used
vec4 single_wave = vec4(0.5,0.5,0.5,10);// direction.x direction.y Steepness wave_length
//float Steepness = 0.5;  // var3
//float wave_length = 20;  //var4
float speed  = 1; // not used
//vec2 direction = normalize(vec2(0.5,0.5)); // var 1,2
#define M_PI 3.1415926535897932384626433832795
#define Earth_G 9.8


vec3 GerstnerWave(vec4 wave,vec3 pos,inout vec3 tangent,inout vec3 binormal)
{
	
	float steepness = wave.z;
	float wavelength = wave.w;
	float newX,newY,newZ;
	float k = 2*M_PI/wave.w;
	float Phase_speed = sqrt(Earth_G / k); // swap in Phase_speed instead of speed for earth like waves
	vec2 direction = wave.xy;
	float f = k * (dot(direction,pos.xz+tranlasion.xz) - Phase_speed * time);
	float amplitude = wave.z / k;


	newX = pos.x + wave.x* amplitude * cos(f);
	newY = pos.y + amplitude* sin(f);
	newZ =  pos.z + wave.y * amplitude * cos(f);



	 tangent +=vec3(	-direction.x*direction.x * steepness * sin(f)
								,direction.x*steepness * cos(f)
								,-direction.x*direction.y*steepness*sin(f) );
	 binormal += vec3(	-direction.x*direction.y *steepness*sin(f),
						direction.y* steepness * cos(f),
						-direction.y*direction.y *steepness * sin(f));

	//newnormal = normalize(cross(binormal,tangent));



	//vec3 newPos = vec3(newX,newY,newZ);
	return vec3(	direction.x* amplitude * cos(f),
					amplitude* sin(f),
					direction.y * amplitude * cos(f));
}

vec3 vert(vec3 pos)
{
		vec3 gridPoint = pos.xyz;
		vec3 tangent = vec3(1, 0, 0);
		vec3 binormal = vec3(0, 0, 1);
		vec3 p = gridPoint;
		p += GerstnerWave(single_wave, gridPoint, tangent, binormal);

		vec3 normal = normalize(cross(binormal, tangent));
		newnormal = normal;
	
	return p;
}


void main()
{
	//vec3 temp = vec3(3,1,2)+vec3(1,2,3);
	Color = texture2D(sampler1, texCoord0)* vec4(color0,0.5); //you must have gl_FragColor
}
