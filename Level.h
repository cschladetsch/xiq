// (C) 2009 christian.schladetsch@gmail.com

#ifndef LEVEL_H_INCLUDED
#	define LEVEL_H_INCLUDED

/// @brief a description of a level in the game
///
/// contains the initial conditions for the layout of objects on
/// a playfield
struct Level : Object
{
	Time time_last_impact;
	Time min_impact_time;

	Level();

	// Object overloads
	bool Update(GameTime);
	void Draw(Matrix const &);

	// list<object construct params>
	void Start();
};

#endif // LEVEL_H_INCLUDED

//EOF
