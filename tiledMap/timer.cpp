/*
 * Author: KK <thumbd03803@gmail.com>
 *
 * File: timer.h
 */

#include "timer.h"

Timer::Timer()
{
	startTicks_ = 0;
	pauseTicks_ = 0;
	
	isStarted_ = false;
	isPaused_ = false;
}

Timer::~Timer()
{
}

void
Timer::start()
{
	isStarted_ = true;
	isPaused_ = false;
	
	/* Get the start ticks */
	startTicks_ = SDL_GetTicks();
}

void
Timer::pause()
{
	if ((isStarted_ == true) && (isPaused_ == false)) {
		isPaused_ = true;
		pauseTicks_ = SDL_GetTicks() - startTicks_;
	}
}

void
Timer::unpause()
{
	if (isPaused_ == true) {
		isPaused_ = false;

		startTicks_ = SDL_GetTicks() - pauseTicks_;
		pauseTicks_ = 0;

	}
}
		
int
Timer::getTicks()
{
	if (isStarted_ == true) {
		if (isPaused_ == false)
			return SDL_GetTicks() - startTicks_;
		else
			return pauseTicks_;
	}

	return 0;
}

bool
Timer::isPaused()	
{
	return isPaused_;
}

bool
Timer::isStarted()	
{
	return isStarted_;
}
