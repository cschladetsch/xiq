// (C) 2009 christian.schladetsch@gmail.com

#include "Common.h"
#include "Object.h"
#include "Player.h"
#include "Playfield.h"
#include "World.h"
#include "Game.h"

#include "Xiq.h"
#include "Impact.h"
#include "Styx.h"

#include "Draw.h"

// create the world, given viewport extents
void World::Construct(int width, int height)
{
	playfield = New<Playfield>();
	playfield->Create(width, height);
}

World::~World()
{
	::Delete(playfield);
	Clear();
}

bool World::Update(GameTime)
{
	UpdateObjects();
	CollisionDetection();
	return true;
}

Box World::GetBounds() const
{
	int width = playfield->GetWidth();
	int height = playfield->GetHeight();
	Box box;
	box.top_left = Point(0,0);
	box.top_right = Point(width - 1, 0);
	box.bottom_left = Point(0, height - 1);
	box.bottom_right = Point(width - 1, height - 1);
	return box;
}

SDL_Surface *World::GetSurface() const
{
	return GetRoot()->GetSurface();
}

Point World::GetMidPoint() const
{
	return Point(playfield->GetWidth()/2, playfield->GetHeight()/2);
}

void World::Clear()
{
	playfield->Clear();
	DeleteObjects();
}

void World::DeleteObjects()
{
	Objects::const_iterator obj = objects.begin(), end = objects.end();
	for (; obj != end; ++obj)
	{
		(*obj)->Delete();
	}
	objects.clear();
}

void World::CollisionDetection()
{
	Player *player = GetRoot()->GetPlayer();
	if (player && player->IsImmune())
	{
		return;
	}

// TODO: events
//	// test if xiq hit a line
//	if (xiq->HasHitPlayer())
//	{
//		xiq->SetHitPlayer(false);
//		player->LoseLife();
//		if (player->HasNoLives())
//		{
//			EndGame();
//		}
//		return;
//	}

	// test all other objects
	Objects::const_iterator obj = objects.begin(), end = objects.end();
	for (; obj != end; ++obj)
	{
		Object *object = *obj;

		if (!object->Collides())
			continue;

		float distance = (object->GetLocation() - player->GetLocation()).Length();
		if (distance < object->GetRadius() + player->GetRadius())
		{
// TODO: Events
//			player->LoseLife();
//			if (player->HasNoLives())
//			{
//				EndGame();
//			}
			return;
		}
	}
}

void World::UpdateObjects()
{
	typedef std::vector<Object *> Deathrow;
	Deathrow deathrow;

	// iterate over objects, updating each
	// add objects that fail to update  to deathrow for later deletion
	Objects::const_iterator obj = objects.begin(), end = objects.end();
	for (; obj != end; ++obj)
	{
		Object *object = *obj;
		bool lives = object->Update(game_time);
		if (!lives)
		{
			deathrow.push_back(object);
		}
	}

	// for each object in deathrow, remove from objects container and delete
	Deathrow::const_iterator dr_start = deathrow.begin(), dr_end = deathrow.end();
	for (; dr_start != dr_end; ++dr_start)
	{
		Object *object = *dr_start;
		objects.erase(objects.find(object));
		object->Delete();
	}
}

void World::AddImpact(int x, int y, float radius)
{
	Time now = GetRoot()->TimeNow();
	if (now - time_last_impact < min_impact_time)
		return;
	time_last_impact = now;
	Impact *impact = New<Impact>();
	impact->SetLocation(Point(x,y));
	impact->SetRadius(radius);
	impact->time_to_die = now + 0.35f;

	objects.insert(impact);
}

void World::Draw(Matrix const &M)
{
	playfield->Draw(M);
	DrawObjects(M);
}

void World::DrawObjects(Matrix const &M)
{
	foreach (Object *obj, objects)
	{
		obj->Draw(M);
	}
}

//EOF
