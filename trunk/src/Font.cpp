// (C) 2009 christian.schladetsch.net

#include "Common.h"
#include "Font.h"

void Font::Glyph::Draw(SDL_Surface *, Matrix const &, Color)
{
}

bool Font::FromFile(const char *)
{
	return false;
}

/// draw some text
void Font::DrawText(SDL_Surface *, Matrix const &transform, Box const &box, Color color, const char *text) const
{
}

//EOF
