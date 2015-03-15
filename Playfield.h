// (C) 2009 christian.schladetsch@gmail.com

#ifndef PLAYFIELD_H_INCLUDED
#define PLAYFIELD_H_INCLUDED

/// @brief the logical representation of the playfield.
///
/// the playfield is a raster-space superimposed on the visual frame-buffer.
/// each element in the playfield
/// contains an enumerated value; this value determines if a given
/// element on the playfield is empty, is filled, or is an edge or a line
struct Playfield : Object
{
	enum Element { Empty = 0, Filled = 1, Line = 2, NewLine = 3, Edge  = 4, Fill0 = 5, Fill1 = 6, None };
	typedef std::vector<Element> Elements;

private:
	int width;
	int height;
	Elements elements;
	float percent_filled;

public:
	/// creates a playfield with the given dimension
	void Create(int width, int height);

	// Object overloads
	bool Update(GameTime);
	void Draw(Matrix const &);

	float GetPercentFilled() const { return percent_filled; }

	/// plot the trajectory against the playfield. this will fire events,
	/// and possibly clip the movement
	/// \param vector the desired vector of movement for the player
	/// \param player_state the current state of the player. whether drawing new line, etc
	void MovePlayer(Player &player, GameTime time, Element player_state);

	/// reset the playfield to empty
	void Clear();

	/// return true if the given point is outside the playfield extents
	bool OutOfBounds(Point p) const;

	/// retrieve the element at a given location in the playfield
	Element At(Point const &P) const { return At(P.x, P.y); }
	Element At(int x, int y) const
	{
		return OutOfBounds(Point(x,y)) ? None : elements.at(x + y*width);
	}

	int GetWidth() const { return width; }
	int GetHeight() const { return height; }

	/// remove all occurances of NewLine from the playfield, replacing them with Line elements
	void RemoveNewLines(Element = Line);

	/// return end of sequence of points by rasterising linesegment p0->p1
	Point *LineDraw(Point p0, Point p1, Point *out) const;

//protected:
	/// fills the playfield with new element data along a given line
	void DrawLineSegment(LineSegment const &line, Element what);


	/// collision handler for player moving from p0 to p1, with new_start over current
	void Collision(Point p0, Element new_start, Element current);

	/// test two flood fills; return 0 if they are the same area, else 1 if seed1 is greater area than seed0
	bool FloodFillCompare(Point seed0, Point seed1);

	/// perform a flood fill
	int FloodFill(Point seed, Element type) { return FloodFill(seed.x, seed.y, type); }
	int FloodFill(int x, int y, Element type);

	/// set elements in the playfield
	void Set(Point P, Element E) { Set(P.x, P.y, E); }
	void Set(int x, int y, Element E) { if (OutOfBounds(Point(x,y))) return; elements.at(x + y*width) = E; }

	/// calculate a new area to fill after the player completes a loop on the playfield
	/// \param dir the direction the player was travelling
	/// \param P the point that completed the loop
	/// \return the number of points filled
	int CalcNewArea(Direction dir, Point P);

};

#endif // PLAYFIELD_H_INCLUDED

//EOF
