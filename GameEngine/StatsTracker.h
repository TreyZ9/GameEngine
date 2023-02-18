#pragma once

#include <array>

class StatsTracker
{
private:
	std::array<unsigned int, 10> fpsBuffer;
	unsigned int fps = 60;

public:
	StatsTracker();
	~StatsTracker() = default;

	void update(double frameDelta);

	unsigned int getFps() const;
};
