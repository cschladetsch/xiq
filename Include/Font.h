// (C) 2009 christian.schladetsch.net

#pragma once

/// a set of glyphs that can be used to draw text on a surface
struct Font
{
    /// an element in a font, mapped from a character
    struct Glyph
    {
        typedef std::vector<LineSegment> Lines;
        Lines lines;
        char letter;
        void Draw(SDL_Surface *, Matrix const &, Color) const;
    };
    typedef std::map<char, Glyph> Glyphs;

private:
    Glyphs glyphs;

public:
    Font()
    {
    }

    /// create a font given the filename to load from
    Font(const char *filename)
    {
        FromFile(filename);
    }

    /// read a font description from a file
    bool FromFile(const char *);

    /// draw some text
    void DrawText(SDL_Surface *, Matrix const &transform, Box const &box, Color color, const char *text) const;

    /// draw text with a shadow
    void DrawShadowedText(SDL_Surface *, Matrix const &transform, Box const &box, Color color, const char *text) const;
};

