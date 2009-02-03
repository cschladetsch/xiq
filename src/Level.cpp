// (C) 2009 www.christian.schladetsch.net

#include "Common.h"
#include "Object.h"
#include "Player.h"
#include "Playfield.h"
#include "Level.h"
#include "Game.h"

#include "Xiq.h"

Level::Level()
{
}

void Level::Start()
{
	Playfield *playfield = 0;
	Player *player = 0;
	GameTime time;

	int width = playfield->GetWidth();
	int height = playfield->GetHeight();

	// start the player
	Point start_pos(width/2, height - 1);
	player->SetLocation(start_pos);
	player->SetImmune(time.total + 4);
	player->SetRespawn(time.total + 1);
	player->SetNumLives(3);

	// add a xiq
	Xiq *xiq = New<Xiq>();
	xiq->SetLocation(Point(300,200));
	//TODO objects.insert(xiq);

	time_last_impact = 0;
	min_impact_time = .050f;

	//TODO AddStyx();
}

bool Level::Update(GameTime)
{
	return true;
}

void Level::Draw(Matrix const &)
{
}

//EOF
