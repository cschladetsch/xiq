// (C) 2009 www.christian.schladetsch.net

#ifndef STYX_H_INCLUDED
#define STYX_H_INCLUDED

/// These enemies wander around on created line segments
struct Styx : Object
{
	Sinusoid rot;

	Styx();
	bool Update(GameTime);
	void Draw(Matrix const &);

protected:
	void Draw(Matrix const &M, Color);
	void DrawLine(LineSegment const &, Color);
};

#endif // STYX_H_INCLUDED
