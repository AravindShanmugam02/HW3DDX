
/*
* What a keyboard system of an engine should do?
	* Application Class side operations:
		* Produce Keyboard Events:
			* KeyCode Events
				* KEY DOWN (Key Press & Which Key)
				* KEY UP (Key Release & Which Key)
			* Character Events
				* KEY DOWN (Key Press & Which Char)
				* KEY UP (Key Release & Which Char)
		* Provide Control For Other Functions:
			* Auto Repeat Function For Keyboard
		* Define Follow Up Operations:
			* On Key Pressed
			* On Key Released
			* On Char (If a char event is read)
	* Game client side operations:
		* Handle Produced Keyboard Events:
			* What Should Be Done On A Key Press
			* How Does The Engine Should React For A Key Press
		* Conditional Operations:
			* Check If A Key Is Pressed
*/

/* Sort of replicating Chilli's Keyboard system as this is new to me. Might refactor later.*/

#pragma once
#include <bitset>
#include <queue>

class Keyboard
{
	friend class Application; // To allow Application class to access Keyboard class's private members.

public:
	class KeyboardEvents // To contain keyboard information within an object when being used on the Game engine/client side.
	{
	public:
		enum class EventType
		{
			NONE,
			KEY_DOWN,
			KEY_UP
		};

		// A Default Constructor
		KeyboardEvents()
			:type(EventType::NONE),
			keyCode(0u)
		{}

		KeyboardEvents(EventType _type, unsigned char _keyCode)
			:type(_type),
			keyCode(_keyCode)
		{}

		bool IsPressEvent() { return type == EventType::KEY_DOWN; }
		bool IsReleaseEvent() { return type == EventType::KEY_UP; }
		bool IsValidEvent() { return type != EventType::NONE; }
		unsigned char GetCode() { return keyCode; }

	private:
		EventType type;
		unsigned char keyCode;
	};

public:
	// A default constructor
	Keyboard() = default;
	// Deleting copy constructor
	Keyboard(const Keyboard&) = delete;
	// Deleting overloaded assignment operator
	Keyboard& operator=(const Keyboard&) = delete;

	// Remember that the Application Class which talks to WINDOWS API side is not going to handle the events.
	// It is going to produce events.
	// It is the work of the engine/client to handle the events produced by Application class talking to WINDOWS API.

#pragma region Stuff that can be used by CLIENT/GAME side:
	bool IsKeyboardKeyPressed(unsigned char keycode);
	KeyboardEvents ReadKeyboardKeyEvent();
	bool IsKeyboardKeyEventQueueEmpty() { return keyboardKeyEventsBuffer.empty(); }
	void FlushKeyboardKeyEventQueue();

	unsigned char ReadKeyboardCharEvent();
	bool IsKeyboardCharEventQueueEmpty() { return keyboardCharEventsBuffer.empty(); }
	void FlushKeyboardCharEventQueue();

	void FlushKeyboardKeyAndCharEventQueues();

	void EnableAutoRepeat() { autoRepeatEnabled = true; }
	void DisableAutoRepeat() { autoRepeatEnabled = false; }
	bool IsAutoRepeatEnabled() { return autoRepeatEnabled; }
#pragma endregion

#pragma region Functions that the MyWindows Class would use. Mostly private:
private:
	void OnKeyPressed(unsigned char keycode);
	void OnKeyReleased(unsigned char keycode);
	void OnCharacter(unsigned char character);
	void ClearKeyboardKeyStates();

	// Making it a template so that both the KeyEventsBuffer and CharEventsBuffer can be used with this function.
	template<typename T>
	void TrimBuffer(std::queue<T>& bufferToTrim);

	static const unsigned int nKeys = 256u;
	std::bitset<nKeys> keyboardKeyStates; // When nothing is initialized for a bitset<> it defaults all the values to 0.

	static const unsigned int bufferSize = 16u;
	std::queue<KeyboardEvents> keyboardKeyEventsBuffer;	
	std::queue<char> keyboardCharEventsBuffer;

	bool autoRepeatEnabled = false;
#pragma endregion
};