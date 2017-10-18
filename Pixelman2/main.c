//
//  main.c
//  Pixelman2
//
//  Created by Julie Glasdam on 14/10/2017.
//  Copyright © 2017 Julie Glasdam. All rights reserved.
//

#include <stdio.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_ttf/SDL_ttf.h>
#include <SDL2_mixer/SDL_mixer.h>
#include "main.h"
#include "define.h"


int globalTime = 0;


// Print message to console
void printLog(char *message) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    printf("[%d:%d:%d] %s\n", tm.tm_hour, tm.tm_min, tm.tm_sec, message);
}


// Error message in log if a sprite is not found
void errorMsgLoadTextures(char *message, SDL_Surface *surface) {
    if (surface == NULL) {
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);
        printf("[%d:%d:%d] Cannot find %s\n", tm.tm_hour, tm.tm_min, tm.tm_sec, message);
        SDL_Quit();
        exit(1);
    }
}

// Method is used to change the sprite when running, and actually move the player
void movePlayer(GameState *game) {
    
    // Add gravity to the variable that controls the players movement on the y axis
    game->player.dy += GRAVITY;
    
    // For every frame, one is added to the x/y positions
    game->player.x += game->player.dx;
    game->player.y += game->player.dy;
    
    // If he is not standing still and the is not in the air
    if(game->player.dx != 0 && game->player.onLedge) {
        if (globalTime % 5 == 0) {
            // Keep changing the sprite until the end of the spritesheet
            if (game->player.currentSprite < 7) {
                game->player.currentSprite++;
            }
            // Set the sprite back to 1 (Sprite 0 is not running)
            else {
                game->player.currentSprite = 1;
            }
        }
    }
    
}

// Method checks if a key is pressed and acts accordingly
void checkKeyboardEvent(GameState *game) {
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    
    if(state[SDL_SCANCODE_A] || state[SDL_SCANCODE_LEFT]) {
        game->player.direction = 1;                         // Change the level on the spritesheet
        game->player.dx -= 0.5;                             // The exact number doesn't matter, but it has to be there or he will not move
        if(game->player.dx < -(game->player.speed)) {       // Stops him from gaining more speed, when he reaches his max speed
            game->player.dx = -(game->player.speed);        // His speed (max speed)
        }
    }
    
     else if(state[SDL_SCANCODE_D] || state[SDL_SCANCODE_RIGHT]){
        game->player.direction = 0;
         game->player.dx += 0.5;
         if(game->player.dx > game->player.speed) {
             game->player.dx = game->player.speed;
         }
    }
    
    else if(state[SDL_SCANCODE_W] || state[SDL_SCANCODE_UP]){
        game->player.direction = 0;
        game->player.dy -= 2;
        if(game->player.dy < 6) {
            game->player.dy = 6;
        }
    }
    else if(state[SDL_SCANCODE_S] || state[SDL_SCANCODE_DOWN]){
        game->player.direction = 0;
        game->player.dy += 0.5;
        if(game->player.dy > 6) {
            game->player.dy = 6;
        }
    }
    else {
        game->player.currentSprite = 0;
        
        game->player.dx *= 0.8f;
      //  game->man.slowingDown = 1;
        if(fabsf(game->player.dx) < 0.1f) {
            game->player.dx = 0;
        }
    }
}


/* Check if any keys or buttons are pressed to close window
 The method returns an int to let main know if program should be terminated or not
 */
int processEvents(SDL_Window *window, GameState *game) {
    SDL_Event event;
    int done = 0;
    
    while(SDL_PollEvent(&event)) {
        switch(event.type) {
            case SDL_WINDOWEVENT_CLOSE:{
                if(window){
                    SDL_DestroyWindow(window);
                    window = NULL;
                    done = 1;
                }
            }
                break;
            case SDL_KEYDOWN: {
                switch(event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        done = 1;
                        break;
                }
            }
                break;
            case SDL_QUIT:
                done = 1;
                break;
        }
    }
    checkKeyboardEvent(game);
    return done;
}



// Collision detection for paths
void collisionDetectPlatforms(GameState *game, int x, int y, int w) {
    float mw = (float) PLAYER_W;                // Player width
    float mh = (float) PLAYER_H;                // Player height
    float mx = (float) game->player.x;          // Player x coordinate
    float my = (float) game->player.y;          // Player y coordinate
    float bx = (float) x;                       // x coordinate of object we are checking
    float by = (float) y;                       // y coordinate of object we are checking
    float bw = (float) w;                       // width of object we are checking
    
    /* If players x coordinate + players height is more than the x position of the top of the path
    (remember to check if up key is pressed) */
    if(mx+mw > bx && mx<bx+bw){
    
       /* If players y coordinate + players height is more than the top of the path and the players y coordinate
       is less than the top of the path and the player is moving */
        if(my+mh > by && my < by && game->player.dy > 0) {
            game->player.y = by-mh;              // Players y position is y coordinate of ledge - mans height
            my = by-mh;                          // Players y position is top of ledge - mans height
            game->player.dy = 0;                 // Player is not moving up or down
            game->player.onLedge = 1;            // Player is on the platform
        }
    }
}

// Besides calling this function, add the scrollX variable to the x position, when making a rect
void scrolling(GameState *game) {
    // Making the game scroll. Set scrollX to minus to make it scroll forwards. Offset by half the screen (+300) to center the player
    game->scrollX = -game->player.x+300;
    
    // Make sure we cant see anything to the left at the beginning of the level
    if(game->scrollX > 0) {
        game->scrollX = 0;
    }
    
    // Making sure we cant see anything to the right in the end
    else if(game->scrollX < -500) {
         game->scrollX = -500;
    }
}



/* This method draws the background and presents it to the window
 Parameters: renderer object and gamestate object
 */
void doRender(SDL_Renderer *renderer, GameState *game){
   
    // Set background color
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    
    //Clear the screen
    SDL_RenderClear(renderer);
    
    //Draw player
    SDL_Rect playerSrcRect = { PLAYER_W*game->player.currentSprite, PLAYER_H*game->player.direction, PLAYER_W, PLAYER_H};
    SDL_Rect playerRect = { game->scrollX+game->player.x, game->player.y, PLAYER_W, PLAYER_H };
    
    SDL_RenderCopyEx(renderer,                                  // renderer
                     game->texturePlayerSheet01,                // the texture to apply the rectangle
                     &playerSrcRect,                            // defines which part of texture to use. Leave null to use the whole image
                     &playerRect,                               // define target
                     0,                                         // the angle of target
                     NULL,                                      // the point to rotate. I don't know why this is null
                     0);                                        // 0 for no flip, 1 for flip
    
//====================================
// This needs to be in a seperate function eventually
    
    movePlayer(game);
    scrolling(game);
    
    // Draw test path
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    // x, y, w, h
    SDL_Rect testPathRect01 = { game->scrollX+0, 500, 300, 100 }; // Start path
    SDL_RenderFillRect(renderer, &testPathRect01);
    collisionDetectPlatforms(game, 0, 500, 300);
    
    
    SDL_Rect testPathRect02 = { game->scrollX+300, 505, 10, 95 }; // Hill
    SDL_RenderFillRect(renderer, &testPathRect02);
    collisionDetectPlatforms(game, 300, 505, 10);
    
    SDL_Rect testPathRect03 = { game->scrollX+310, 510, 10, 90 };
    SDL_RenderFillRect(renderer, &testPathRect03);
    collisionDetectPlatforms(game, 310, 510, 10);
    
    SDL_Rect testPathRect04 = { game->scrollX+320, 515, 10, 85 };
    SDL_RenderFillRect(renderer, &testPathRect04);
    collisionDetectPlatforms(game, 320, 515, 10);
    
    SDL_Rect testPathRect05 = { game->scrollX+330, 520, 10, 80 };
    SDL_RenderFillRect(renderer, &testPathRect05);
    collisionDetectPlatforms(game, 330, 520, 10);
    
    
    SDL_Rect testPathRect06 = { game->scrollX+340, 525, 10, 75 };
    SDL_RenderFillRect(renderer, &testPathRect06);
    collisionDetectPlatforms(game, 340, 525, 10);
    
    
    SDL_Rect testPathRect07 = { game->scrollX+350, 530, 230, 70 }; // Second path
    SDL_RenderFillRect(renderer, &testPathRect07);
    collisionDetectPlatforms(game, 350, 530, 230);
    
    
    SDL_Rect testPathRect08 = { game->scrollX+580, 535, 20, 85 }; // Hill
    SDL_RenderFillRect(renderer, &testPathRect08);
    collisionDetectPlatforms(game, 580, 535, 20);
    
    SDL_Rect testPathRect09 = { game->scrollX+600, 540, 20, 85 };
    SDL_RenderFillRect(renderer, &testPathRect09);
    collisionDetectPlatforms(game, 600, 540, 20);
    
    
    SDL_Rect testPathRect10 = { game->scrollX+620, 545, 300, 85 }; // Third path
    SDL_RenderFillRect(renderer, &testPathRect10);
    collisionDetectPlatforms(game, 620, 545, 300);
    
    
    SDL_Rect testPathRect11 = { game->scrollX+920, 540, 30, 85 }; // Hill
    SDL_RenderFillRect(renderer, &testPathRect11);
    collisionDetectPlatforms(game, 920, 540, 30);
    
    SDL_Rect testPathRect12 = { game->scrollX+950, 535, 30, 85 };
    SDL_RenderFillRect(renderer, &testPathRect12);
    collisionDetectPlatforms(game, 950, 535, 30);
    
    SDL_Rect testPathRect13 = { game->scrollX+980, 530, 30, 85 };
    SDL_RenderFillRect(renderer, &testPathRect13);
    collisionDetectPlatforms(game, 980, 530, 30);
    
    SDL_Rect testPathRect14 = { game->scrollX+1010, 525, 30, 85 };
    SDL_RenderFillRect(renderer, &testPathRect14);
    collisionDetectPlatforms(game, 1010, 525, 30);
    
    SDL_Rect testPathRect15 = { game->scrollX+1040, 520, 30, 85 };
    SDL_RenderFillRect(renderer, &testPathRect15);
    collisionDetectPlatforms(game, 1040, 520, 30);
    
    SDL_Rect testPathRect16 = { game->scrollX+1070, 515, 30, 85 };
    SDL_RenderFillRect(renderer, &testPathRect16);
    collisionDetectPlatforms(game, 1070, 515, 30);
    
    
    SDL_Rect testPathRect17 = { game->scrollX+1100, 510, 300, 90 };  // Fourth path
    SDL_RenderFillRect(renderer, &testPathRect17);
    collisionDetectPlatforms(game, 1100, 510, 300);
    
    
    
 //====================================
    
    //We are done drawing, "present" or show to the screen what we've drawn
    SDL_RenderPresent(renderer);
}


/* Load all textures for the game, assign them to an SDL_Texture object and free the textures
 Parameters: Renderer and GameState object
 */
void loadTextures(SDL_Renderer *renderer, GameState *game) {
    SDL_Surface *surface = NULL;
    
    // Player spritesheet
    surface = IMG_Load("player_spritesheet_05.png");
    errorMsgLoadTextures("player_spritesheet_05.png", surface);
    game->texturePlayerSheet01 = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
}

// Set attributes
void setAttributes(GameState *game) {
    game->player.x = 220;
    game->player.y = 446;
    game->player.speed = 5;
    game->player.dy = 0;
    game->player.dx = 0;
    game->player.currentSprite = 0;
    game->player.direction = 0;
    game->player.life = 10;
    game->player.onLedge = 0;
    game->scrollX = 0;
}

int main(int argc, char *argv[]) {
    int done = 0;
    
    SDL_Window *window;                    // Declare a window
    SDL_Renderer *renderer;                // Declare a renderer
    SDL_Init(SDL_INIT_VIDEO);              // Initialize SDL2
    
    GameState gamestate;                   // Create instance of GameState to utilize when calling methods
    
    //Create an application window with the following settings:
    window = SDL_CreateWindow("Pixelman 2",                      // window title
                              SDL_WINDOWPOS_UNDEFINED,           // initial x position
                              SDL_WINDOWPOS_UNDEFINED,           // initial y position
                              WINDOW_W,                          // width, in pixels
                              WINDOW_H,                          // height, in pixels
                              0                                  // flags
                              //| SDL_WINDOW_RESIZABLE             // enable resize
                              );
    
    // Initialize renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC); // PREVENTSYNC makes animations run smoother
    
    // Set player attributes, before beginning game
    setAttributes(&gamestate);
    
    // Load textures
     loadTextures(renderer, &gamestate);
    
    // Run this as long as program is not exited
    while(!done){
        globalTime++;
        done = processEvents(window, &gamestate);
        doRender(renderer, &gamestate);
    }
    
    // Close and destroy the window
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    
    // Destroy textures
    SDL_DestroyTexture(gamestate.texturePlayerSheet01);
    
    // Clean up
    TTF_Quit();
    SDL_Quit();
    return 0;
}
