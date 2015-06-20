#include "glFuncs.h"

namespace zakix {
	void enableCulling() {
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	}

	void disableCulling() {
		glDisable(GL_CULL_FACE);
	}
}