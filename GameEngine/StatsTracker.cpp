#include "StatsTracker.h"

#include <algorithm>
#include <numeric>
#include <iostream>

StatsTracker::StatsTracker()
{
	this->fpsBuffer.fill(60);
}

void StatsTracker::update(double frameDelta)
{
	unsigned int fps = (unsigned int)(1 / frameDelta);
	std::rotate(this->fpsBuffer.begin(), this->fpsBuffer.begin() + this->fpsBuffer.size() - 1, this->fpsBuffer.end());
	this->fpsBuffer.at(0) = fps;

	this->fps = std::reduce(this->fpsBuffer.begin(), this->fpsBuffer.end()) / this->fpsBuffer.size();
}

unsigned int StatsTracker::getFps() const
{
	return this->fps;
}