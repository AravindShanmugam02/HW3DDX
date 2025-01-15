#pragma once
#include <chrono>

using namespace std::chrono;

/* (ashanmugam [TO-DO] Fix My TimeSteps)
* I tried my best to understand Gaffer On Games article about Fix your timestep.
* Unfortunately, I don't understand it beyond the point of using variable delta time.
* I have it implemented in my CalculateDeltaTime function.
* But, I should take time or ask help to understand the whole tips about semi-fixed,
fixed and making systems independent mentioned on the article.
* This is something I should work on. https://gafferongames.com/post/fix_your_timestep/
*/

class Timer
{
public:
	Timer();
	~Timer() {};

	// Deleting copy constructor
	Timer(const Timer&) = delete;

	double GetDt() { return delta_time; };
	double GetFPS() { return frames_per_second; };
	double GetAvgFPS() { return avg_fps; };

	void UpdateTime();

private:
	void CalculateDeltaTime();
	void CalculateFramesPerSecond();
	void CalculateAvgFPS();

	// new time point
	steady_clock::time_point current_time_point;
	// old time point
	steady_clock::time_point previous_time_point;
	// time between each frame in seconds
	double delta_time;

	/*
	// Start time point for a second count
	steady_clock::time_point start_time_point;
	// Stop time point for a second count
	steady_clock::time_point stop_time_point;
	// Frame counter
	unsigned int frame_counter;
	*/

	// Frames in the last second
	double frames_per_second;
	// Avg frames per second
	double avg_fps;
};