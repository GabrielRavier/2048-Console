//
//  Game.h
//  2048
//
//  Created by Tom Hu on 12/21/14.
//  Copyright (c) 2014 Tom Hu. All rights reserved.
//

#ifndef ___048__Game__
#define ___048__Game__

typedef enum {
    MoveCommandUp,
    MoveCommandDown,
    MoveCommandLeft,
    MoveCommandRight
} MoveCommand;

class Game {
private:
    int score;
    bool over;
    bool won;
    
    int numberOfEmptyCells;
    
    static const int size = 4;
    static const int numberOfStartCells = 2;
    
    int chessboard[4][4];
    
    void initChessboard();
    void initStartCells();
    void addRandomCell();
    int getRandomAvailableCell() const;
    
    void clearCell(int row, int col);
    
#pragma mark Can Move?
    bool canMove(const MoveCommand cmd) const;
    bool canMoveUp() const;
    bool canMoveDown() const;
    bool canMoveLeft() const;
    bool canMoveRight() const;
    
public:
#pragma mark
#pragma mark Constructor & Destructor
    Game();
    ~Game();
    
#pragma mark -
#pragma mark Actions
    void doMove(const MoveCommand cmd);
    void doUp();
    void doDown();
    void doLeft();
    void doRight();
    
    void restart();
    
#pragma mark -
#pragma mark View
    void print() const;
    void printHelpInfo() const;
    
#pragma mark -
#pragma mark Is over or won or terminated?
    bool isOver() const;
    bool isWon() const;
    bool isTerminated() const;
};

#endif /* defined(___048__Game__) */
