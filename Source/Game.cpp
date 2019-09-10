// (C) 2009 christian.schladetsch.net

#include "Precompiled.h"
#include "Game.h"
#include "Phase.h"
#include "Font.h"
#include "World.h"
#include "Player.h"

// may want this later; but for the moment just stick with raw audio
//#include "SDL/SDL_mixer.h"

void Game::Create(int width, int height)
{
    phase = 0;
    world = 0;
    screen = 0;
    next_phase = 0;
    font = 0;
    initialised = false;

    if (!InitialiseSDL(width, height))
        return;

    font = new Font("font");

    PhaseChange(New<Phase::Boot>());
//    PhaseChange(New<Phase::Play>());

    initialised = true;
    finished = false;
}

Game::~Game()
{
    ::Delete(phase);
    ::Delete(next_phase);
}

SDL_Surface *Game::GetSurface() const
{
    return screen;
}

GameTime Game::GetTime() const
{
    return time;
}

Color Game::MakeColor(int r, int g, int b) const
{
    return SDL_MapRGB(GetSurface()->format, r, g, b);
}

Time Game::TimeNow() const
{
    return time.TotalElapsedSeconds();
}

bool Game::Transitioning() const
{
    return next_phase != 0 && time.IsAfter(transition_ends);
}

void Game::Transist()
{
    if (time.IsAfter(transition_ends))
    {
        EndTransition();
    }
}

void Game::EndTransition()
{
    if (phase)
    {
        phase->Leave(next_phase);
    }
    if (next_phase)
    {
        next_phase->Enter(phase);
    }

//    ::Delete(phase);
    phase = next_phase;
    next_phase = 0;
    transition_ends = 0;
}

void Game::PhaseChange(Phase::Base *next, Time transition_time)
{
    if (!next)
        return;
    if (Transitioning())
        EndTransition();
    transition_ends = TimeNow() + transition_time;
    next_phase = next;
}

bool Game::Update(GameTime)
{
    time.StartFrame();

    SDL_LockAudio();
    {
        ParseInput();
        if (Transitioning())
        {
            Transist();
        }
        else if (phase)
        {
            phase->Update(time);
        }
    }
    SDL_UnlockAudio();

    GetFactory()->Purge();

    if (finished)
    {
        SDL_CloseAudio();
    }

    return !finished;
}

void Game::ParseInput()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (phase && phase->InputEvent(event))
            continue;
        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
            finished = true;
        if (event.type == SDL_QUIT)
            finished = true;
    }
}

void Game::Draw(Matrix const &transform)
{
    SDL_LockSurface(screen);
    SDL_FillRect(screen, 0, SDL_MapRGB(screen->format, 30, 30, 50));
    if (phase)
        phase->Draw(transform);
    SDL_UnlockSurface(screen);
    SDL_UpdateWindowSurface(window);
}

float t_last;
bool first = true;
double target_frequency;
double frequency;

void AudioCallback(void *userdata, Uint8 *stream, int len)
{
    Game *game = (Game *)userdata;
    Phase::Play *play = game->GetPhase<Phase::Play>();
    if (!play)
    {
        return;
    }

    typedef Sint8 Sample;
    double playback_rate = 8000;
    double dt = 1.0f/playback_rate;
    int sample_num_bytes = sizeof(Sample);

    target_frequency = 200;

    Player *player = game->GetWorld()->GetPlayer();
    Direction dir = player->GetDirection();

    if (dir == Direction::None)
    {
        target_frequency = 0;
    }

    target_frequency += dir.value*200;
    frequency += (target_frequency - frequency)/5;

    Sample *buffer = (Sample *)stream;
    int num_samples = len/sample_num_bytes;

    double t = t_last;
    for (int n = 0; n < num_samples; ++n)
    {
        double value = sin(t*frequency);
        Sample sample = 127*value;
        buffer[n] = sample;
        t += dt;

//        // half-way through, move closer to target
//        if (n > num_samples/2)
//        {
//            frequency += (target_frequency - frequency)/2;
//        }
    }
    t_last = t;
}

bool Game::InitialiseSDL(int width, int height)
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
    {
        printf( "Unable to init SDL: %s\n", SDL_GetError() );
        return false;
    }

    // make sure SDL clseans up before exit
    atexit(SDL_Quit);

    window = SDL_CreateWindow("SDL_RenderClear",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        width, height,
        0);

    //renderer = SDL_CreateRenderer(window, -1, 0);
    //if (!renderer)
    //{
    //    printf("Unable to create window: %s\n", SDL_GetError());
    //    return false;
    //}

    screen = SDL_GetWindowSurface(window);

    SDL_AudioSpec desired, obtained;

    // latency is freq/samples
    desired.freq = 8000;
    desired.format = AUDIO_S8;
    desired.samples = 256;
    desired.channels = 1;
    desired.callback = AudioCallback;
    desired.userdata = this;
    if (SDL_OpenAudio(&desired, &obtained) < 0)
    {
        printf("Unable to open audio device: %s\n", SDL_GetError());
        return 1;
    }

    printf("audio: freq=%d; samples=%d; channels=%d\n", obtained.freq, obtained.samples, obtained.channels);

    // lol
    SDL_PauseAudio(0);

    SDL_SetWindowTitle(window, "XIQ");
    return true;
}
