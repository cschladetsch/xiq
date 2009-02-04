// (C) 2009 christian.schladetsch.net

#include "Common.h"
#include "Font.h"
#include "Draw.h"

#include <fstream>
#include <sstream>
#include <iterator>

using namespace std;

void Font::Glyph::Draw(SDL_Surface *surface, Matrix const &matrix, Color color) const
{
	foreach (LineSegment const &line, lines)
	{
//		DrawLineSegment(surface, Transform(line, matrix), color);
		DrawLineSegmentAntiAliased(surface, Transform(line, matrix), color);
	}
}

bool Font::FromFile(const char *filename)
{
	std::fstream file(filename);
	if (!file)
	{
		std::cerr << "couldn't find font file: " << filename << std::endl;
		return false;
	}

	// dimensions of the glyph representation in the file
	size_t y_dim = 8;
	size_t glyph_num_lines = y_dim + 2;
	size_t num_glyphs = 26;

	// read the glyphs
	for (size_t glyph_num = 0; glyph_num < num_glyphs; ++glyph_num)
	{
		// read the lines
		std::string glyh_desc[glyph_num_lines];
		for (size_t n = 0; n < glyph_num_lines; ++n)
		{
			char buffer[2000];
			if (!file.getline(buffer, sizeof(buffer)))
			{
				goto end;
			}
			glyh_desc[n] = buffer;
		}

		// construct glyph from string input
		Glyph glyph;
		std::vector<Point> points;
		glyph.letter = glyh_desc[0][0];
		cout << "glyph: " << glyph.letter << endl;
		for (size_t y = 0; y < y_dim; ++y)
		{
			std::string const &line = glyh_desc[1 + y];
			for (size_t x = 0; x < line.size(); ++x)
			{
				if (line[x] == '.')
					continue;
				points.push_back(Point(x, y));
			}
		}

		// read the point indices
		stringstream str(glyh_desc[9]);
		vector<int> indices;
		copy(istream_iterator<int>(str), istream_iterator<int>(), back_inserter(indices));
		if ((indices.size() % 2) != 0)
		{
			std::cerr << "error in font file " << filename << ": num indices must be even" << endl;
			goto end;
		}

		// create the line-segments for the glyph
		vector<int>::const_iterator index = indices.begin(), end = indices.end();
		while (index != end)
		{
			int n0 = *index++;
			int n1 = *index++;
			glyph.lines.push_back(LineSegment(points[n0], points[n1]));
		}

		// store result
		glyphs[glyph.letter] = glyph;
	}

end:
	file.close();
	return false;
}

void Font::DrawText(SDL_Surface *surface, Matrix const &transform, Box const &box, Color color, const char *text) const
{
	(void)box;

	Matrix matrix = transform;

	// for each character in the string to draw
	for (; text && *text; ++text)
	{
		// find the glyph that represents the next character in the string
		char letter = *text;
		Glyphs::const_iterator iter = glyphs.find(letter);
		if (iter == glyphs.end())
			continue;

		// draw it
		iter->second.Draw(surface, matrix, color);

		// move to next location.
		matrix = Matrix::Translate(9,0)*matrix;
	}
}

//EOF
