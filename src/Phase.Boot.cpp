// (C) 2009 christian.schladetsch@gmail.com

#include "PhaseCommon.h"
#include "Font.h"

namespace Phase
{
	void Boot::Prepare()
	{
	}

	bool Boot::InputEvent(SDL_Event const &event)
	{
		bool space = event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE;
		if (space)
		{
			GetRoot()->PhaseChange(New<Phase::Play>());
		}
		return !space;
	}

	bool Boot::Update(GameTime)
	{
		return true;
	}

	void Boot::Draw(Matrix const &)
	{
		Font *font = GetRoot()->GetFont();
		SDL_Surface *surface = GetRoot()->GetSurface();
		Time now = GetRoot()->TimeNow();

		// draw the 'xiq' text rotating underneath
		std::string xiq = "xiq";
		int len = xiq.size();
		float scale = 8;
		float angle = now;
		Matrix M =
			Matrix::Translate(-len*4,-4)
			*Matrix::Scale(scale,scale)
			*Matrix::Rotation(angle)
			*Matrix::Translate(300,200);

		font->DrawShadowedText(
			surface
			, M
			, Box(Point(50,50), Point(400,200))
			, GetRoot()->MakeColor(255,0,0)
			, xiq.c_str());
		font->DrawShadowedText(
			surface
			, M*Matrix::Translate(1,1)
			, Box(Point(50,50), Point(400,200))
			, GetRoot()->MakeColor(0,255,0)
			, xiq.c_str());
		font->DrawShadowedText(
			surface
			, M*Matrix::Translate(2,2)
			, Box(Point(50,50), Point(400,200))
			, GetRoot()->MakeColor(0,0,255)
			, xiq.c_str());

		// draw 'press space' zooming in and out
		{
			std::string text = "press-space";
			int len = text.size();

			float scale = 3 + 3*sin(now*2);

			Matrix M =
				Matrix::Translate(-len*4 - 2,-4)
				*Matrix::Scale(scale,scale)
				*Matrix::Translate(300,200);

			font->DrawShadowedText(
				surface
				, M
				, Box(Point(50,50), Point(400,200))
				, GetRoot()->MakeColor(255,255,255)
				, text.c_str());
		}
	}
}

//EOF
