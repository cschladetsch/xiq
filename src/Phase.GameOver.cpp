// (C) 2009 christian.schladetsch@gmail.com

#include "Precompiled.h"
#include "PhaseCommon.h"
#include "Font.h"

namespace Phase
{
	void GameOver::Prepare()
	{
	}

	void GameOver::Enter(Base *play_phase)
	{
		prev = play_phase;
		score = GetRoot()->GetWorld()->GetPlayer()->GetScore();
	}

	void GameOver::Leave(Base * /*next*/)
	{
		prev->Delete();	// delete the play phase
		this->Delete();		// delete this phase
	}

	bool GameOver::InputEvent(SDL_Event const &event)
	{
		bool space = event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE;
		if (space)
		{
			GetRoot()->PhaseChange(New<Phase::Boot>());
		}
		return space;
	}

	bool GameOver::Update(GameTime time)
	{
		GetRoot()->GetWorld()->Update(time);
		return true;
	}

	void GameOver::Draw(Matrix const &M)
	{
		prev->Draw(M);

		Font *font = GetRoot()->GetFont();
		char text[1000];
		sprintf_s(text, 1000, "score %06d", score);
		size_t len = strlen(text);
		Matrix matrix = Matrix::Translate(-(int)len*4,-4)*Matrix::Scale(3,3)*Matrix::Translate(300,200);
		Color color = GetRoot()->MakeColor(255,255,0);
		font->DrawShadowedText(GetRoot()->GetSurface(), matrix, Box(), color, text);
	}
}

//EOF

