//
//  collisionDetection.c
//  Pixelman2
//
//  Created by Julie Glasdam on 21/10/2017.
//  Copyright © 2017 Julie Glasdam. All rights reserved.
//

#include <stdio.h>
#include "main.h"
#include "define.h"

// Collision detection for paths
void collisionDetectPlatforms(GameState *game, int x, int y, int w) {
    float mw = (float) PLAYER_W;                // Player width
    float mh = (float) PLAYER_H;                // Player height
    float mx = (float) game->player.x;          // Player x coordinate
    float my = (float) game->player.y;          // Player y coordinate
    float bx = (float) x;                       // x coordinate of object we are checking
    float by = (float) y;                       // y coordinate of object we are checking
    float bw = (float) w;                       // width of object we are checking
    
    /* If players x coordinate + players width is more than the x position of the top of the path
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
