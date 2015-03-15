// (C) 2009 christian.schladetsch.net

#ifndef INPUT_H_INCLUDED
#define INPUT_H_INCLUDED

/// @brief maintains a persistent record of the keyboard state
///
/// this structure keeps track of key up and key down events.
/// it uses this state to answer queries about the current keyboard
/// state, like "is this key down". as such, there is a danger
/// that if a key-up event is somehow missed, queries will return
/// false information.
struct Keyboard
{
	/// represents the current state of a key
	struct Key
	{
		enum State { Up, Down, None };
		Time time;			///< the time in millisends that the event occured
		State state;		///< current key state
		SDL_Event event;	///< the original event that led to this state

		Key() : time(0), state(None) { memset(&event, 0, sizeof(event)); }
	};

	typedef int KeyId;					///< an identifier for a key on the board
	typedef std::map<KeyId, Key> Keys;	///< the internel keyboard representation

private:
	Keys keys;

public:
	/// returns the key record given its id. use the SDLK_* defines
	Key &GetKey(KeyId id) { return keys[id]; }

	/// process an SDL input event. updates the persistent state of the keyboard
	void Process(SDL_Event const &);

	/// clear the state of the keyboard
	void Reset() { keys.clear(); }

	/// returns true if the given key is down
	bool KeyDown(KeyId id) const { return IsKeyState(id, Key::Down); }

	/// returns true if the given key is up
	bool KeyUp(KeyId id) const { return IsKeyState(id, Key::Down); }

	/// returns true if the given key has a specified state
	bool IsKeyState(KeyId, Key::State state) const;
};

#endif // INPUT_H_INCLUDED
