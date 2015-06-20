#pragma once
#include "camera.h"

namespace zakix {
	class FirstPersonCamera : public Camera{
	private:
		bool grabbed = false;
		double mouseX = 0, mouseY = 0;
	public:
		void (*controller)(Window* window, FirstPersonCamera* camera);
		
		FirstPersonCamera();
		FirstPersonCamera(void (*controller) (Window* window, FirstPersonCamera* camera));
		virtual void update(World3D* world, Window* window);
		inline const bool isGrabbed() const { return grabbed; }
		void setGrabbed(bool grabbed);
	};
}