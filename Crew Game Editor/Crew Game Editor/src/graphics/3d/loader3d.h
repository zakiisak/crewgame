#pragma once
#include "model/rawmodel.h"
#include <string>
#include <vector>
#include <GL/glew.h>

namespace zakix {
	class Loader3D {
	private:
		std::vector<GLuint> vaos;
		std::vector<GLuint> vbos;
		bool disposed = false;
	public:
		~Loader3D();
		RawModel loadToVAO(std::vector<GLfloat> vertices, std::vector<GLfloat> uv, std::vector<GLfloat> normals, std::vector<GLuint> indices);
		RawModel loadToVAO(std::vector<GLfloat> vertices, GLuint dimensions);
		RawModel loadModel(std::string filename, bool normalsInversed = false);
		void dispose();
	private:
		GLuint createVAO();
		void storeDataInAttributeList(GLint attributeNumber, GLint coordinateSize, std::vector<GLfloat> data, GLenum usage = GL_STATIC_DRAW);
		void unbindVAO();
		void bindIndicesBuffer(std::vector<GLuint> indices, GLenum usage = GL_STATIC_DRAW);
	};
}