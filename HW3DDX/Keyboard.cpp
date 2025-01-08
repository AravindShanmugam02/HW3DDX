#include "Keyboard.h"

bool Keyboard::IsKeyboardKeyPressed(unsigned char keycode)
{
	return keyboardKeyStates.test(keycode); //keyboardKeyStates[keycode]
}

Keyboard::KeyboardEvents Keyboard::ReadKeyboardKeyEvent()
{
	if (keyboardKeyEventsBuffer.size() > 0u)
	{
		KeyboardEvents e = keyboardKeyEventsBuffer.front();
		keyboardKeyEventsBuffer.pop();
		return e;
	}
	
	return KeyboardEvents(); // Returns an object for KeyboardEvents with default invalid value.
}

void Keyboard::FlushKeyboardKeyEventQueue()
{
	while (!keyboardKeyEventsBuffer.empty())
	{
		keyboardKeyEventsBuffer.pop();
	}
}

unsigned char Keyboard::ReadKeyboardCharEvent()
{
	if (keyboardCharEventsBuffer.size() > 0u)
	{
		unsigned char c = keyboardCharEventsBuffer.front();
		keyboardCharEventsBuffer.pop();
		return c;
	}

	return 0;
}

void Keyboard::FlushKeyboardCharEventQueue()
{
	while (!keyboardCharEventsBuffer.empty())
	{
		keyboardCharEventsBuffer.pop();
	}
}

void Keyboard::FlushKeyboardKeyAndCharEventQueues()
{
	FlushKeyboardKeyEventQueue();
	FlushKeyboardCharEventQueue();
}

void Keyboard::OnKeyPressed(unsigned char keyCode)
{
	// First to set bitset keycode
	keyboardKeyStates[keyCode] = true;
	keyboardKeyEventsBuffer.push(KeyboardEvents(KeyboardEvents::EventType::KEY_DOWN, keyCode));
	TrimBuffer(keyboardKeyEventsBuffer);
}

void Keyboard::OnKeyReleased(unsigned char keyCode)
{
	// First to set bitset keycode
	keyboardKeyStates[keyCode] = false;
	keyboardKeyEventsBuffer.push(KeyboardEvents(KeyboardEvents::EventType::KEY_UP, keyCode));
	TrimBuffer(keyboardKeyEventsBuffer);
}

void Keyboard::OnCharacter(unsigned char character)
{
	keyboardCharEventsBuffer.push(character);
	TrimBuffer(keyboardCharEventsBuffer);
}

void Keyboard::ClearKeyboardKeyStates()
{
	keyboardKeyStates.reset();
}

template <typename T>
void Keyboard::TrimBuffer(std::queue<T>& bufferToTrim)
{
	while (bufferToTrim.size() > bufferSize)
	{
		bufferToTrim.pop();
	}
}