
/*
* What a Mouse system of an engine should do?
	* Application class side operations:
		* Produce mouse events for:
			* Left Click (Both Press and Release)
			* Right Click (Both Press and Release)
			* Middle Click (Both Press and Release)
			* Scroll Up
			* Scroll Down
			* Drag
			* Move
		* Control behaviors:
			* Move an object or something (window) that can be dragged
			* Limiting cursor within window
			* Behavior when drag and moving something but cursor is outside the client region
	* Game client/code side:
		* Handle produced mouse events:
			* Whether to ignore or take into account an event?
			* What happens for this event?
		* Player control system:
			* Camera / Player character / Camera & Player character control
			* Set of player actions and camera behaviors can be defined
*/

/* Mostly replicating Chilli's mouse system implementation as this is new to me. Might refactor later.*/

#pragma once
#include <queue>

class Mouse
{
	friend class Application; // To allow Application class to access Mouse class's private members.

public:
	class MouseEvents // To contain mouse information within an object when being used on the Game engine/client side.
	{
	public:
		enum class EventType
		{
			NONE,
			LEFT_PRESS,
			LEFT_RELEASE,
			RIGHT_PRESS,
			RIGHT_RELEASE,
			MIDDLE_PRESS,
			MIDDLE_RELEASE,
			WHEEL_UP,
			WHEEL_DOWN,
			INSIDE_CLIENT_REGION,
			OUTSIDE_CLIENT_REGION,
			MOVE,
			DRAG
		};

		// A default Constructor
		MouseEvents()
			:type(EventType::NONE),
			isLeftButtonPressed(false),
			isRightButtonPressed(false),
			x(0),
			y(0)
		{}

		MouseEvents(
			EventType _type,
			bool _isLeftButtonPressed,
			bool _isRightButtonPressed,
			int _x,
			int _y)
			:type(_type),
			isLeftButtonPressed(_isLeftButtonPressed),
			isRightButtonPressed(_isRightButtonPressed),
			x(_x),
			y(_y)
		{};

		EventType GetType() const { return type; }
		bool IsValidEvent() const { return type != EventType::NONE; }
		bool IsLeftButtonPressed() const { return isLeftButtonPressed; }
		bool IsRightButtonPressed() const { return isRightButtonPressed; }
		std::pair<int, int> GetMousePositionForEvent() const { return { x, y }; } // never knew a pair could be written within {,}.
		int GetMouseXPositionForEvent() const { return x; }
		int GetMouseYPositionForEvent() const { return y; }

	private:
		int x;
		int y;
		EventType type;
		bool isLeftButtonPressed;
		bool isRightButtonPressed;
		bool isInsideClientRegion;
		bool isOutsideClientRegion;
	};

	// A default constructor
	Mouse() = default;
	// Deleting copy constructor
	Mouse(const Mouse&) = delete;
	// Deleting overloaded assignment operator
	Mouse& operator=(const Mouse&) = delete;

#pragma region Stuff that can be used by CLIENT/GAME side:
	bool IsLeftButtonPressed() { return isLeftButtonPressed; }
	bool IsRightButtonPressed() { return isRightButtonPressed; }
	bool IsInsideClientRegion() { return isInsideClientRegion; }
	bool IsOutsideClientRegion() { return isOutsideClientRegion; }
	std::pair<int, int> GetMousePosition() { return { x, y }; } // never knew a pair could be written within {,}.
	int GetMouseXPosition() { return x; }
	int GetMouseYPosition() { return y; }
	int GetMouseScrollsNumber() { return noOfScrollsUpAndDown; }
	MouseEvents ReadMouseEvent();
	bool IsMouseEventsQueueEmpty() { return mouseEventsQueue.empty(); }
	void FlushMouseEventsQueue();
#pragma endregion

#pragma region Functions that the Application Class would use. Mostly private:
private:
	void OnMouseLeftPressed(int _x, int _y);
	void OnMouseLeftReleased(int _x, int _y);
	void OnMouseRightPressed(int _x, int _y);
	void OnMouseRightReleased(int _x, int _y);
	void OnMouseWheelUp(int _x, int _y);
	void OnMouseWheelDown(int _x, int _y);
	void OnMouseWheelDelta(int _x, int _y, int _delta);
	void OnMouseMove(int _x, int _y);
	void OnMouseDrag(int _x, int _y);
	void OnMouseInsideClientRegion(int _x, int _y);
	void OnMouseOutsideClientRegion(int _x, int _y);
	void TrimBuffer();

	int x;
	int y;
	int wheelDelta;
	int noOfScrollsUpAndDown;
	bool isLeftButtonPressed = false;
	bool isRightButtonPressed = false;
	bool isInsideClientRegion = false;
	bool isOutsideClientRegion = false;
	static const unsigned int buffersize = 16u;
	std::queue<MouseEvents> mouseEventsQueue;
#pragma endregion
};