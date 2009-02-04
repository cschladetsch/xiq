// (C) 2009 christian.schladetsch@gmail.com

#include "Common.h"
#include "Draw.h"
#include "Object.h"
#include "Xiq.h"
#include "Player.h"
#include "Playfield.h"
#include "World.h"
#include "Game.h"

Xiq::Xiq()
{
	collides = true;
	angle = RandomUnit();
	steer = RandomUnitVector();

	SetRadius(30);
	speed = 50;
	max_speed = 50;

	force = RandomUnitVector()*1000;

	last_pos = location = Vector(300,200);

	radius_waves[0].amplitude = 60;
	radius_waves[0].frequency = 1.3f;
	radius_waves[0].offset = 20;
	radius_waves[1].amplitude = 30;
	radius_waves[1].frequency = 0.7f;
	radius_waves[1].phase = math::pi/2.0f;

	colors[0].amplitude = 55;
	colors[0].offset = 200;
	colors[0].frequency = 0.5f;

	colors[1].amplitude = 55;
	colors[1].offset = 200;
	colors[1].frequency = 1.0f;

	colors[2].amplitude = 55;
	colors[2].offset = 200;
	colors[2].frequency = 1.5f;

	hit_player = false;
}

void Xiq::SetRadius(float R)
{
	float density = 0.0001f;
	if (R < 10)
		R = 10;
	radius = R;
	mass = math::pi*radius*radius*density;
}

bool Xiq::Update(GameTime time)
{
	float t = time.TotalElapsedSeconds();
	SetRadius(radius_waves[0](t) + radius_waves[1](t));

	angle += RandomRanged(-0.3f, 0.3f);
	while (angle > math::two_pi)
		angle -= math::two_pi;
	while (angle < 0)
		angle += math::two_pi;

	Vector center = location + steer*sqrt(2);
	Point target = center + Vector(cos(angle), sin(angle));
	steer = (target - location).Normalised();
	force += steer*time.DeltaSeconds()*5000;

	// see http://en.wikipedia.org/wiki/Verlet_integration:
	// np = p0*2 - p1 + a*t*t;
	// p1 = p0
	// p0 = np
	//
	// i have fudged this here to enforce constant speed:

	float dt = 1.0f/60.0f;
	Vector accel = force*(1.0f/mass);
	Point next_location = location*2.0f - last_pos + accel*dt*dt;

	Vector vel = (next_location - location).Normalised()*speed*time.DeltaMillis();

	last_pos = location;
	location += vel;
	force = Vector(0,0);

	return true;
}

void Xiq::SetPixel(int x, int y, Color color)
{
	SDL_Surface *surface = GetRoot()->GetSurface();
	World *world = GetRoot()->GetWorld();
	Playfield *playfield = world->GetPlayfield();
	Player *player = world->GetPlayer();

	if (x < 0 || y < 0 || x >= surface->w  || y >= surface->h)
	{
		// if we clip against the edge, apply a restorative force
		force += Vector(world->GetMidPoint() - Vector(x,y));
		world->AddImpact(x,y, GetRadius()*6);
		return;
	}

	Playfield::Element element = playfield->At(x, y);
	if (element == Playfield::NewLine)
	{
		if (!player->IsImmune())
		{
			world->AddImpact(x,y, GetRadius()*6);
//			player->OnHit(...);
		}
	}
	if (element != Playfield::Empty)
	{
		// if we hit something, apply a force to move towards the player
		force += (location - Vector(x,y))*5;
		world->AddImpact(x,y, GetRadius()*6);
	}

	char *pixels = (char *)surface->pixels;
	Color *scanline = (Color *)(pixels + surface->pitch*y);
	scanline[x] = color;
}

void Xiq::Draw(Matrix const &/*matrix*/)
{
	float x0 = location.x;
	float y0 = location.y;

	Time t = GetRoot()->TimeNow();
	float r = colors[0](t);
	float g = colors[1](t);
	float b = colors[2](t);
	Color color = GetRoot()->MakeColor(r,g,b);

	int ra = radius;
	if (ra < 0)
		ra *= -1;

	int f = 1 - ra;
	int ddF_x = 1;
	int ddF_y = -2 * ra;
	int x = 0;
	int y = ra;

	SetPixel(x0, y0 + ra, color);
	SetPixel(x0, y0 - ra, color);
	SetPixel(x0 + ra, y0, color);
	SetPixel(x0 - ra, y0, color);

	while (x < y)
	{
		if (f >= 0)
		{
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;
		SetPixel(x0 + x, y0 + y, color);
		SetPixel(x0 - x, y0 + y, color);
		SetPixel(x0 + x, y0 - y, color);
		SetPixel(x0 - x, y0 - y, color);
		SetPixel(x0 + y, y0 + x, color);
		SetPixel(x0 - y, y0 + x, color);
		SetPixel(x0 + y, y0 - x, color);
		SetPixel(x0 - y, y0 - x, color);
	}
}

//EOF
