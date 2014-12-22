//
//  main.cpp
//  2048
//
//  Created by Tom Hu on 12/21/14.
//  Copyright (c) 2014 Tom Hu. All rights reserved.
//

#include <cstdio>
#include <iostream>
#include "Game.h"

int main(int argc, const char * argv[]) {
    Game game;
    
    char cmd;
    cmd = getchar();
    while (true) {
        if (cmd == 'q') break; // Quit
        if (cmd == 'r' && game.isTerminated()) game.restart(); // Restart
        
        /**
         * Move
         *
         * WASD: A - Left, S - Down, W - Up, D - Right
         * Vim:  H - Left, J - Down, K - Up, L - Right
         */
        switch (cmd) {
            case 'a':
            case 'h':
                game.doMove(MoveCommandLeft); // Left
                break;
            case 'w':
            case 'k':
                game.doMove(MoveCommandUp); // Up
                break;
            case 'd':
            case 'l':
                game.doMove(MoveCommandRight); // Right
                break;
            case 's':
            case 'j':
                game.doMove(MoveCommandDown); // Down
                break;
            default:
                break;
        }
        
        if (game.isTerminated()) game.print();
        
        fflush(stdin);
        cmd = getchar();
    }
    
    return 0;
}
