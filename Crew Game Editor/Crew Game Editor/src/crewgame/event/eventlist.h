#pragma once
#include "event.h"
#include <vector>

namespace zakix {
	class EventList {
	private:
		std::vector<Event*> events;
		std::string condition = "";
		bool removed = false;
		unsigned int process = 0;

		unsigned int variableIndex = 0;
		int			 variableValueCondition = 0;
		std::string  variableCondition = "";
		bool		 variableReferringToVar = false;


	public:
		EventList();
		EventList(std::string condition);

		bool perform(CrewGame* cg);
		void performAll(CrewGame* cg);

		void addEvent(Event* event);

		const std::vector<Event*> getEvents() const;
		Event* getEvent(unsigned int index) const;
		const inline bool isRemoved() const { return removed; }
		void setRemoved(bool removed);
	};
}