// (C) 2009 christian.schladetsch@gmail.com

#include "Common.h"
#include "Object.h"
#include "Player.h"
#include "Playfield.h"
#include "World.h"
#include "Game.h"

#include "Draw.h"

Player::Player()
{
	speed = 180;
	radius = 20;
	drawing = false;
	std::fill(wants_direction, wants_direction + 5, false);
	num_lives = 3;
	immunity_ends = 0;
	score = 0;
}

void Player::LoseLife()
{
	if (num_lives > 0)
		--num_lives;
	Time now = GetRoot()->TimeNow();
	respawn_ends = now + 1.5;
	immunity_ends = now + 3.5;

	SetDirection(Direction::None);
	GetRoot()->GetWorld()->GetPlayfield()->RemoveNewLines(Playfield::Empty);
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

void Player::SetWantsDirection(Direction D, bool F)
{
	if (D == direction)
		SetDirection(Direction::None);

	wants_direction[D.value] = F;
	if (F == false)
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

bool Player::Update(GameTime time)
{
	if (!Moving())
		return true;

	// plot the trajectory against the playfield-> this will fire events,
	// and possibly clip the movement
	GetRoot()->GetWorld()->GetPlayfield()->MovePlayer(*this, time, IsDrawing() ? Playfield::NewLine : Playfield::Line);
	return true;
}

void Player::AddScore(int s)
{
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
	float radius = GetRoot()->GetWorld()->GetPlayfield()->GetWidth()*remaining*remaining;
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
