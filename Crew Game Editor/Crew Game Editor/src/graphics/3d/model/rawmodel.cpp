#include "rawmodel.h"


namespace zakix {
	RawModel::RawModel() {}
	RawModel::RawModel(GLuint vaoID, GLuint vertexCount) {
		this->vaoID = vaoID;
		this->vertexCount = vertexCount;
	}
}