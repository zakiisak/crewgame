#pragma once

#define _USE_MATH_DEFINES
#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

namespace zakix {

	const float barryCentric(vec3 p1, vec3 p2, vec3 p3, vec2 pos);
	mat4 createTransformationMatrix(const vec3& translation, float rx, float ry, float rz, float scale);
	mat4 createViewMatrix(vec3 cameraPos, float pitch, float yaw);

}