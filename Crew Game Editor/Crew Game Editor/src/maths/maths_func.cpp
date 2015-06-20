#include "maths_func.h"

namespace zakix {

	const float barryCentric(vec3 p1, vec3 p2, vec3 p3, vec2 pos) {
		float det = (p2.z - p3.z) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.z - p3.z);
		float l1 = ((p2.z - p3.z) * (pos.x - p3.x) + (p3.x - p2.x) * (pos.y - p3.z)) / det;
		float l2 = ((p3.z - p1.z) * (pos.x - p3.x) + (p1.x - p3.x) * (pos.y - p3.z)) / det;
		float l3 = 1.0f - l1 - l2;
		return l1 * p1.y + l2 * p2.y + l3 * p3.y;
	}

	mat4 createTransformationMatrix(const vec3& translation, float rx, float ry, float rz, float scale) {
		glm::mat4 matrix;
		matrix = translate(glm::mat4(1.0f), translation);
		matrix = rotate(matrix, glm::radians(rx), glm::vec3(1, 0, 0));
		matrix = rotate(matrix, glm::radians(ry), glm::vec3(0, 1, 0));
		matrix = rotate(matrix, glm::radians(rz), glm::vec3(0, 0, 1));
		matrix = glm::scale(matrix, vec3(scale, scale, scale));
		return matrix;
	}

	mat4 createViewMatrix(vec3 cameraPos, float pitch, float yaw) {
		glm::mat4 viewMatrix;
		viewMatrix = glm::rotate(viewMatrix, glm::radians(pitch), glm::vec3(1, 0, 0));
		viewMatrix = glm::rotate(viewMatrix, glm::radians(yaw), glm::vec3(0, 1, 0));
		glm::vec3 negativeCameraPos(-cameraPos.x, -cameraPos.y, -cameraPos.z);
		viewMatrix = glm::translate(viewMatrix, negativeCameraPos);
		return viewMatrix;
	}
}