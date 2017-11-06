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
#include "render_world_level01.h"


int globalTime = 0;
int developerMode = 1;
SDL_Color black = {0, 0, 0, 255};


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



// Create static text. Pass '-1' as x-parameter to center
void createStaticText(GameState *game, SDL_Renderer *renderer, char *text, SDL_Color color, int x, int y) {
    
    SDL_Surface *tmp = TTF_RenderText_Blended(game->font, text, color);
    if (x == -1) {
        x = (WINDOW_W/2)-(tmp->w/2);
    }
    SDL_Texture *tmpTex = SDL_CreateTextureFromSurface(renderer, tmp);
    SDL_Rect textRect = {x, y, tmp->w, tmp->h};
    SDL_RenderCopy(renderer, tmpTex, NULL, &textRect);
    SDL_FreeSurface(tmp);
    SDL_DestroyTexture(tmpTex);
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
    
     else if (state[SDL_SCANCODE_W] || state[SDL_SCANCODE_UP]) {
         if (game->player.dx == 0) {
              game->player.currentSprite = 9;
         }
     }
    
    // If he's not moving
    else {
        game->player.currentSprite = 0;
        game->player.dx *= 0.8f;
      //  game->man.slowingDown = 1;
        if(fabsf(game->player.dx) < 0.1f) {
            game->player.dx = 0;
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



/* Renders all drawing. Different levels need differt version of this. Later probably also make a logic function for each level */
void doRender(SDL_Renderer *renderer, GameState *game){
   scrolling(game);
    // Set background color
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    
    //Clear the screen
    SDL_RenderClear(renderer);
    
    // Render the world with 'render_world_level01.c'
    draw_paths_level01(game);
    draw_hillpaths_level01(game);
    
    // Draw texture ontop of world
    SDL_Rect mountain1Rect = { game->scrollX+0, game->scrollY+150, 194, 352 };
    SDL_RenderCopy(renderer, game->textureMountain_lvl1_01, NULL, &mountain1Rect);
    
    SDL_Rect mountain4Rect = { game->scrollX+380, game->scrollY+80, 589, 506 };
    SDL_RenderCopy(renderer, game->textureMountain_lvl1_04, NULL, &mountain4Rect);
    
    SDL_Rect mountain3Rect = { game->scrollX+465, game->scrollY+205, 383, 415 };
    SDL_RenderCopy(renderer, game->textureMountain_lvl1_03, NULL, &mountain3Rect);
    
    SDL_Rect mountain2Rect = { game->scrollX+330, game->scrollY+260, 410, 300 };
    SDL_RenderCopy(renderer, game->textureMountain_lvl1_02, NULL, &mountain2Rect);
    
    
    SDL_Rect path1aRect = { game->scrollX+0, game->scrollY+0, 1400, 600 };
    SDL_RenderCopy(renderer, game->texturePath_1_a, NULL, &path1aRect);
    
    
    // Draw hearts
    SDL_Rect heartContainerSrcRect = { 0, 0, 27*(int)game->player.life, 23};
    SDL_Rect heartContainerRect = { 20, 20, 27*(int)game->player.life, 23 };
    
    SDL_RenderCopyEx(renderer, game->textureHeartContainer, &heartContainerSrcRect, &heartContainerRect, 0, NULL,0);
    
    SDL_Rect heartsOutline = { 20, 20, 270, 23 };
    SDL_RenderCopy(renderer, game->tetxureHeartContainerOutline, NULL, &heartsOutline);
    
    //Draw player
    SDL_Rect playerSrcRect = { PLAYER_W*game->player.currentSprite, PLAYER_H*game->player.direction, PLAYER_W, PLAYER_H};
    SDL_Rect playerRect = { game->scrollX+game->player.x, game->scrollY+game->player.y, PLAYER_W, PLAYER_H };
    
    SDL_RenderCopyEx(renderer,                                  // renderer
                     game->texturePlayerSheet01,                // the texture to apply the rectangle
                     &playerSrcRect,                            // defines which part of texture to use. Leave null to use the whole image
                     &playerRect,                               // define target
                     0,                                         // the angle of target
                     NULL,                                      // the point to rotate. I don't know why this is null
                     0);                                        // 0 for no flip, 1 for flip
    
    
    // X coordinate
    char player_x[128] = "";
    sprintf(player_x, "X %d", (int)game->player.x);
    
    SDL_Surface *tmp = TTF_RenderText_Blended(game->font, player_x, black);
    SDL_Texture *tmpTex = SDL_CreateTextureFromSurface(renderer, tmp);
    SDL_Rect textRect = {20, 60, tmp->w, tmp->h};
    SDL_RenderCopy(renderer, tmpTex, NULL, &textRect);
    SDL_FreeSurface(tmp);
    SDL_DestroyTexture(tmpTex);
    
    
    // Y coordinate
    char player_y[128] = "";
    sprintf(player_y, "Y %d", (int)game->player.y+PLAYER_H);
    
    SDL_Surface *tmp2 = TTF_RenderText_Blended(game->font, player_y, black);
    SDL_Texture *tmp2Tex = SDL_CreateTextureFromSurface(renderer, tmp2);
    SDL_Rect text2Rect = {20, 80, tmp2->w, tmp2->h};
    SDL_RenderCopy(renderer, tmp2Tex, NULL, &text2Rect);
    SDL_FreeSurface(tmp2);
    SDL_DestroyTexture(tmp2Tex);
    
    // Developer mode
    if (developerMode == 0) {
        createStaticText(game, renderer, "DEVELOPER MODE: OFF", black, 20, 100);
    }
    else {
        createStaticText(game, renderer, "DEVELOPER MODE: ON", black, 20, 100);
    }
    
    
    // Level text
    createStaticText(game, renderer, "LEVEL 01", black, -1, 20);
    
    // We are done drawing, "present" or show to the screen what we've drawn
    SDL_RenderPresent(renderer);
    
}


void setLevel01( SDL_Renderer *renderer, GameState *game) {
    doRender(renderer, game);
    movePlayer(game);
    scrolling(game);
    
    // Player cant move past boundaries
    if (game->player.x < LEVEL_01_START) {
        game->player.x = LEVEL_01_START;
    }
    if (game->player.x > LEVEL_01_END) {
        game->player.x = LEVEL_01_END;
    }
    

    
    
}



/* Load all textures for the game, assign them to an SDL_Texture object and free the textures
 Parameters: Renderer and GameState object
 */
void loadTextures(SDL_Renderer *renderer, GameState *game) {
    SDL_Surface *surface = NULL;
    
    // Player spritesheet
    surface = IMG_Load("player_spritesheet_06.png");
    errorMsgLoadTextures("player_spritesheet_06.png", surface);
    game->texturePlayerSheet01 = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    
    // Heartcontainer
    surface = IMG_Load("heartsoutline.png");
    errorMsgLoadTextures("heartsoutline.png", surface);
    game->tetxureHeartContainerOutline = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    
    surface = IMG_Load("heartcontainer.png");
    errorMsgLoadTextures("heartcontainer.png", surface);
    game->textureHeartContainer = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    
    // Paths
    surface = IMG_Load("path_1a.png");
    errorMsgLoadTextures("path_1a.png", surface);
    game->texturePath_1_a = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    
    //Mountains
    surface = IMG_Load("mountain_lvl1_01.png");
    errorMsgLoadTextures("mountain_lvl1_01.png", surface);
    game->textureMountain_lvl1_01 = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    
    surface = IMG_Load("mountain_lvl1_02.png");
    errorMsgLoadTextures("mountain_lvl1_02.png", surface);
    game->textureMountain_lvl1_02 = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    
    surface = IMG_Load("mountain_lvl1_03.png");
    errorMsgLoadTextures("mountain_lvl1_03.png", surface);
    game->textureMountain_lvl1_03 = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    
    surface = IMG_Load("mountain_lvl1_04.png");
    errorMsgLoadTextures("mountain_lvl1_04.png", surface);
    game->textureMountain_lvl1_04 = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    
    surface = IMG_Load("mountain_lvl1_05.png");
    errorMsgLoadTextures("mountain_lvl1_05.png", surface);
    game->textureMountain_lvl1_05 = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    
    // Fonts
    game->font = TTF_OpenFont("FreePixel.ttf", FONT_SIZE);
    if (!game->font) {
        printf("Cannot load font");
        SDL_Quit();
        exit(1);
    }
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
    game->player.life = 4;
    game->player.onLedge = 0;
    game->scrollX = 0;
    game->scrollY = 0;
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
                    /* I don't know why the following three cases has to be here and not in
                     'checkKeyboardEvents', but he can't jump if it's not here. */
                    // Small jump in normal mode, high jump in developer mode
                    case SDLK_w:
                        if(game->player.onLedge) {
                            if (developerMode == 0) {
                                game->player.dy = -12;
                            }
                            else {
                                game->player.dy = -20;
                            }
                            game->player.currentSprite = 8;
                            game->player.onLedge = 0;
                            
                        }
                        break;
                    // Small jump in normal mode, high jump in developer mode
                    case SDLK_UP:
                        if(game->player.onLedge) {
                            if (developerMode == 0) {
                                game->player.dy = -12;
                            }
                            else {
                                game->player.dy = -20;
                            }
                            game->player.currentSprite = 8;
                            game->player.onLedge = 0;
                            
                            
                        }
                        break;
                    // Add small jump for developer mode
                    case SDLK_e:
                        if (developerMode == 1) {
                            game->player.dy = -12;
                        }
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




int main(int argc, char *argv[]) {
    int done = 0;
    
    SDL_Window *window = NULL;                    // Declare a window
    SDL_Renderer *renderer = NULL;                // Declare a renderer
    SDL_Init(SDL_INIT_VIDEO);                     // Initialize SDL
    TTF_Init();                                   // Initialize TTF
    GameState gamestate;                          // Create instance of GameState to utilize when calling methods
    
    //Create an application window with the following settings:
    window = SDL_CreateWindow("Pixelman 2",                      // window title
                              SDL_WINDOWPOS_UNDEFINED,           // initial x position
                              SDL_WINDOWPOS_UNDEFINED,           // initial y position
                              WINDOW_W,                          // width, in pixels
                              WINDOW_H,                          // height, in pixels
                              0                                  // flags
                              //| SDL_WINDOW_RESIZABLE             // enable resize
                              );
    
    /* Initialize renderer and set renderer-instance in gamestate to this, so all files, that have access
     to gamestate(main.h) can use the renderer. PREVENTSYNC flag makes animations run smoother */
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    gamestate.renderer = renderer;
    
    // Set the attributes to initial values, and load all the textures for the entire game
    setAttributes(&gamestate);
    loadTextures(renderer, &gamestate);
    
    
    // Run this as long as program is not exited
    while(!done){
        globalTime++;
        done = processEvents(window, &gamestate);
        
        // Right now this sets everything for first level. Might be changed when states are added
        setLevel01(renderer, &gamestate);
    }
    
    // Close and destroy the window
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    
    // Destroy textures
    SDL_DestroyTexture(gamestate.texturePlayerSheet01);
    SDL_DestroyTexture(gamestate.tetxureHeartContainerOutline);
    SDL_DestroyTexture(gamestate.textureHeartContainer);
    SDL_DestroyTexture(gamestate.texturePath_1_a);
    SDL_DestroyTexture(gamestate.textureMountain_lvl1_01);
    SDL_DestroyTexture(gamestate.textureMountain_lvl1_02);
    SDL_DestroyTexture(gamestate.textureMountain_lvl1_03);
    SDL_DestroyTexture(gamestate.textureMountain_lvl1_04);
    SDL_DestroyTexture(gamestate.textureMountain_lvl1_05);
    
    if (gamestate.textCoordinates != NULL) {
        SDL_DestroyTexture(gamestate.textCoordinates);
    }
    
    // Close font
    TTF_CloseFont(gamestate.font);
    
    // Clean up
    TTF_Quit();
    SDL_Quit();
    return 0;
}
