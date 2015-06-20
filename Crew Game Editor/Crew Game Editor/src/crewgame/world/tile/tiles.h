#pragma once

namespace zakix {
	class CrewGame;

	class Tiles {
	private:
		static void createTileset();
	public:
		static void load(CrewGame* cg);
		static void dispose(CrewGame* cg);
	private:
		Tiles() {}
	};
}