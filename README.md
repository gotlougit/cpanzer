# CPanzer

## Overview

This is a rewrite of [panzer](https://github.com/gotlougit/panzer) in C using [libSDL](https://www.libsdl.org/). It isn't a full 1:1 recreation, think of it more as a demo of the game instead. I wrote it because I wanted to get more familiar with how programs are written in C and as a fun way to learn SDL as well. It also paves the way for more ports to other systems as well, not just desktops.

## Requirements

Make sure you have the following in order to both build and run the game on your system:

```
gcc, make, SDL2, SDL2_image, SDL2_ttf, SDL2_mixer
```
(you can skip the last one for now if you want but it'll come in handy later)

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

## Compiling and Running the Game

Clone this git repository, change directory into it and run ```make```

```
$ git clone <repo-url>
$ cd cpanzer/
$ make
```

To run the game, run ```./panzer```

## Controls

| Key | Action |
|---|---|
| W or up arrow key | Moves player up |
| A or left arrow key | Moves player left |
| S or down arrow key | Moves player down |
| D or right arrow key | Moves player right |
| Q | Rotates nozzle slightly counter-clockwise |
| E | Rotates nozzle slightly clockwise |
| Space bar | Fires a projectile in the direction the nozzle is facing |
| P | Pauses the game |
| Esc | Exits the game |

## Source code structure

| File/Folder | Description |
|---|---|
| `constants.h` | Specifies constants like player speed, window name, dimensions etc. and also imports all the library functions we need |
| `input.h` | Abstracts away all input and returns a custom struct detailing the effects of input (like how much should player move up/down or left/right, if player pressed pause button etc. |
| `objects.h` | Creates the unfortunately named `textures` struct which contains all the information about each object, like player, object, base etc, as well as related functions |
| `window.c` | Renders the window and brings all this together |
| `assets` | Contains all the sprites, font files etc. for the game |

## Observed bugs

- valgrind reports ~6kb missing, it goes all the way back to the beginning of this game, so find bug and fix it (UPDATE: this is probably an SDL issue, so there's nothing I can do here except create a bug report)

- nozzle freaks out sometimes whenever someone collides with it

- movement is not as smooth as in original panzer

- enemies spawn in awkward positions, like inside base (again)

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

- Projectile

## Roadmap

- A better pause menu than the original Panzer

- Sound effects (look at sdlwiki/Tutorials-AudioStream.html)

- Fixing bugs

## Things to be left out

- MySQL integration for keeping track of scores; this could be added in the form of SQLite but it seems unlikely.

- Game menu screen

- Rage mode

- Map and player tank options

- Fancy HUD with a health bar

- Powerups, such as those for speed or getting more ammo

- A debug mode which gives you God mode in the game

## Font

The font used in the game HUD is [Ubuntu Regular](https://design.ubuntu.com/font/)
