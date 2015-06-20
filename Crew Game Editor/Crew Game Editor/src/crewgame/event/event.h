#pragma once

namespace zakix {
	class EventList;
	class CrewGame;

	class Event {
	private:
	public:
		virtual void perform(CrewGame* cg, EventList* list) = 0;
	};
}