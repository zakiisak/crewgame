#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "loader3d.h"
#include "../../maths/maths.h"
#include <GLFW/glfw3.h>

namespace zakix {
	Loader3D::~Loader3D() {
		dispose();
	}

	void Loader3D::dispose() {
		if (!disposed) {
			for (unsigned int i = 0; i < vaos.size(); i++) {
				glDeleteVertexArrays(1, &vaos.at(i));
			}

			for (unsigned int i = 0; i < vbos.size(); i++) {
				glDeleteBuffers(1, &vbos.at(i));
			}
			disposed = true;
		}
	}


	/**************** VBO änd VAO RELATED ****************/

	GLuint Loader3D::createVAO() {
		GLuint vaoID;
		glGenVertexArrays(1, &vaoID);
		vaos.push_back(vaoID);
		glBindVertexArray(vaoID);
		return vaoID;
	}

	void Loader3D::unbindVAO() {
		glBindVertexArray(0);
	}

	void Loader3D::bindIndicesBuffer(std::vector<GLuint> indices, GLenum usage) {
		GLuint vboID;
		glGenBuffers(1, &vboID);
		vbos.push_back(vboID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)* indices.size(), &indices[0], usage);
	}


	void Loader3D::storeDataInAttributeList(GLint attributeNumber, GLint coordinateSize, std::vector<GLfloat> data, GLenum usage) {
		GLuint vboID;
		glGenBuffers(1, &vboID);
		vbos.push_back(vboID);
		glBindBuffer(GL_ARRAY_BUFFER, vboID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)* data.size(), &data[0], usage);
		glVertexAttribPointer(attributeNumber, coordinateSize, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	RawModel Loader3D::loadToVAO(std::vector<GLfloat> vertices, std::vector<GLfloat> uv, std::vector<GLfloat> normals, std::vector<GLuint> indices) {
		GLuint vaoID = createVAO();
		bindIndicesBuffer(indices);
		storeDataInAttributeList(0, 3, vertices, GL_STATIC_DRAW);
		storeDataInAttributeList(1, 2, uv, GL_STATIC_DRAW);
		storeDataInAttributeList(2, 3, normals, GL_STATIC_DRAW);
		unbindVAO();
		return RawModel(vaoID, indices.size());
	}

	RawModel Loader3D::loadToVAO(std::vector<GLfloat> vertices, GLuint dimensions) {
		GLuint vaoID = createVAO();
		storeDataInAttributeList(0, dimensions, vertices);
		unbindVAO();
		return RawModel(vaoID, vertices.size() / dimensions);
	}

	/**************** OBJ-LOADING RELATED ****************/

	struct vertex {
		std::vector<float> v;
		void normalize() {
			float magnitude = 0.0f;
			for (int i = 0; i < v.size(); i++)
				magnitude += pow(v[i], 2.0f);
			magnitude = sqrt(magnitude);
			for (int i = 0; i < v.size(); i++)
				v[i] /= magnitude;
		}
		vertex operator-(vertex v2) {
			vertex v3;
			if (v.size() != v2.v.size()) {
				v3.v.push_back(0.0f);
				v3.v.push_back(0.0f);
				v3.v.push_back(0.0f);
			}
			else {
				for (int i = 0; i < v.size(); i++)
					v3.v.push_back(v[i] - v2.v[i]);
			}
			return v3;
		}
		vertex cross(vertex v2) {
			vertex v3;
			if (v.size() != 3 || v2.v.size() != 3) {
				v3.v.push_back(0.0f);
				v3.v.push_back(0.0f);
				v3.v.push_back(0.0f);
			}
			else {
				v3.v.push_back(v[1] * v2.v[2] - v[2] * v2.v[1]);
				v3.v.push_back(v[2] * v2.v[0] - v[0] * v2.v[2]);
				v3.v.push_back(v[0] * v2.v[1] - v[1] * v2.v[0]);
			}
			return v3;
		}
	};

	struct face {
		std::vector<int> vertex;
		std::vector<int> texture;
		std::vector<int> normal;
	};




	void processVertex(int v, int vt, int vn, std::vector<GLuint> &indices, std::vector<vec2> &textures, std::vector<vec3> &normals, std::vector<GLfloat> &textureArray, std::vector<GLfloat> &normalsArray) {
		int currentVertexPointer = v - 1;
		if (currentVertexPointer >= 0)
			indices.emplace_back(currentVertexPointer);
		if (vt - 1 < textures.size()) {
			vec2 currentTex = textures[vt - 1];
			textureArray[currentVertexPointer * 2] = currentTex.x;
			textureArray[currentVertexPointer * 2 + 1] = 1 - currentTex.y;
		}
		if (vn > 0) {
			vec3 currentNorm = normals[vn - 1];
			normalsArray[currentVertexPointer * 3] = currentNorm.x;
			normalsArray[currentVertexPointer * 3 + 1] = currentNorm.y;
			normalsArray[currentVertexPointer * 3 + 2] = currentNorm.z;
		}
	}

	RawModel Loader3D::loadModel(std::string filename, bool normalsInversed) {
		double firstTime = glfwGetTime();
		
		std::vector<vec3> vertices;
		std::vector<vec2> uv;
		std::vector<vec3> normals;
		std::vector<GLuint> indices;

		std::vector<GLfloat> verticesArray;
		std::vector<GLfloat> textureArray;
		std::vector<GLfloat> normalsArray;

		bool fNotInit = true;

		float inverseSub = 1.0f;

		char string[64];
		FILE *stream; fopen_s(&stream, filename.c_str(), "r");
		while (!feof(stream))
		{
			fgets(string, 64, stream);
			
			if (string[0] == 'v' && string[1] == ' ') {
				float x;
				float y;
				float z;
				sscanf_s(string, "v %f %f %f", &x, &y, &z);
				vec3 vertex(x, y, z);
				vertices.emplace_back(vertex);
			}
			else if (string[0] == 'v' && string[1] == 't') {
				float x;
				float y;
				sscanf_s(string, "vt %f %f", &x, &y);
				vec2 texture(x, y);
				uv.emplace_back(texture);
			}
			else if (string[0] == 'v' && string[1] == 'n') {
				float x;
				float y;
				float z;
				sscanf_s(string, "vn %f %f %f", &x, &y, &z);
				vec3 normal;
				if (normalsInversed) normal = vec3(1.0f - x, 1.0f - y, 1.0f - z);
				else normal = vec3(x, y, z);
				normals.emplace_back(normal);
			}
			else if (string[0] == 'f' && string[1] == ' ') {
				if (fNotInit) {
					textureArray.resize(vertices.size() * 2);
					normalsArray.resize(vertices.size() * 3);
					fNotInit = false;
				}
				int v1 = 0;
				int vt1 = 0;
				int vn1 = 0;

				int v2 = 0;
				int vt2 = 0;
				int vn2 = 0;

				int v3 = 0;
				int vt3 = 0;
				int vn3 = 0;

				sscanf_s(string, "f %d/%d/%d %d/%d/%d %d/%d/%d", &v1, &vt1, &vn1, &v2, &vt2, &vn2, &v3, &vt3, &vn3);

				processVertex(v1, vt1, vn1, indices, uv, normals, textureArray, normalsArray);
				processVertex(v2, vt2, vn2, indices, uv, normals, textureArray, normalsArray);
				processVertex(v3, vt3, vn3, indices, uv, normals, textureArray, normalsArray);
			}
			
		}

		verticesArray.resize(vertices.size() * 3);
		int vertexPointer = 0;
		for (unsigned int i = 0; i < vertices.size(); i++) {
			vec3 vertex = vertices[i];
			verticesArray[vertexPointer++] = vertex.x;
			verticesArray[vertexPointer++] = vertex.y;
			verticesArray[vertexPointer++] = vertex.z;
		}

		double secondTime = glfwGetTime();
		std::cout << (secondTime - firstTime) * 1000.0 << " ms to load model: [" << filename << "], vertices: " << vertices.size() << std::endl;

		return loadToVAO(verticesArray, textureArray, normalsArray, indices);
	}
}