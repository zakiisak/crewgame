#include "component.h"

namespace zakix {
	const vec2& Component::getPosition() const {
		return position;
	}

	const vec2& Component::getSize() const {
		return size;
	}

	const vec2& Component::getOffset() const {
		return offset;
	}

	void Component::setPosition(const vec2& position) {
		this->position = position;
	}

	Event* Component::getEvent() const {
		return event;
	}

	void Component::setSize(const vec2& size) {
		this->size = size;
	}

	void Component::setOffset(const vec2& offset) {
		this->offset = offset;
	}

	void Component::setEvent(Event* event) {
		this->event = event;
	}


	bool Component::contains(const vec2& point) {
		float x1 = position.x + offset.x;
		float y1 = position.y + offset.y;
		float x2 = point.x;
		float y2 = point.y;
		float x3 = x1 + size.x;
		float y3 = y1 + size.y;
		return x2 >= x1 && y2 >= y1 && x2 <= x3 && y2 <= y3;
	}
}