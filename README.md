# CPanzer

## Overview

This is a rewrite of [panzer](https://github.com/gotlougit/panzer) in C using [libSDL](https://www.libsdl.org/). It isn't a full 1:1 recreation, think of it more as a demo of the game instead. I wrote it because I wanted to get more familiar with how programs are written in C and as a fun way to learn SDL as well. It also paves the way for more ports to other systems as well, not just desktops.

## Requirements

Make sure you have the following in order to both build and run the game on your system:

gcc, make, SDL2, SDL2_image, SDL2_ttf, SDL2_mixer (you can skip this for now if you want but it'll come in handy later)

These packages vary by Linux distro, so here are the common ones:

- Fedora:

```
SDL2 SDL2_image SDL2_mixer SDL2_ttf SDL2-devel SDL2_mixer-devel SDL2_image-devel SDL2_ttf-devel
```

- Ubuntu:

```
libsdl2 libsdl2-dev libsdl2-mixer libsdl2-mixer-dev libsdl2-image libsdl2-image-dev libsdl2-ttf libsdl2-ttf-dev
```

Note that macOS and Windows aren't supported right now; all development work is focused on Linux. However, it is planned to add support for compiling to Windows. macOS may not be possible due to the lack of a Mac for development and testing purposes.

## Usage

Clone this git repository, change directory into it and run ```make```

```
$ git clone <repo-url>
$ cd cpanzer/
$ make
```

To run the game, run ```./panzer```

## Observed bugs

- valgrind reports ~6kb missing, it goes all the way back to the beginning of this game, so find bug and fix it

- some enemies spawn inside base and then there's no fixing them

- base and player start out at wrong positions

- nozzle freaks out sometimes whenever someone collides with it

- movement is not as smooth as in original panzer

## Implemented Features

- Player's basic movement

- Collision detection

- HUD Space

- Pausing

- HUD info

- Enemy AI

- Rotating player when they move a certain way

- Enemy auto respawn when player kills some of them off

- Nozzle 

- Base

## Roadmap

- Projectile

- A better pause menu than the original Panzer

- Sound effects (look at sdlwiki/Tutorials-AudioStream.html)

## Things to be left out

- MySQL integration for keeping track of scores; this could be added in the form of SQLite but it seems unlikely.

- Game menu screen

- Rage mode

- Map and player tank options
