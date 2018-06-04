# XIQ - A QIX clone written quickly

This is a _very_ simple game based on the old arcade game, **Qix**.

Now, this may seem like a really small, trivial and badly written game. And it is.

*However* it is written with practically no 3rd-party library source or components. It uses a framebuffer of NxM pixels, and a sound buffer of unsigned 8-bit bytes. These are provided by SDL, as well as input.

Nothing else is used. So when a line has to be drawn, I had to write a line-draw algorithm. When a sound effect had to be played, I had to write the sound values directly to the byte array for the sound buffer.

There is a custom Font library as well.

Basically, I wrote this to see what I could with the following self-imposed limitations:

1. A *framebuffer* of R8G8B8A0 pixels.
1. A *sound-buffer* of unsigned 8-bit bytes.
1. An *input system* that I could poll for key down/key up.
1. One weekend.
1. And *absolutely nothing else*.

So, yeah, it's a simple, badly-drawn game with no fancy graphics or sound or music or fonts. But it has graphics and sound and music and fonts, all custom made from the ground up.

## Instructions
Move with the cursor keys or WASD.
Get hit and you lose a life. 
You have three lives.

## Build Instructions

To build the game, you will need the SDL and boost libraries. 

To build for windows, you will also need at least DirectX 9.0 SDK.

Projects for Code::Blocks and Visual Studio 2013 are included.

The game builds and runs on Linux, OS/X and Windows.

## TODO

CMake support.
