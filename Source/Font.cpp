// (C) 2009 christian.schladetsch.net

#include "Precompiled.h"
#include "Font.h"
#include "Draw.h"

#include <fstream>
#include <sstream>
#include <iterator>

using namespace std;

void Font::Glyph::Draw(SDL_Surface *surface, Matrix const &matrix, Color color) const
{
    for (const auto& line : lines)
    {
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
    size_t num_glyphs = 255;

    for (size_t glyph_num = 0; glyph_num < num_glyphs; ++glyph_num)
    {
        std::vector<std::string> glyh_desc(glyph_num_lines);
        for (size_t n = 0; n < glyph_num_lines; ++n)
        {
            char buffer[2000];
            if (!file.getline(buffer, sizeof(buffer)))
            {
                goto end;
            }
            glyh_desc[n] = buffer;
        }

        Glyph glyph;
        std::vector<Point2> points;
        glyph.letter = glyh_desc[0][0];
        for (size_t y = 0; y < y_dim; ++y)
        {
            std::string const &line = glyh_desc[1 + y];
            for (size_t x = 0; x < line.size(); ++x)
            {
                if (line[x] == '.')
                    continue;
                points.push_back(Point2(x, y));
            }
        }

        stringstream str(glyh_desc[9]);
        vector<int> indices;
        copy(istream_iterator<int>(str), istream_iterator<int>(), back_inserter(indices));
        if ((indices.size() % 2) != 0)
        {
            std::cerr << "error in font file " << filename << ": num indices must be even" << endl;
            goto end;
        }

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

void Font::DrawShadowedText(SDL_Surface *surface, Matrix const &transform, Box const &box, Color color, const char *text) const
{
    Matrix shadow = transform*Matrix::Translate(3,3);
    DrawText(surface, shadow, box, SDL_MapRGB(surface->format, 0,0,0), text);
    DrawText(surface, transform, box, color, text);
}

void Font::DrawText(SDL_Surface *surface, Matrix const &transform, Box const &box, Color color, const char *text) const
{
    (void)box;

    Matrix matrix = transform;

    for (; text && *text; ++text)
    {
        char letter = *text;
        Glyphs::const_iterator iter = glyphs.find(letter);
        if (iter == glyphs.end())
            continue;

        iter->second.Draw(surface, matrix, color);

        matrix = Matrix::Translate(9,0)*matrix;
    }
}
