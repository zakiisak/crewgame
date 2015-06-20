#pragma once
#include <GL/glew.h>

namespace zakix {
	class RawModel {
	private:
		GLuint vaoID;
		GLuint vertexCount;
	public:
		RawModel();
		RawModel(GLuint vaoID, GLuint vertexCount);

		inline const GLuint getVaoID() const { return vaoID; }
		inline const GLuint getVertexCount() const { return vertexCount; }
	};
}