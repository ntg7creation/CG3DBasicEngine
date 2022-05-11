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

uniform vec4 eye;
uniform vec4 ambient;
uniform vec4[20] objects;
uniform vec4[20] objColors;
uniform vec4[10] lightsDirection;
uniform vec4[10] lightsIntensity;
uniform vec4[10] lightsPosition;
uniform ivec4 sizes;

out vec4 Color;


/// [x] DEFINITIONS for implemetation

const vec4 INFINITY_COLOR = vec4(0, 0, 0, 1);
const vec3 objectSpecularValue = vec3(0.7, 0.7, 0.7);

vec3 calculatePixelColor(vec3 pointOnObject, int objectId);
vec3 calculateDiffuseColor(vec3 pointOnObject, int objectId, int lightSourceId);
vec3 calculateSpecularColor(vec3 pointOnObject, int objectId, int lightSourceId);

vec3 calculateNormalToSurface(vec3 pointOnObject, int objectId);
bool lightReachesObject(int lightSourceId, vec3 pointOnObject);
float objectShininessFactor(int objectId);
bool lightIsDirectional(int lightSourceId);
bool objectIsSphere(int objectId);


void main() {

	Color = INFINITY_COLOR; // FIXME : incase not hitting anything (infinity)


	// Color = clamp(vec4(1.1, 100, -0.01, 1), 0, 1);

	// if (	(length(position0) <= length(vec3(0.1, 0.1, 0.1)))
	// 		&& Color == vec4(0,0,0,1)) {
			
	// 	Color = vec4(1,0,0,1);
	// }

	// if (	(length(position0) <= length(vec3(0.1, 0.1, 0.1)))
	// 		&& Color == vec4(0,1,0,1)) {
			
	// 	Color = vec4(0,0,1,1);
	// }
	
	// if (	(length(position0) <= length(vec3(0.1, 0.1, 0.1)))
	// 		&& Color == vec4(1,1,0,1)) {
			
	// 	Color = vec4(0,1,0,1);
	// }
	
	

	// if (length(intersectionPoint - vec3(0.6, 0.5, -1.0)) <= objects1[1][4]) {
	// 	Color = objColors[1];
	// }
}


/// [x] COLOR calculations

vec3 calculatePixelColor(vec3 pointOnObject, int objectId) {

	// TODO separate calculation of color based on object type: sphere or plane 
	// 			(sphere has multiplication by normal, planes have colored squares)

	vec3 color = // emission
		+ ambient.xyz * objColors[objectId].xyz;

	int numOfLights = sizes[1];

	for (int lightSourceId = 0; lightSourceId < numOfLights; ++lightSourceId) {
		// if light from source reaches the point on the object (if ray from point on object hits light source first without interruptions)
		if (lightReachesObject(lightSourceId, pointOnObject)) {
			color += calculateDiffuseColor(pointOnObject, objectId, lightSourceId)
				+ calculateSpecularColor(pointOnObject, objectId, lightSourceId);
		}
		// TODO : else, need shadow?
	}

	return clamp(color, 0, 1);
}



vec3 calculateDiffuseColor(vec3 pointOnObject, int objectId, int lightSourceId) {
	vec3 color;

	// TODO : make sure that works for both the spheres and the surfaces
	// TODO : implement squares on planes with different coefficient
	// 			(mod(int(1.5*p.x),2) == mod(int(1.5*p.y),2))

	color = objColors[objectId].xyz
		* lightsIntensity[lightSourceId].xyz
		* calculateNormalToSurface(pointOnObject, objectId) // FIXME : not using dot product
		* normalize(lightsDirection[lightSourceId].xyz); // FIXME : direction is not entirely correct for spot lights

	return color;
}


vec3 calculateSpecularColor(vec3 pointOnObject, int objectId, int lightSourceId) {

	// TODO: divide the implementation between directional light and a spot light

	return objectSpecularValue
		* pow(
				dot(
						normalize(pointOnObject - eye),
						normalize(calculateReflectRay(pointOnObject, objectId, lightSourceId))
					), // TODO check if should use max(0, V*R)
				objectShininessFactor(objectId)
			)
		* lightsIntensity[lightSourceId].xyz; // FIXME
}


// FIXME : think about the implications of the cutoff angle of a spotlight
vec3 calculateReflectRay(vec3 pointOnObject, int objectId, int lightSourceId) {
	vec3 correctedNormal;
	vec3 rayFromSurfaceToLight;
	
	if (lightIsDirectional(lightSourceId)) {
		rayFromSurfaceToLight = lightDirection[lightSourceId].xyz;
	}
	else {
		rayFromSurfaceToLight = pointOnObject - lightsPosition[lightSourceId];
	}

	correctedNormal = calculateNormalToSurface(pointOnObject, objectId);
	if (dot(rayFromSurfaceToLight, correctedNormal) < dot(rayFromSurfaceToLight, -correctedNormal)) {
		correctedNormal = -correctedNormal;
	}

	return 2 * dot(correctedNormal, rayFromSurfaceToLight) * correctedNormal - rayFromSurfaceToLight;
}



/// [x] HELPER functions


float objectShininessFactor(int objectId) {
	return objColors[objectId][3];
}


vec3 calculateNormalToSurface(vec3 pointOnObject, int objectId) {
	if (objectIsSphere(objectId)) {
		return normalize(pointOnObject - objects[objectId].xyz);
	}
	else {
		return normalize(objects[objectId].xyz);
	}
}


bool lightReachesObject(int lightSourceId, vec3 pointOnObject) {
	// if light is directional, return true if not hitting ANY objects on the opposite direction of the light

	// if light is a spotlight, return true if not hitting any objects, before reaching the light source, on the opposite direction of the light

	return false; // FIXME
}


bool lightIsDirectional(int lightSourceId) {
	return lightsDirection[lightSourceId][3] == 0.0;
}


bool objectIsSphere(int objectId) {
	return objects[objectId][3] > 0.0; // by definition of syntax of scene file
}