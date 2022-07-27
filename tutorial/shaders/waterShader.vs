#version 330

attribute vec3 position;
attribute vec3 normal;
attribute vec4 Ka;
attribute vec4 Kd;
attribute vec4 Ks;
attribute vec2 texcoord;


out vec2 texCoord0;
out vec3 normal0;
out vec3 color0;
out vec3 position0;

uniform float time;
uniform mat4 Proj;
uniform mat4 View;
uniform mat4 Model;
uniform vec4 tranlasion;

vec3 newnormal = vec3(0);
float amplitude = 1; // not used
vec4 wave1 = vec4(1,0,0.5,20);// direction.x direction.y Steepness wave_length
vec4 wave2 = vec4(0,1,0.25,40);
vec4 wave3 = vec4(1,1,0.15,20);
//float Steepness = 0.5;  // var3
//float wave_length = 20;  //var4
float speed  = 1; // not used
//vec2 direction = normalize(vec2(0.5,0.5)); // var 1,2
#define M_PI 3.1415926535897932384626433832795
#define Earth_G 9.8

float maxAmplitude = 0;
vec3 GerstnerWave(vec4 wave,vec3 pos,inout vec3 tangent,inout vec3 binormal)
{
	
	float steepness = wave.z;
	float wavelength = wave.w;
	float newX,newY,newZ;
	float k = 2*M_PI/wave.w;
	float Phase_speed = sqrt(Earth_G / k); // swap in Phase_speed instead of speed for earth like waves
	vec2 direction = wave.xy;
	float f = k * (dot(direction,(pos.xz+tranlasion.xz)) - Phase_speed * time);
	float amplitude = wave.z / k;
	maxAmplitude+= amplitude;

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
		p += GerstnerWave(wave1, gridPoint, tangent, binormal);
		p += GerstnerWave(wave2, gridPoint, tangent, binormal);
		p += GerstnerWave(wave3, gridPoint, tangent, binormal);
		vec3 normal = normalize(cross(binormal, tangent));
		newnormal = normal;
	
	return p;
}



void main()
{

	newnormal = vec3(0,1,0);//normal;

	vec3 position3 = vert(position);
	
	float maxY =position.y +  maxAmplitude;
	float minY =position.y -  maxAmplitude;
	float hight_bloom = 0.5 + (position3.y - minY)/(maxY-minY);

	texCoord0 = texcoord;
	//color0 = vec3(Ka);
	//color0 =  (hight_bloom+ newnormal+vec3(1))/2*vec3(1,1,1);
	color0 = hight_bloom * vec3(1,1,1);

	normal0 = (Model  * vec4(newnormal, 0.0)).xyz;
	position0 = vec3(Proj *View *Model * vec4(position3, 1.0));
	gl_Position = Proj *View * Model* vec4(position3, 1.0); //you must have gl_Position

}
