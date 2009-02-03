// (C) 2009 christian.schladetsch.net

#include "Common.h"
#include "Input.h"

void Keyboard::Process(SDL_Event const &event)
{
	if (event.type == SDL_KEYDOWN)
	{
		Key &key = keys[event.key.keysym.sym];
		key.time = 0;//TimeNow();
		key.state = Key::Down;
		key.event = event;
	}
	else if (event.type == SDL_KEYUP)
	{
		Key &key = keys[event.key.keysym.sym];
		key.time = 0;//TODO TimeNow();
		key.state = Key::Up;
		key.event = event;
	}
}

bool Keyboard::IsKeyState(KeyId id, Key::State state) const
{
	Keys::const_iterator key = keys.find(id);
	return key != keys.end() && key->second.state == state;
}

//EOF

