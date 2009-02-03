// (C) 2009 www.christian.schladetsch.net

#include "Common.h"
#include "Object.h"
#include "Draw.h"
#include "Impact.h"
#include "Game.h"

bool Impact::Update(GameTime time)
{
	return time.total > time_to_die;
}

void Impact::Draw(Matrix const &)
{
	float a = time_to_die - GetRoot()->GetTime().total;
	if (a > 1)
		a = 1;
	a *= a;
	Color color = GetRoot()->MakeColor(255*a,255*a,255*a);
	DrawCircle(GetRoot()->GetSurface(), location.x, location.y, radius*a, color, BlendMode::Add);
}

//EOF
