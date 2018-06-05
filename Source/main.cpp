// (C) 2009 christian.schladetsch@gmail.com

#include "Precompiled.h"
#include "PhaseCommon.h"

#include "Xiq.h"
#include "Styx.h"
#include "Impact.h"

/// @mainpage XIQ
/// XIQ is a fast and sweet clone of the class arcade game, <a href="http://en.wikipedia.org/wiki/Qix">QIX</a>
///
/// I decided to go retro in the code as well, and write the entire thing with
/// just a frame-buffer. that means:
/// <ul>
///  <li> all custom graphics primitives</li>
///  <li> anti-aliased bresenham line-draws </li>
///  <li> flood-fills </li>
/// </ul>
///
/// Complete source, and a windows binary, are <a href="http://schladetsch.net/xiq/archive">available</a>,
/// and here are some <a href="http://schladetsch.net/xiq/screenshots">screenshots</a> at various stages of development.
///
/// And <a href="http://nz.youtube.com/watch?v=nGCsK3JLN9A">here's</a> a youtube video of the first playable build.
///
/// See my <a href="http://schladetsch.net/wordpress">blog</a> for details.
///
/// A port of the orignal game can be <a href="http://www.eskimo.com/games/qix/index.php?act=Arcade&do=newscore">played online</a>,
/// and here's a screenshot of the original:
///
/// <img src="http://upload.wikimedia.org/wikipedia/en/8/8d/Qixingame.png" />
///

void RegisterTypes(Factory &factory)
{
    factory.AddClass<Playfield>();
    factory.AddClass<Player>();
    factory.AddClass<Xiq>();
    factory.AddClass<Styx>();
    factory.AddClass<Level>();
    factory.AddClass<World>();
    factory.AddClass<Impact>();
    factory.AddClass<Game>();

    factory.AddClass<Phase::Boot>();
    factory.AddClass<Phase::Attract>();
    factory.AddClass<Phase::Play>();
    factory.AddClass<Phase::GameOver>();
}

int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    Factory factory;
    RegisterTypes(factory);

    Game *game = factory.New<Game>();
    game->game = game;
    game->Create(600, 400);
    if (!game->Initialised())
        return 1;

    GameTime time;
    Matrix transform;

    for (;;)
    {
        time.StartFrame();
        if (!game->Update(time))
            break;
        game->Draw(transform);
    }
    return 0;
}


