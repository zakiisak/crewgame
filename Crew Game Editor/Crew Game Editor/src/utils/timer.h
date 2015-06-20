#pragma once

namespace zakix {
	class Timer {
	private:
		double elapsed;
	public:
		void reset();
		double getElapsedSeconds() const;
		double getElapsedMilli() const;
		double getElapsedMicro() const;
		double getElapsedNano() const;
	};
}