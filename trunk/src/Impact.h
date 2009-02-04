// (C) 2009 christian.schladetsch@gmail.com

#ifndef IMPACT_H_INCLUDED
#define IMPACT_H_INCLUDED

/// a visual representation of the impact of an object onto another object, or the playfield
struct Impact : Object
{
//	Vector color_start, color_end;
	float time_to_die;			////< when this impact effect is to be removed

	bool Update(GameTime);
	void Draw(Matrix const &);
};

#endif // IMPACT_H_INCLUDED

//EOF
