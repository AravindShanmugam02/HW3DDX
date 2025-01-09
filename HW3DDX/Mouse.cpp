#include "Mouse.h"
#include <Windows.h>

Mouse::MouseEvents Mouse::ReadMouseEvent()
{
	if (mouseEventsQueue.size() > 0u)
	{
		MouseEvents e = mouseEventsQueue.front();
		mouseEventsQueue.pop();
		return e;
	}

	return MouseEvents(); // Returns an object for MouseEvents with default invalid value.
}

void Mouse::FlushMouseEventsQueue()
{
	while (!mouseEventsQueue.empty())
	{
		mouseEventsQueue.pop();
	}
}

void Mouse::OnMouseLeftPressed(int _x, int _y)
{
	isLeftButtonPressed = true;
	x = _x;
	y = _y;
	mouseEventsQueue.push(
		MouseEvents(
			MouseEvents::EventType::LEFT_PRESS,
			IsLeftButtonPressed(),
			IsRightButtonPressed(),
			x, y)
	);

	TrimBuffer();
}

void Mouse::OnMouseLeftReleased(int _x, int _y)
{
	isLeftButtonPressed = false;
	x = _x;
	y = _y;
	mouseEventsQueue.push(
		MouseEvents(
			MouseEvents::EventType::LEFT_RELEASE,
			IsLeftButtonPressed(),
			IsRightButtonPressed(),
			x, y)
	);

	TrimBuffer();
}

void Mouse::OnMouseRightPressed(int _x, int _y)
{
	isRightButtonPressed = true;
	x = _x;
	y = _y;
	mouseEventsQueue.push(
		MouseEvents(
			MouseEvents::EventType::RIGHT_PRESS,
			IsLeftButtonPressed(),
			IsRightButtonPressed(),
			x, y)
	);

	TrimBuffer();
}

void Mouse::OnMouseRightReleased(int _x, int _y)
{
	isRightButtonPressed = false;
	x = _x;
	y = _y;
	mouseEventsQueue.push(
		MouseEvents(
			MouseEvents::EventType::RIGHT_RELEASE,
			IsLeftButtonPressed(),
			IsRightButtonPressed(),
			x, y)
	);

	TrimBuffer();
}

void Mouse::OnMouseWheelUp(int _x, int _y)
{
	x = _x;
	y = _y;
	mouseEventsQueue.push(
		MouseEvents(
			MouseEvents::EventType::WHEEL_UP,
			IsLeftButtonPressed(),
			IsRightButtonPressed(),
			x, y)
	);

	TrimBuffer();
}

void Mouse::OnMouseWheelDown(int _x, int _y)
{
	x = _x;
	y = _y;
	mouseEventsQueue.push(
		MouseEvents(
			MouseEvents::EventType::WHEEL_DOWN,
			IsLeftButtonPressed(),
			IsRightButtonPressed(),
			x, y)
	);
	
	TrimBuffer();
}

void Mouse::OnMouseWheelDelta(int _x, int _y, int _delta)
{
	wheelDelta += _delta;
	// We are sending events only when wheelDelta reaches -120 or more / 120 or more.
	// WHEEL_DELTA is 120 by default.
	while (wheelDelta >= WHEEL_DELTA)
	{
		wheelDelta -= WHEEL_DELTA;
		noOfScrollsUpAndDown++;
		OnMouseWheelUp(_x, _y);
	}

	while (wheelDelta <= -WHEEL_DELTA)
	{
		wheelDelta += WHEEL_DELTA;
		noOfScrollsUpAndDown--;
		OnMouseWheelDown(_x, _y);
	}
}

void Mouse::OnMouseMove(int _x, int _y)
{
	x = _x;
	y = _y;
	mouseEventsQueue.push(
		MouseEvents(
			MouseEvents::EventType::MOVE,
			IsLeftButtonPressed(),
			IsRightButtonPressed(),
			x, y)
	);

	TrimBuffer();
}

void Mouse::OnMouseDrag(int _x, int _y)
{
	x = _x;
	y = _y;
	mouseEventsQueue.push(
		MouseEvents(
			MouseEvents::EventType::DRAG,
			IsLeftButtonPressed(),
			IsRightButtonPressed(),
			x, y)
	);

	TrimBuffer();
}

void Mouse::OnMouseInsideClientRegion(int _x, int _y)
{
	isInsideClientRegion = true;
	isOutsideClientRegion = false;
	x = _x;
	y = _y;
	mouseEventsQueue.push(
		MouseEvents(
			MouseEvents::EventType::INSIDE_CLIENT_REGION,
			IsLeftButtonPressed(),
			IsRightButtonPressed(),
			x, y)
	);

	TrimBuffer();
}

void Mouse::OnMouseOutsideClientRegion(int _x, int _y)
{
	isInsideClientRegion = false;
	isOutsideClientRegion = true;
	x = _x;
	y = _y;
	mouseEventsQueue.push(
		MouseEvents(
			MouseEvents::EventType::OUTSIDE_CLIENT_REGION,
			IsLeftButtonPressed(),
			IsRightButtonPressed(),
			x, y)
	);

	TrimBuffer();
}

void Mouse::TrimBuffer()
{
	while (mouseEventsQueue.size() > buffersize)
	{
		mouseEventsQueue.pop();
	}
}