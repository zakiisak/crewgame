#pragma once
#include "../../maths/maths.h"
#include "../event/event.h"

namespace zakix {
	class CrewGame;
	class BatchRenderer2D;
	class Component {
	protected:
		vec2 position;
		vec2 size;
		vec2 offset;
		Event* event;

	public:
		virtual bool update(CrewGame* cg) = 0;
		virtual void render(BatchRenderer2D* renderer) = 0;

		const vec2& getPosition() const;
		const vec2& getSize() const;
		const vec2& getOffset() const;
		Event* getEvent() const;
		void setPosition(const vec2& position);
		void setSize(const vec2& size);
		void setOffset(const vec2& offset);
		void setEvent(Event* event);

		//UTIL FUNCTIONS

		bool contains(const vec2& point);

	};
}