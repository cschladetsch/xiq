# XIQ - A QIX clone written quickly

This is a _very_ simple game based on the old arcade game, **Qix**.

Now, this may seem like a really small, trivial and badly written game. And it is.

_However_ it is written with practically no 3rd-party library source or components. It uses a framebuffer of pixels, and a sound buffer of unsigned 8-bit bytes. These are provided by SDL, as well as input.

Nothing else is used. So when a line has to be drawn, I had to write a line-draw algorithm. Want to draw a circle? Then write the algorithm to plot the points of a circle (quickly).

When a sound effect had to be played, I had to write the sound values directly to the byte array for the sound buffer.

There is a custom **FontLibrary** as well. As well as an **ObjectFactory** and **GamePhase** management system.

Basically, I wrote this to see what I could with the following self-imposed limitations:

1. A _framebuffer_ of R8G8B8A0 pixels.
1. A _sound-buffer_ of unsigned 8-bit bytes.
1. An _input system_ that I could poll for key down/key up.
1. One weekend.
1. And _absolutely nothing else_.

So, it's a simple, badly-drawn game with no fancy graphics or sound or music or fonts. But it has graphics and sound and music and fonts, all custom made from the ground up.

## Instructions

- Move with the cursor keys or WASD.
- Get hit and you lose a life.
- You have three lives.

I know there's a problem with the game area being half the screen. That's a consequence of the 23-minute port from SDL1 to SDL2 and can't see much point fixing it.

## Build Instructions

### Linux

$ sudo apt install cmake libsdl2-dev
$ mkdir -p build && cd build
$ cmake ..
$ make

### Windows

Projects for VS2017 and VS2019 are included.

## TODO

CMake support.
