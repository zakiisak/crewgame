#pragma once
#include "component.h"

#include <vector>

namespace zakix {
	class Panel {
	private:
		static mat4 identityMatrix;
		vec2 offset;
		std::vector<Component*> components;
		
	public:
		Panel(const vec2& offset = vec2(0, 0));

		void update(CrewGame* cg);
		void render(BatchRenderer2D* renderer);

		const vec2& getOffset() const;
		const std::vector<Component*> getComponents() const;
		const Component* getComponent(unsigned int index) const;

		void add(Component* component);
		void remove(unsigned int index);

	};
}