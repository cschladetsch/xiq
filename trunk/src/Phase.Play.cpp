// (C) 2009 christian.schladetsch@gmail.com

#include "PhaseCommon.h"

namespace Phase
{
	void Play::Prepare()
	{
		world = game->GetWorld();
		player = game->GetPlayer();
		playfield = world->GetPlayfield();
		level = 0;
	}

	bool Play::Update(GameTime)
	{
		return true;
	}

	void Play::Draw(Matrix const &)
	{
	}

	bool Play::InputEvent(SDL_Event const &event)
	{
		switch (event.type)
		{
		case SDL_KEYUP:
			{
				switch (event.key.keysym.sym)
				{
					case SDLK_a:
					case SDLK_LEFT:
						PlayerUnDirects(Direction::Left);
						break;
					case SDLK_d:
					case SDLK_RIGHT:
						PlayerUnDirects(Direction::Right);
						break;
					case SDLK_w:
					case SDLK_UP:
						PlayerUnDirects(Direction::Up);
						break;
					case SDLK_s:
					case SDLK_DOWN:
						PlayerUnDirects(Direction::Down);
						break;
					case SDLK_SPACE:
//						space_down = false;
						break;
					default:
						break;
				}
			}
			break;
		case SDL_KEYDOWN:
			{
				switch (event.key.keysym.sym)
				{
				case SDLK_a:
				case SDLK_LEFT:
					PlayerDirects(Direction::Left);
					break;
				case SDLK_d:
				case SDLK_RIGHT:
					PlayerDirects(Direction::Right);
					break;
				case SDLK_w:
				case SDLK_UP:
					PlayerDirects(Direction::Up);
					break;
				case SDLK_s:
				case SDLK_DOWN:
					PlayerDirects(Direction::Down);
					break;
//				case SDLK_SPACE:
//					space_down = true;
//					break;
				case SDLK_q:
//					world->RestartLevel();
					break;
				case SDLK_n:
//					world->NextLevel();
					break;
				default:
					break;
				}
			}
		} // end switch
		return true;
	}

	void Play::PlayerDirects(Direction dir)
	{
		player->SetWantsDirection(dir, true);

		if (dir == player->GetDirection().Opposite())
		{
			player->SetDirection(Direction::None);
			return;
		}
		Vector V = dir.GetVector();				// direction of movement
		Point N = player->GetLocation() + V;	// intended next location

		if (playfield->OutOfBounds(N))
			return;

		bool start_new = true;//TODO space_down;			// start a new shape?
		Playfield::Element element = playfield->At(N);	// what is at intended pos

		if (element == Playfield::Filled)
			return;

		if (player->IsDrawing())
		{
			if (element == Playfield::Line || element == Playfield::Empty)
			{
				player->SetDirection(dir);
			}
			return;
		}

		// player is not drawing:
		if (element == Playfield::Line)
			player->SetDirection(dir);

		if (element == Playfield::Empty && start_new)
		{
			player->SetDrawing();
			player->SetDirection(dir);
		}
	}

	void Play::PlayerUnDirects(Direction dir)
	{
		player->SetWantsDirection(dir, false);
	}
}

//EOF
