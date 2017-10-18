//
//  main.h
//  Pixelman2
//
//  Created by Julie Glasdam on 14/10/2017.
//  Copyright © 2017 Julie Glasdam. All rights reserved.
//

#define main_h

#include <SDL2/SDL.h>
#include <SDL2_ttf/SDL_ttf.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_mixer/SDL_mixer.h>

typedef struct {
    int x, y;
    float dy, dx;
    int speed;
    int currentSprite;
    int direction;
    int onLedge;
    short life;
} Player;

typedef struct {
    int x, y, w, h;
} Paths;


/* Create instances of all structs, so they can be easy referenced,
 by calling GameState. Textures are stored here instead of in their respective
 structs, so the same texture can be loaded for several objects, without
 having to initalize a new sdl texture every time*/
typedef struct {
    float scrollX;
    
    Player player;
    Paths ledgetest;
    
    // Textures
    SDL_Texture *texturePlayerSheet01;
    
    // Font
    TTF_Font *font;
} GameState;
