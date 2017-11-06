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
    int life;
} Player;




/* Create instances of all structs, so they can be easy referenced,
 by calling GameState. Textures are stored here instead of in their respective
 structs, so the same texture can be loaded for several objects, without
 having to initalize a new sdl texture every time*/
typedef struct {
    float scrollX;
    float scrollY;
    
    Player player;
   
    
    // Textures
    SDL_Texture *texturePlayerSheet01;
    SDL_Texture *tetxureHeartContainerOutline;
    SDL_Texture *textureHeartContainer;
    SDL_Texture *texturePath_1_a;
    SDL_Texture *textureMountain_lvl1_01;
    SDL_Texture *textureMountain_lvl1_02;
    SDL_Texture *textureMountain_lvl1_03;
    SDL_Texture *textureMountain_lvl1_04;
    SDL_Texture *textureMountain_lvl1_05;
    
    SDL_Texture *textCoordinates;
   
    // Font
    TTF_Font *font;
    
    SDL_Renderer *renderer;
} GameState;
