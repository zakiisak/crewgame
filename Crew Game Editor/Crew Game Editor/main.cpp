#define _CREW_GAME_


#if defined(_CREW_GAME_)

#include "src/crewgame/crewgame.h"

using namespace zakix;

int main() {
	CrewGame game;
	bool res = game.start();
	return res;
}
#endif