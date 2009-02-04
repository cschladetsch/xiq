// (C) 2009 www.christian.schladetsch.net

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

	player = New<Player>();

	initialised = false;

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf( "Unable to init SDL: %s\n", SDL_GetError() );
        return;
    }

    // make sure SDL cleans up before exit
    atexit(SDL_Quit);

    screen = SDL_SetVideoMode(width, height, 16, SDL_HWSURFACE|SDL_DOUBLEBUF);
    if (!screen)
    {
        printf("Unable to create window: %s\n", SDL_GetError());
        return;
    }

	SDL_WM_SetCaption("XIQ != QIX", "XIQ");

	// initialise timing
	game_time.start_game = (float)SDL_GetTicks()/1000.0f;	// SDL_GetTicks() return milliseconds
	game_time.start_frame = game_time.start_game;
	game_time.total = 0;
	game_time.delta = 0;
}

World::~World()
{
	Clear();
	SDL_Quit();
}

bool World::Update(GameTime)
{
	UpdateTime();
	UpdateObjects();
	CollisionDetection();
	game_time.total += game_time.delta;
	return true;
}

void World::UpdateTime()
{
	float seconds = (float)SDL_GetTicks()/1000.f;
	const float ideal = 1.0f/60.0f;
	game_time.delta = 0;

	do
	{
		game_time.delta += seconds - game_time.start_frame;
		seconds = (float)SDL_GetTicks()/1000.f;
	}
	while (game_time.delta < ideal);

	game_time.delta = ideal;
	game_time.start_frame = seconds;
}

Box World::GetBounds() const
{
	int width = screen->w;
	int height = screen->h;
	Box box;
	box.top_left = Point(0,0);
	box.top_right = Point(width - 1, 0);
	box.bottom_left = Point(0, height - 1);
	box.bottom_right = Point(width - 1, height - 1);
	return box;
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
	if (player->IsImmune())
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
	if (game_time.total - time_last_impact < min_impact_time)
		return;
	time_last_impact = game_time.total;
	Impact *impact = New<Impact>();
	impact->SetLocation(Point(x,y));
	impact->SetRadius(radius);
	impact->time_to_die = game_time.total + 0.35f;

	objects.insert(impact);
}

void World::Draw(Matrix const &M)
{
	playfield->Draw(M);
	DrawObjects(M);
}

void World::DrawObjects(Matrix const &M)
{
	Objects::const_iterator obj = objects.begin(), end = objects.end();
	for (; obj != end; ++obj)
	{
		(*obj)->Draw(M);
	}
}

//EOF
