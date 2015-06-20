#include "panel.h"
#include "../crewgame.h"

namespace zakix {
	mat4 Panel::identityMatrix = mat4(1.0f);

	Panel::Panel(const vec2& offset) {
		this->offset = offset;
	}

	void Panel::update(CrewGame* cg) {
		for (unsigned int i = 0; i < components.size(); i++) {
			Component* c = components[i];
			c->setOffset(offset);
			if (c->update(cg)) {
				if (c->getEvent() != nullptr) c->getEvent()->perform(cg, 0);
			}
		}
	}

	void Panel::render(BatchRenderer2D* renderer) {
		renderer->begin(RENDER_2D_MODE);
		renderer->applyTransformation(identityMatrix);
		for (unsigned int i = 0; i < components.size(); i++) {
			Component* c = components[i];
			c->render(renderer);
		}
		renderer->end();
		renderer->flush();
	}

	const vec2& Panel::getOffset() const {
		return offset;
	}
	const std::vector<Component*> Panel::getComponents() const {
		return components;
	}
	const Component* Panel::getComponent(unsigned int index) const {
		return components[index];
	}

	void Panel::add(Component* component) {
		components.push_back(component);
	}

	void Panel::remove(unsigned int index) {
		assert(index < components.size());

		if (index + 1 != components.size())
			std::swap(components[index], components.back());

		components.pop_back();
	}
}