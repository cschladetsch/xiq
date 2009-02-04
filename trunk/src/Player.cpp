// (C) 2009 christian.schladetsch@gmail.com

#include "Common.h"
#include "Object.h"
#include "Player.h"
#include "Playfield.h"
#include "World.h"
#include "Game.h"

#include "Draw.h"

void Player::Prepare()
{
	speed = 180;
	radius = 20;
	drawing = false;
	std::fill(wants_direction, wants_direction + 5, false);
	num_lives = 3;
	Time now = GetRoot()->TimeNow();
	immunity_ends = now + 3;
	respawn_ends = 0;//now + 2;
	score = 0;
	location = Point(300,399);
}

void Player::LoseLife()
{
	if (num_lives > 0)
	{
		--num_lives;
		//TODO OnLifeLost();
	}
	Time now = GetRoot()->TimeNow();
	respawn_ends = now + 1.5;
	immunity_ends = now + 3.5;

	SetDirection(Direction::None);
	if (IsDrawing())
	{
		drawing = false;
		location = launch_point;
	}
}

void Player::Move(LineSegment const &traversed)
{
	location = traversed.second;
}

void Player::SetDirection(Direction dir)
{
	if (dir == direction)
		return;
	direction = dir;
}

void Player::SetWantsDirection(Direction dir, bool wants)
{
	if (dir == direction)
		SetDirection(Direction::None);

	wants_direction[dir.value] = wants;
	if (wants == false)
	{
		for (int n = 0; n < 4; ++n)
		{
			if (wants_direction[n])
			{
				SetDirection(Direction::Type(n));
				return;
			}
		}
	}
}

void Player::SetLocation(Point P)
{
	//printf("Player::SetLocation: %f %f\n", P.x, P.y);
	location = P;
}

bool Player::Update(GameTime time)
{
	if (!Moving())
		return true;

	Playfield *playfield = GetPlayfield();

	// determine the path of movement
	Direction dir = GetDirection();
	Vector v = dir.GetVector();
	float speed = GetSpeed();
	double distance = speed*time.DeltaSeconds();
//	Point end_pos = location + v*distance;

	Playfield::Element what = drawing ? Playfield::NewLine : Playfield::Line;

	//printf("step=%g, distance=%g\n", time.DeltaSeconds(), distance);

	// while there is still some distance 	to move
	while (distance > 0)
	{
		// move at most one unit
		double step_dist = Clamp(distance, 0.0, 1.0);
		Point next = location + v*step_dist;
//		// if moving less than half of a pixel, move and return
//		if (step_dist < 0.5)
//		{
//			playfield->Set(next, what);
//			SetLocation(next);
//			return true;
//		}

		// get the element at the proposed next location
		Playfield::Element e_next = playfield->At(next);

		if (playfield->OutOfBounds(next))
			break;

		// can't move into filled locations
		if (e_next == Playfield::Filled)
			break;

		// can't go through an existing new line
		if (drawing && e_next == Playfield::NewLine)
			break;

		// can only move on lines when not creating new one
		if (!drawing && e_next != Playfield::Line)
			break;

		// complete an area if we are drawing and hit an existing line
		if (drawing && e_next == Playfield::Line)
		{
			int num_filled = playfield->CalcNewArea(dir, next);
			double score = (double )num_filled*num_filled/10000000.;
			SetLocation(next);
			AddScore((int)score);
			SetDrawing(false);
			return true;
		}

		// test for movement into a perpendicular line
		if (!drawing)
		{
			for (int n = Direction::Left; n < Direction::Last; ++n)
			{
				Direction dir = Direction::Type(n);
				if (!WantsDirection(dir))
					continue;
				Point P = next + dir.GetVector();
				if (playfield->OutOfBounds(P))
					continue;
				if (playfield->At(P) == Playfield::Line)
				{
					// change movement.
					// we should really continue along the new
					// path, because as it stands the player will
					// move a shorter distance when cornering.
					// but i doubt it will matter.
					SetDirection(dir);
					SetLocation(location + v);
					return true;
				}
			}
		}

		// write to the playfield
		playfield->Set(next, what);

		// remove the covered distance
		distance -= step_dist;

		// update position
		SetLocation(next);
	}
	return true;
}

void Player::AddScore(int s)
{
	//OnScoreAdded(this, s);
	score += s;
}

bool Player::IsImmune() const
{
	return GetRoot()->TimeNow() < immunity_ends;
}

bool Player::IsRespawning() const
{
	return GetRoot()->TimeNow() < respawn_ends;
}

bool Player::Moving() const
{
	return direction != Direction::None && !IsRespawning();
}

void Player::Draw(Matrix const &)
{
	if (IsRespawning())
	{
		DrawRespawn();
	}

	Point P = GetLocation();
	float L = GetRadius()/2.0f;

	Matrix matrix = Matrix::ScaleRotateTranslate(L,L,0,P);
	Matrix shadow = Matrix::ScaleRotateTranslate(L,L,0,P + Vector(2,2));

	Color color = IsImmune() ? GetRoot()->MakeColor(255,255,0) : GetRoot()->MakeColor(0,255,255);

	Draw(shadow, 0);
	Draw(matrix, color);
}

void Player::Draw(Matrix const &M, Color color)
{
	Point points[] =
	{
		Point(-1, 0),
		Point(0, -1),
		Point(1, 0),
		Point(0, 1),
	};
	for (int n = 0; n < 4; ++n)
	{
		points[n] = M*points[n];
	}
	SDL_Surface *surface = GetRoot()->GetSurface();
	for (int n = 0; n < 4; ++n)
	{
		DrawLineSegment(surface, LineSegment(points[n], points[(n + 1)%4]), color);
	}
}

void Player::DrawRespawn()
{
	float remaining = respawn_ends - GetRoot()->TimeNow();
	float radius = 600*remaining*remaining;
	float C = 255;
	Color color1 = GetRoot()->MakeColor(C,0,0);
	Color color2 = GetRoot()->MakeColor(0,C,0);
	Color color3 = GetRoot()->MakeColor(0,0,C);
	SDL_Surface *surface = GetRoot()->GetSurface();
	for (int n = 0; n < 10; ++n, radius *= 0.80f)
	{
		DrawCircle(surface, location.x - 1, location.y - 1, radius + 1, color1);
		DrawCircle(surface, location.x + 0, location.y + 0, radius + 0, color2);
		DrawCircle(surface, location.x + 1, location.y - 1, radius - 1, color3);
	}
}

//EOF
