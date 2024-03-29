// (C) 2009 christian.schladetsch@gmail.com

#include "Precompiled.h"
#include "Object.h"
#include "Player.h"
#include "Playfield.h"
#include "World.h"
#include "Game.h"
#include "Xiq.h"
#include "Impact.h"
#include "Styx.h"
#include "Draw.h"

Playfield *Object::GetPlayfield() const
{
    return GetRoot()->GetWorld()->GetPlayfield();
}

void World::Construct(int width, int height)
{
    GetRoot()->SetWorld(this);
    playfield = New<Playfield>();
    playfield->Create(width, height);
    player = New<Player>();

    objects.insert(player);

    time_last_impact = GetRoot()->TimeNow();
    min_impact_time = 1.0f/20.0f;
}

World::~World()
{
    ::Delete(playfield);
    Clear();
}

bool World::Update(GameTime time)
{
    UpdateObjects(time);
    CollisionDetection();
    return true;
}

void World::Clear()
{
    playfield->Clear();
    DeleteObjects();
}

void World::DeleteObjects()
{
    for (auto object : objects)
    {
        object->Delete();
    }
    objects.clear();
}

void Trace(Object *object)
{
    if (!Exists(object))
    {
        printf("Null Object\n");
        return;
    }
    printf("Object type=%d; radius=%f; pos=%f,%f\n", object->GetTypeNumber(), object->GetRadius(), object->GetLocation().x, object->GetLocation().y);
}

void World::CollisionDetection()
{
    for (auto A : objects)
    {
        if (!A->Collides())
            continue;

        for (auto B :  objects)
        {
            if (!B->Collides())
                continue;
            if (A == B)
                continue;

            float distance = (A->GetLocation() - B->GetLocation()).Length();
            bool collision = distance < A->GetRadius() + B->GetRadius();
            if (collision)
            {
                if (!player->IsImmune() && (A->IsType<Player>() || B->IsType<Player>()))
                {
                    player->LoseLife();
                }
            }
        }
    }
}

void World::UpdateObjects(GameTime time)
{
    std::vector<Object *> tmp(objects.begin(), objects.end());
    for (auto object : tmp)
    {
        if (!object->Update(time))
        {
            objects.erase(objects.find(object));
            object->Delete();
        }
    }
}

void World::AddImpact(int x, int y, float radius)
{
    Time now = GetRoot()->TimeNow();
    if (now - time_last_impact < min_impact_time)
        return;
    time_last_impact = now;
    Impact *impact = New<Impact>();
    impact->SetLocation(Point2(x,y));
    impact->SetRadius(radius);
    impact->time_to_die = now + 0.35f;

    objects.insert(impact);
}

Box World::GetBounds() const
{
    int width = playfield->GetWidth();
    int height = playfield->GetHeight();
    Box box;
    box.top_left = Point2(0,0);
    box.top_right = Point2(width - 1, 0);
    box.bottom_left = Point2(0, height - 1);
    box.bottom_right = Point2(width - 1, height - 1);
    return box;
}

Point2 World::GetMidPoint() const
{
    return Point2(playfield->GetWidth()/2, playfield->GetHeight()/2);
}

void World::Draw(Matrix const &M)
{
    playfield->Draw(M);
    DrawObjects(M);
}

void World::DrawObjects(Matrix const &M)
{
    for (auto obj : objects)
    {
        obj->Draw(M);
    }
}
