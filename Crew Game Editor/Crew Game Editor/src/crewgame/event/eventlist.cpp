#include "eventlist.h"
#include "../../utils/util_funcs.h"
#include "../crewgame.h"

namespace zakix {
	EventList::EventList() {}

	EventList::EventList(std::string condition) {
		this->condition = condition;
		std::vector<std::string> splitter = split_string(condition, ' ');
		variableIndex = std::atoi(splitter[0].c_str());
		variableCondition = splitter[1];
		if (splitter[2][0] == 'v' || splitter[2][0] == 'V') {
			//Passing condition value by reference to another variable.
			variableReferringToVar = true;
			char* index = new char[splitter[2].size() - 1];
			for (unsigned int i = 1; i < splitter[2].size(); i++) {
				index[i] = splitter[2][i];
			}
			const char* indexToParse = index;
			variableValueCondition = atoi(indexToParse);
		}
	}

	bool EventList::perform(CrewGame* cg) {
		if (!condition.empty()) {
			int value = cg->variables[variableIndex];
			int conditionValue = variableValueCondition;
			if (variableReferringToVar) conditionValue = cg->variables[variableValueCondition];

			if (variableCondition == "==") {
				if (value == conditionValue); else return false;
			}
			else if (variableCondition == "!=") {
				if (value != conditionValue); else return false;
			}
			else if (variableCondition == ">") {
				if (value > conditionValue); else return false;
			}
			else if (variableCondition == "<") {
				if (value < conditionValue); else return false;
			}
			else if (variableCondition == ">=") {
				if (value >= conditionValue); else return false;
			}
			else if (variableCondition == "<=") {
				if (value <= conditionValue); else return false;
			}
		}

		for (unsigned int i = process; i < events.size(); i++) {
			events[i]->perform(cg, this);
			process++;
			if (removed) break;
		}
		return process >= events.size();
	}

	void EventList::performAll(CrewGame* cg) {
		for (unsigned int i = 0; i < events.size(); i++) {
			events[i]->perform(cg, this);
			if (removed) break;
		}
	}

	void EventList::addEvent(Event* event) {
		events.push_back(event);
	}

	const std::vector<Event*> EventList::getEvents() const {
		return events;
	}

	Event* EventList::getEvent(unsigned int index) const {
		return events[index];
	}

	void EventList::setRemoved(bool removed) {
		this->removed = removed;
	}
}