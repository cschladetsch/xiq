// (C) 2009 christian.schladetsch.net

#ifndef FONT_H_INCLUDED
#define FONT_H_INCLUDED

/// a set of glyphs that can be used to draw text on a surface
struct Font
{
	/// an element in the font, mapped from a character
	struct Glyph
	{
		char letter;
		typedef std::vector<LineSegment> lines;

		void Draw(SDL_Surface *, Matrix const &, Color);
	};
	typedef std::map<char, Glyph> Glyphs;

private:
	Glyphs glyphs;

public:
	/// read a font description from a file
	bool FromFile(const char *);

	/// draw some text
	void DrawText(SDL_Surface *, Matrix const &transform, Box const &box, Color color, const char *text) const;
};

#endif // FONT_H_INCLUDED
