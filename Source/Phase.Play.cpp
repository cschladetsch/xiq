// (C) 2009 christian.schladetsch@gmail.com

#include "Precompiled.h"
#include "PhaseCommon.h"
#include "Font.h"
#include "Xiq.h"
#include "Styx.h"

namespace Phase
{
	Play::Play()
	{
	}

	Play::~Play()
	{
		::Delete(world);
	}

	void Play::Prepare()
	{
		level = 0;
		world = New<World>();
		world->Construct(600, 400);

		GetRoot()->SetWorld(world);

		Xiq *xiq = New<Xiq>();
		world->AddObject(xiq);

		Styx *styx = New<Styx>();
		Point spawn_point(0,0);
		styx->SetLocation(spawn_point);
		styx->SetRadius(15);
		styx->SetDirection(Direction::Down);
		styx->SetSpeed(40);
		styx->SetColor(GetRoot()->MakeColor(255,0,0));
		styx->move_toward_player = true;
		world->AddObject(styx);

		styx = New<Styx>();
		spawn_point = Point(599,0);
		styx->SetLocation(spawn_point);
		styx->SetRadius(8);
		styx->SetDirection(Direction::Down);
		styx->SetSpeed(40);
		styx->SetColor(GetRoot()->MakeColor(0,0,255));
		styx->move_toward_player = false;
		world->AddObject(styx);

//		player->OnLifeLost += Delegate(this, &Play::PlayerLostLife);
	}

//	void Play::PlayerLostLife()
//	{
//		world->GetPlayfield()->RemoveNewLines(Playfield::Empty);
//	}

	bool Play::Update(GameTime time)
	{
		world->Update(time);

		if (world->GetPlayer()->GetNumLives() == 0)
		{
			GetRoot()->PhaseChange(New<GameOver>());
			return true;
		}

		// test for completion
		float filled = world->GetPlayfield()->GetPercentFilled();
		if (filled > 0.75f)
		{
			float over = filled - 0.75f;
			world->GetPlayer()->AddScore(over * 100 * 1000);

			Play *next_level = New<Phase::Play>();
			next_level->SetLevel(level + 1);
			GetRoot()->PhaseChange(next_level);//, 3);
		}
		return true;
	}

	void Play::SetLevel(int l)
	{
		level = l;
		int num_styx = Clamp<int>(level*3/2, 0, 6);

	}

	void Play::Draw(Matrix const &matrix)
	{
		world->Draw(matrix);

		// draw the score
		Matrix transform;
		Color color;

		char text[1000];

		sprintf_s(text, 1000, "score %06d", world->GetPlayer()->GetScore());
		size_t len = strlen(text);
		color = GetRoot()->MakeColor(255,255,255);
		transform = Matrix::Translate(len/2, -4)*Matrix::Scale(1.5,1.5)*Matrix::Translate(12,15);
		GetRoot()->GetFont()->DrawShadowedText(GetRoot()->GetSurface(), transform, Box(), color, text);


		sprintf_s(text, 1000, "lives %d", world->GetPlayer()->GetNumLives());
		len = strlen(text);
		color = GetRoot()->MakeColor(255,255,255);
		transform = Matrix::Translate(len/2, -4)*Matrix::Scale(1.5,1.5)*Matrix::Translate(12,30);
		GetRoot()->GetFont()->DrawShadowedText(GetRoot()->GetSurface(), transform, Box(), color, text);
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
					{
						GetRoot()->PhaseChange(New<Phase::Play>());
					}
					break;
				case SDLK_n:
//					world->NextLevel();
					break;
				case SDLK_ESCAPE:
					{
						GetRoot()->PhaseChange(New<Phase::Boot>());
					}
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
		Player *player = world->GetPlayer();
		player->SetWantsDirection(dir, true);

		if (dir == player->GetDirection().Opposite())
		{
			player->SetDirection(Direction::None);
			return;
		}
		Vector V = dir.GetVector();				// direction of movement
		Point N = player->GetLocation() + V;	// intended next location

		Playfield *playfield = world->GetPlayfield();
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
		world->GetPlayer()->SetWantsDirection(dir, false);
	}
}

//EOF
