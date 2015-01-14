/*
 * Author: KK <thumbd03803@gmail.com>
 *
 * File: timer.cpp
 */

#ifndef TIMER_H
#define TIMER_H

#include <iostream>
#include <SDL2/SDL.h>

class Timer
{
public:
	Timer();
	~Timer();

	void start();
	void pause();
	void stop();
	void unpause();

	int getTicks();

	bool isPaused();
	bool isStarted();

private:
	int startTicks_;

	int pauseTicks_;

	bool isPaused_;
	bool isStarted_;
};

#endif	/* TIMER_H */
