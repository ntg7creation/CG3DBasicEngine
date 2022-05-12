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

vec3 eyeCoordinates = eye.xyz;
const vec4 INFINITY_COLOR = vec4(0, 0, 0, 1);
const vec3 objectSpecularValue = vec3(0.7, 0.7, 0.7);

vec4 rayObjectIntersection(vec3 originPoint, vec3 normalizedDirection, int objectIndex);
vec4 rayPlaneIntersection(vec3 originPoint, vec3 normalizedDirection, vec4 plane);
vec4 raySphereIntersection(vec3 originPoint, vec3 normalizedDirection, vec4 sphere);

vec3 calculatePixelColor(vec3 pointOnObject, int objectId);
vec3 calculateDiffuseColor(vec3 pointOnObject, int objectId, int lightSourceId);
vec3 calculateSpecularColor(vec3 pointOnObject, int objectId, int lightSourceId);
vec3 calculateReflectRay(vec3 pointOnObject, int objectId, int lightSourceId);

vec3 calculateNormalToSurface(vec3 pointOnObject, int objectId);
bool lightReachesObject(int lightSourceId, vec3 pointOnObject, int objectIndex);
float objectShininessFactor(int objectId);
bool lightIsDirectional(int lightSourceId);
bool objectIsSphere(int objectId);
vec3 centerOfSphere(vec4 sphere);
float radiusOfSphere(vec4 sphere);

const vec4 RED = vec4(1, 0, 0, 1);
const vec4 GREEN = vec4(0, 1, 0, 1);
const vec4 BLUE = vec4(0, 0, 1, 1);
const vec4 WHITE = vec4(1, 1, 1, 1);
const vec4 BLACK = vec4(0, 0, 0, 1);
void assert(bool condition, vec4 colorOnTrue);


void main() {
	Color = WHITE; // FIXME : incase not hitting anything (infinity)

	vec3 normalizedRayDirection = normalize(vec3(position0.x, position0.y, 1) - eyeCoordinates); // TODO : figure out why it's negative
	int numOfObjects = sizes[0];

	vec4 res; // FIXME : remove?
	float min_t = 1.0e10;

	for (int objectIndex = 0; objectIndex < numOfObjects; ++objectIndex) {
		res = rayObjectIntersection(eyeCoordinates, normalizedRayDirection, objectIndex);

		if (res[3] > 0 && res[3] < min_t) { // if valid intersection point
			min_t = res[3];
			Color = vec4(calculatePixelColor(res.xyz, objectIndex), 1);
		}
	}

	// FIXME : remove debug
	// assert(lightReachesObject(/*light*/0, /*pointOnObject*/), RED);

	// Color = clamp(vec4(1.1, 100, -0.01, 1), 0, 1); // TODO : if using another piece of code make sure it has clamp
}


/// [x] COLOR calculations

vec3 getPointOnPlane(vec4 plane) {
	float a = plane.x;
	float b = plane.y;
	float c = plane.z;
	float d = plane.w;

	vec3 Q0 = vec3(0,0,0);
	if (a != 0.0) {
		Q0.x = -d/a;
	}
	else if (b != 0.0) {
		Q0.b = -d/b;
	}
	else { // c != 0.0
		Q0.b = -d/c;
	}

	return Q0;
}


vec3 calculatePixelColor(vec3 pointOnObject, int objectId) {

	// TODO separate calculation of color based on object type: sphere or plane 
	// 			(sphere has multiplication by normal, planes have multiplication by normal and colored squares)

	vec3 color = ambient.xyz * objColors[objectId].xyz; // FIXME : uncomment ambient color
	
	int numOfLights = sizes[1];
	for (int lightSourceId = 0; lightSourceId < numOfLights; ++lightSourceId) {
		// if light from source reaches the point on the object (if ray from point on object hits light source first without interruptions)
		if (lightReachesObject(lightSourceId, pointOnObject, objectId)) {
			color += calculateDiffuseColor(pointOnObject, objectId, lightSourceId)
				+ calculateSpecularColor(pointOnObject, objectId, lightSourceId);

			// FIXME : remove debug
			// if (!objectIsSphere(objectId)) {
			// 	return RED.xyz;
			// }
		}
		// TODO : else, need shadow?
	}

	return clamp(color, 0, 1);
}


float correctedDot(vec3 normal, vec3 normalizedDirection) {
	float rawDot = dot(normal, normalizedDirection);

	return max(rawDot, -rawDot);
}


bool darkenSquareAtPoint(vec3 pointOnObject) {
	float x = pointOnObject.x;
	float y = pointOnObject.y;

	bool basicCondition = mod(int(x), 2) == mod(int(y), 2);
	bool axisesMatchSigns = x >= 0 && y >= 0 || x <= 0 && y <= 0; // so that there won't be bigger rectangles near the axises

	return basicCondition && axisesMatchSigns
		|| !basicCondition && !axisesMatchSigns;
}


vec3 calculateDiffuseColor(vec3 pointOnObject, int objectId, int lightSourceId) {
	float diffuseCoefficient = 1;

	if ( ( ! objectIsSphere(objectId)) && darkenSquareAtPoint(pointOnObject) ) {
		diffuseCoefficient = 0.5;
	}

	vec3 color = diffuseCoefficient
		* objColors[objectId].xyz
		* lightsIntensity[lightSourceId].xyz
		* correctedDot(
			calculateNormalToSurface(pointOnObject, objectId),
			normalize(lightsDirection[lightSourceId].xyz)
		); // FIXME : direction is not entirely correct for spot lights

	return clamp(color, 0, 1);
}


vec3 calculateSpecularColor(vec3 pointOnObject, int objectId, int lightSourceId) {

	// TODO: divide the implementation between directional light and a spot light
	vec3 color = objectSpecularValue
		* pow(
				max(0, dot(
						normalize(pointOnObject - eye.xyz),
						normalize(calculateReflectRay(pointOnObject, objectId, lightSourceId))
					)), // TODO check if should use max(0, V*R)
				objectShininessFactor(objectId)
		)
		* lightsIntensity[lightSourceId].xyz; // FIXME

		
	return clamp(color, 0, 1);
}


// FIXME : think about the implications of the cutoff angle of a spotlight
vec3 calculateReflectRay(vec3 pointOnObject, int objectId, int lightSourceId) {
	vec3 correctedNormal;
	vec3 rayFromSurfaceToLight;
	
	if (lightIsDirectional(lightSourceId)) {
		rayFromSurfaceToLight = lightsDirection[lightSourceId].xyz;
	}
	else {
		rayFromSurfaceToLight = pointOnObject - lightsPosition[lightSourceId].xyz;
	}

	correctedNormal = calculateNormalToSurface(pointOnObject, objectId);
	if (dot(rayFromSurfaceToLight, correctedNormal) < dot(rayFromSurfaceToLight, -correctedNormal)) {
		correctedNormal = -correctedNormal;
	}

	return 2 * dot(correctedNormal, rayFromSurfaceToLight) * correctedNormal - rayFromSurfaceToLight;
}

///


/// [x] INTERSECTION calculations
vec4 rayObjectIntersection(vec3 originPoint, vec3 normalizedDirection, int objectIndex) {
	if (objectIsSphere(objectIndex)) {
		return raySphereIntersection(originPoint, normalizedDirection, objects[objectIndex]);
	}
	else {
		return rayPlaneIntersection(originPoint, normalizedDirection, objects[objectIndex]);
	}
}

// calculates nearest point of intersection of ray from origin with sphere
vec4 raySphereIntersection(vec3 originPoint, vec3 normalizedDirection, vec4 sphere) {
	// vec4 sphere = vec4(0.6, 0.5, -1.0, 0.5);
	vec3 V = normalizedDirection;
	vec3 P0O = centerOfSphere(sphere) - originPoint; // L vec
	// vec3 ray = position0 - originPoint; // FIXME : should be passed as a parameter

	// vec3 direction = normalize(ray);

	float tm = dot(normalizedDirection, P0O);
	float dSquared = dot(P0O, P0O) 
					- tm*tm;

	// float Th = sqrt(radiusOfSphere(sphere)*radiusOfSphere(sphere) - squaredDistanceOfCenterFromRay);

	float t = tm 
			- sqrt(radiusOfSphere(sphere)*radiusOfSphere(sphere) - dSquared);
	
	vec3 intersectionPoint = originPoint + t*normalizedDirection;

	return vec4(intersectionPoint, t);
}


vec4 rayPlaneIntersection(vec3 originPoint, vec3 normalizedDirection, vec4 plane) {
	vec3 Q0 = getPointOnPlane(plane);
	vec3 planeNormal = plane.xyz;

	float t = dot(
		planeNormal, 
		(Q0 - originPoint)/dot(planeNormal,normalizedDirection)
	);

	vec3 intersectionPoint = originPoint + t*normalizedDirection;

	return vec4(intersectionPoint, t);
}

///


/// [x] HELPER functions

vec3 centerOfSphere(vec4 sphere) {
	return sphere.xyz;
}


float radiusOfSphere(vec4 sphere) {
	return sphere[3];
}


float objectShininessFactor(int objectId) {
	return objColors[objectId][3];
}


vec3 calculateNormalToSurface(vec3 pointOnObject, int objectId) {
	if (objectIsSphere(objectId)) {
		return normalize(pointOnObject - objects[objectId].xyz);
	}
	else {	// object is a plane
		return normalize(objects[objectId].xyz);
	}
}


bool lightReachesObject(int lightSourceId, vec3 pointOnObject, int objectId) {
	// FIXME consider reflection of light from mirror surface???
	vec3 normalizedLightDirection = normalize(lightsDirection[lightSourceId].xyz);
	
	// if light is directional, return true if not hitting ANY objects on the opposite direction of the light
	int numOfObjects = sizes[0];
	if (lightIsDirectional(lightSourceId)) {
		for (int objectIndex = 0; objectIndex < numOfObjects; ++objectIndex) {
			if (objectId != objectIndex
				&& rayObjectIntersection(pointOnObject, -normalizedLightDirection, objectIndex)[3] > 0) {

				return false;
			}
		}
	}
	else { // light is a spot light
		// if angle between (light direction) and (ray from point to it) is bigger than cut-off-angle, then false
		vec3 rayFromPointToSP = pointOnObject - lightsPosition[lightSourceId].xyz;
		float cosCutoffAngle = lightsPosition[lightSourceId].w;
		
		float cosAngle = dot(-normalizedLightDirection, rayFromPointToSP)
					/*/ dot(lightDirection,lightDirection)*/
					/ dot(rayFromPointToSP,rayFromPointToSP);

		if (cosAngle < cosCutoffAngle) { // if cos is smaller, angle is bigger
			return false;
		}
		// if reaching an object before reaching light position, then false

		// FIXME : keep a track of index of spot light, to ensure correct indexing
		float distancePointFromSL = length(pointOnObject - lightsPosition[lightSourceId].xyz);

		for (int objectIndex = 0; objectIndex < numOfObjects; ++objectIndex) {
			float t = rayObjectIntersection(pointOnObject, -normalizedLightDirection, objectIndex)[3];

			if (objectId != objectIndex
				&& t < distancePointFromSL) {

				return false;
			}
		}
	}

	// TODO : if light is a spotlight, return true if not hitting any objects, before reaching the light source, on the opposite direction of the light

	return true; // FIXME
}


bool lightIsDirectional(int lightSourceId) {
	return lightsDirection[lightSourceId][3] == 0.0;
}


// bool objectIsSphere(vec4 object) {
// 	return object[3] > 0.0; // by definition of syntax of scene file
// }

bool objectIsSphere(int objectId) {
	return objects[objectId][3] > 0.0; // by definition of syntax of scene file
}


void assert(bool condition, vec4 colorOnTrue) {
	if (condition && length(position0) <= length(vec3(0.1, 0.1, 0.1))) {
		Color = colorOnTrue;
	}
}