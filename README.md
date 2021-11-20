# CPanzer

## Overview

This is a rewrite of [panzer](https://github.com/gotlougit/panzer) in C using [libSDL](https://www.libsdl.org/). It isn't a full 1:1 recreation, think of it more as a demo of the game instead. I wrote it because I wanted to get more familiar with how programs are written in C and as a fun way to learn SDL as well.

## Usage

Clone this git repository, change directory into it and run ```make```

```
$ git clone <repo-url>
$ cd cpanzer/
$ make
```

To run the game, run ```./panzer```

## Implemented Features

- Player's basic movement

- Collision detection

- HUD Space

- Pausing

- HUD info

- Enemy AI

- Rotating player when they move a certain way

- Enemy auto respawn when player kills some of them off

## Roadmap

- Nozzle 

- Projectile

- Base

- A better pause menu than the original Panzer

- Sound effects (look at sdlwiki/Tutorials-AudioStream.html)

## Things to be left out

- MySQL integration for keeping track of scores; this could be added in the form of SQLite but it seems unlikely.

- Game menu screen

- Rage mode

- Map and player tank options
