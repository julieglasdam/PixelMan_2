//
//  render_world_level01.c
//  Pixelman2
//
//  Created by Julie Glasdam on 21/10/2017.
//  Copyright © 2017 Julie Glasdam. All rights reserved.
//
//  =============
//  Renders the basic pathways, as black boxes, and adds collision detection to them
//  =============

#include <stdio.h>
#include "main.h"
#include "define.h"
#include "collisionDetection.h"

// Helper function - Generate an individual rect, and add collision detection for it. This function should not be called by other files
void generatePath(GameState *game, int x, int y, int w, int h) {
    SDL_Renderer *renderer = game->renderer;
    
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_Rect pathRect = { game->scrollX+x, game->scrollY+y, w, h };
    SDL_RenderFillRect(renderer, &pathRect);
    collisionDetectPlatforms(game, x, y, w);
}

// Create all of the paths on ground level using 'generate paths'. This function can be called from other files
// Params: x, y, w, h
void draw_paths_level01(GameState *game) {
    // Start path
    generatePath(game, 0, 500, 300, 100);
    
    // Hill down
    for (int i = 0; i < 5; i++) { generatePath(game, 300+(i*10), 505+(i*5), 10, 95); }
    
    // Second path
    generatePath(game, 350, 530, 230, 70);
    
    // Second hill - down
    generatePath(game, 580, 535, 20, 85);
    generatePath(game, 600, 540, 20, 85);
    
    // Third path
    generatePath(game, 620, 545, 300, 85);

    // Third hill - up
    for (int i = 0; i < 6; i++) { generatePath(game, 920+(i*30), 540-(i*5), 30, 85); }
    
    // Fourth path
    generatePath(game, 1100, 510, 300, 90);
    
    
}

// Create all of the paths of the hills using 'generate paths'. This function can be called from other files
// Params: x, y, w, h
void draw_hillpaths_level01(GameState *game) {
    // Mountain 1
    generatePath(game, 0, 150, 150, 10);
    
    // Mountain 2
    for (int i = 0; i < 5; i++) { generatePath(game, 698-(i*10), 460-(i*5), 30, 5); }
    for (int i = 0; i < 5; i++) { generatePath(game, 648-(i*10), 432-(i*10), 10, 5); }
    for (int i = 0; i < 5; i++) { generatePath(game, 578-(i*10), 390-(i*10), 30, 10); }
    for (int i = 0; i < 7; i++) { generatePath(game, 528-(i*10), 345-(i*5), 30, 10); }
    for (int i = 0; i < 10; i++) { generatePath(game, 458-(i*10), 310-(i*5), 30, 10); }
    generatePath(game, 339, 265, 40, 10);
    
    // Mountain 3
    for (int i = 0; i < 6; i++) { generatePath(game, 500+(i*30), 230-(i*5), 30, 10); }
    
    // Mountain 4
    generatePath(game, 388, 80, 150, 10);
    generatePath(game, 538, 90, 20, 10);
    generatePath(game, 558, 100, 20, 10);
    generatePath(game, 578, 110, 200, 10);
    
    // Mountain 5
    //generatePath(game, 270, 320, 100, 10);
}
