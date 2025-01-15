#include "Timer.h"

Timer::Timer()
{
	previous_time_point = steady_clock().now();
	current_time_point = steady_clock().now();

	/*
	start_time_point = steady_clock().now();
	stop_time_point = steady_clock().now();
	frame_counter = 0;
	*/

	delta_time = 0.0f;
	frames_per_second = 0;
	avg_fps = 0;
}

// Should run every frame
void Timer::UpdateTime()
{
	CalculateDeltaTime();
	CalculateFramesPerSecond();
	CalculateAvgFPS();
}

void Timer::CalculateDeltaTime()
{
	current_time_point = steady_clock().now();
	duration<double> time_in_seconds = current_time_point - previous_time_point;
	previous_time_point = current_time_point;
	delta_time = time_in_seconds.count();
}

void Timer::CalculateFramesPerSecond()
{
	/*
	
	stop_time_point = steady_clock().now();

	if ((stop_time_point - start_time_point).count() >= 0.90f && delta_time > 0.10f)
	{
		frames_per_second = frame_counter;

		frame_counter = 0;
		start_time_point = steady_clock().now();
	}
	else if ((stop_time_point - start_time_point).count() >= 0.90f && delta_time > 0.05f) // 0.06 - 0.10
	{
		// add 1
		frames_per_second = ++frame_counter;

		frame_counter = 0;
		start_time_point = steady_clock().now();
	}
	else if ((stop_time_point - start_time_point).count() >= 0.90f && delta_time >= 0.04f) // 0.04, 0.05
	{
		// add 2
		frames_per_second = 2 + frame_counter;

		frame_counter = 0;
		start_time_point = steady_clock().now();
	}
	else if ((stop_time_point - start_time_point).count() >= 0.90f && delta_time >= 0.02f) // 0.02, 0.03
	{
		// add 3
		frames_per_second = 3 + frame_counter;

		frame_counter = 0;
		start_time_point = steady_clock().now();
	}
	else if ((stop_time_point - start_time_point).count() >= 0.90f && delta_time > 0.00f) // 0.01
	{
		// add 10
		frames_per_second = 10 + frame_counter;

		frame_counter = 0;
		start_time_point = steady_clock().now();
	}
	else
	{
		// Nothing here!
	}

	frame_counter++;
	
	*/

	// ^^^ The above is used when I want to calculate FPS every one second ^^^

	frames_per_second = 1.0f / delta_time;

	// ^^^ The above can be used when I want to calculate FPS every one frame ^^^
}

void Timer::CalculateAvgFPS()
{
	avg_fps = (avg_fps + frames_per_second) / 2;
}