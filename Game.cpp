//
//  Game.cpp
//  2048
//
//  Created by Tom Hu on 12/21/14.
//  Copyright (c) 2014 Tom Hu. All rights reserved.
//

#include "Game.h"
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>

void Game::initChessboard() {
    this->score = 0;
    this->over = false;
    this->won = false;
    this->numberOfEmptyCells = 16;
    
    // Init chessboard
    memset(this->chessboard, 0, 4 * 4 * sizeof(int));
    
    // Add the initial cells
    this->initStartCells();
    
    // Print
    this->print();
}

void Game::initStartCells() {
    for (int i = 0; i < this->numberOfStartCells; i++) {
        this->addRandomCell();
    }
}

void Game::addRandomCell() {
    int random = rand();
    int value = ((double)random / RAND_MAX) < 0.9 ? 2: 4;
    int index = this->getRandomAvailableCell();
    this->chessboard[index / this->size][index % this->size] = value;
    this->numberOfEmptyCells--;
}

int Game::getRandomAvailableCell() const {
    while (true) {
        int random = rand() % (this->size * this->size);
        int row = random / this->size;
        int col = random % this->size;
        if (this->chessboard[row][col] == 0)
            return random;
    }
    return -1;
}

void Game::clearCell(int row, int col) {
    this->chessboard[row][col] = 0;
    this->numberOfEmptyCells++;
}

#pragma mark -
#pragma mark Can Move?
bool Game::canMove(const MoveCommand cmd) const {
    switch (cmd) {
        case MoveCommandLeft:
            return this->canMoveLeft();
            break;
        case MoveCommandUp:
            return this->canMoveUp();
            break;
        case MoveCommandRight:
            return this->canMoveRight();
            break;
        case MoveCommandDown:
            return this->canMoveDown();
            break;
        default:
            break;
    }
    return false;
}

bool Game::canMoveUp() const {
    for (int j = 0; j < this->size; j++) {
        bool result = false;
        for (int i = 0; i < this->size; i++) {
            if (this->chessboard[i][j] == 0)
                result = true;
            else if ((i != 0 && this->chessboard[i][j] == this->chessboard[i - 1][j]) || result)
                    return true;
        }
    }
    return false;
}

bool Game::canMoveDown() const {
    for (int j = 0; j < this->size; j++) {
        bool result = false;
        for (int i = this->size - 1; i >= 0; i--) {
            if (this->chessboard[i][j] == 0)
                result = true;
            else if ((i != this->size - 1 && this->chessboard[i][j] == this->chessboard[i + 1][j]) || (result))
                return true;
        }
    }
    return false;
}

bool Game::canMoveLeft() const {
    for (int i = 0; i < this->size; i++) {
        bool result = false;
        for (int j = 0; j < this->size; j++) {
            if (this->chessboard[i][j] == 0)
                result = true;
            else if ((j != 0 && this->chessboard[i][j] == this->chessboard[i][j - 1]) || (result))
                return true;
        }
    }
    return false;
}

bool Game::canMoveRight() const {
    for (int i = 0; i < this->size; i++) {
        bool result = false;
        for (int j = this->size - 1; j >= 0; j--) {
            if (this->chessboard[i][j] == 0)
                result = true;
            else if ((j != this->size - 1 && this->chessboard[i][j] == this->chessboard[i][j + 1]) || (result))
                return true;
        }
    }
    return false;
}

#pragma mark -
#pragma mark Constructor & Destructor
Game::Game() {
    this->printHelpInfo(); // Help info
    srand((unsigned)time(0));
    this->initChessboard(); // Init chessboard
}

Game::~Game() {
    
}

#pragma mark -
#pragma mark Actions
void Game::doMove(const MoveCommand cmd) {
    if (!this->canMove(cmd)) return;
    
    switch (cmd) {
        case MoveCommandLeft:
            this->doLeft();
            break;
        case MoveCommandUp:
            this->doUp();
            break;
        case MoveCommandRight:
            this->doRight();
            break;
        case MoveCommandDown:
            this->doDown();
            break;
        default:
            break;
    }
    
    std::cout << (cmd == MoveCommandUp ? "Move Up" : (cmd == MoveCommandDown ? "Move Down" : (cmd == MoveCommandLeft ? "Move Left" : "Move Right"))) << std::endl;
    this->addRandomCell();
    
    if (this->numberOfEmptyCells == 0) {
        if (this->score < 2048)
            this->over = true;
        else
            this->won = true;
    }
    
    this->print();
}

void Game::doUp() {
    for (int j = 0; j < this->size; j++) {
        int l = 0, m = -1, n = -1;
        for (int i = 0; i < this->size; i++) {
            if (this->chessboard[i][j] != 0) {
                if (m == -1) {
                    m = i;
                } else {
                    if (n == -1) {
                        n = i;
                        if (this->chessboard[m][j] == this->chessboard[n][j]) {
                            // Merge
                            this->chessboard[l][j] = this->chessboard[m][j] * 2;
                            if (m != l) {
                                this->numberOfEmptyCells--;
                                this->clearCell(m, j);
                            }
                            this->clearCell(n, j);
                            
                            // Add score
                            this->score += this->chessboard[l][j];
                            
                            m = -1;
                        } else {
                            // Move
                            if (m != l) {
                                this->chessboard[l][j] = this->chessboard[m][j];
                                this->numberOfEmptyCells--;
                                this->clearCell(m, j);
                            }
                            
                            m = n;
                        }
                        
                        l++;
                        n = -1;
                    }
                }
            }
        }
        if (m != -1 && m != l) {
            this->chessboard[l][j] = this->chessboard[m][j];
            this->numberOfEmptyCells--;
            this->clearCell(m, j);
        }
    }
}

void Game::doDown() {
    for (int j = 0; j < this->size; j++) {
        int l = this->size - 1, m = -1, n = -1;
        for (int i = this->size - 1; i >= 0; i--) {
            if (this->chessboard[i][j] != 0) {
                if (m == -1) {
                    m = i;
                } else {
                    if (n == -1) {
                        n = i;
                        if (this->chessboard[m][j] == this->chessboard[n][j]) {
                            // Merge
                            this->chessboard[l][j] = this->chessboard[m][j] * 2;
                            if (m != l) {
                                this->numberOfEmptyCells--;
                                this->clearCell(m, j);
                            }
                            this->clearCell(n, j);
                            
                            // Add score
                            this->score += this->chessboard[l][j];
                            
                            m = -1;
                        } else {
                            // Move
                            if (m != l) {
                                this->chessboard[l][j] = this->chessboard[m][j];
                                this->numberOfEmptyCells--;
                                this->clearCell(m, j);
                            }
                            
                            m = n;
                        }
                        
                        l--;
                        n = -1;
                    }
                }
            }
        }
        if (m != -1 && m != l) {
            this->chessboard[l][j] = this->chessboard[m][j];
            this->numberOfEmptyCells--;
            this->clearCell(m, j);
        }
    }
}

void Game::doLeft() {
    for (int i = 0; i < this->size; i++) {
        int l = 0, m = -1, n = -1;
        for (int j = 0; j < this->size; j++) {
            if (this->chessboard[i][j] != 0) {
                if (m == -1) {
                    m = j;
                } else {
                    if (n == -1) {
                        n = j;
                        if (this->chessboard[i][m] == this->chessboard[i][n]) {
                            // Merge
                            this->chessboard[i][l] = this->chessboard[i][m] * 2;
                            if (m != l) {
                                this->numberOfEmptyCells--;
                                this->clearCell(i, m);
                            }
                            this->clearCell(i, n);
                            
                            // Add score
                            this->score += this->chessboard[i][l];
                            
                            m = -1;
                        } else {
                            // Move
                            if (m != l) {
                                this->chessboard[i][l] = this->chessboard[i][m];
                                this->numberOfEmptyCells--;
                                this->clearCell(i, m);
                            }
                            
                            m = n;
                        }
                        
                        l++;
                        n = -1;
                    }
                }
            }
        }
        if (m != -1 && m != l) {
            this->chessboard[i][l] = this->chessboard[i][m];
            this->numberOfEmptyCells--;
            this->clearCell(i, m);
        }
    }
}

void Game::doRight() {
    for (int i = 0; i < this->size; i++) {
        int l = this->size - 1, m = -1, n = -1;
        for (int j = this->size - 1; j >= 0; j--) {
            if (this->chessboard[i][j] != 0) {
                if (m == -1) {
                    m = j;
                } else {
                    if (n == -1) {
                        n = j;
                        if (this->chessboard[i][m] == this->chessboard[i][n]) {
                            // Merge
                            this->chessboard[i][l] = this->chessboard[i][m] * 2;
                            if (m != l) {
                                this->numberOfEmptyCells--;
                                this->clearCell(i, m);
                            }
                            this->clearCell(i, n);
                            
                            // Add score
                            this->score += this->chessboard[i][l];
                            
                            m = -1;
                        } else {
                            // Move
                            if (m != l) {
                                this->chessboard[i][l] = this->chessboard[i][m];
                                this->numberOfEmptyCells--;
                                this->clearCell(i, m);
                            }
                            
                            m = n;
                        }
                        
                        l--;
                        n = -1;
                    }
                }
            }
        }
        if (m != -1 && m != l) {
            this->chessboard[i][l] = this->chessboard[i][m];
            this->numberOfEmptyCells--;
            this->clearCell(i, m);
        }
    }
}

void Game::restart() {
    this->initChessboard(); // Init chessboard
}

#pragma mark -
#pragma mark Is over or won?
bool Game::isOver() const {
    return this->over;
}

bool Game::isWon() const {
    return this->won;
}

bool Game::isTerminated() const {
    return this->over || this->won;
}

#pragma mark -
#pragma mark View
void Game::print() const {
    std::cout << "================================================================" << std::endl;
    std::cout << "Score: " << this->score << std::endl;
    std::cout << "Empty: " << this->numberOfEmptyCells << std::endl;
    
    if (this->isTerminated()) {
        if (this->won)
            std::cout << "YOU WIN!!!" << std::endl;
        else if (this->over)
            std::cout << "YOU LOSE!!!" << std::endl;
    }
    
    for (int i = 0; i < this->size; i++) {
        for (int j = 0; j < this->size; j++) {
            printf("%4d ", this->chessboard[i][j]);
        }
        std::cout << std::endl;
    }
    std::cout << "================================================================" << std::endl << std::endl;
}

void Game::printHelpInfo() const {
    // 2048
    std::cout << std::endl;
    std::cout << "      ####            ####               ##          ######     " << std::endl;
    std::cout << "   ##########       ########            ##         ###    ###   " << std::endl;
    std::cout << " #####    #####    ###    ###          ##  ###    ###      ###  " << std::endl;
    std::cout << " ###       ####   ###      ###        ##   ###   ###        ### " << std::endl;
    std::cout << "          ####   ###        ###      ##    ###    ###      ###  " << std::endl;
    std::cout << "         ####    ###        ###     ##     ###     ###    ###   " << std::endl;
    std::cout << "        ####     ###        ###    ##      ###       ######     " << std::endl;
    std::cout << "       ####      ###        ###   ##       ###     ####  ####   " << std::endl;
    std::cout << "      ####       ###        ###  ###       ###    ###      ###  " << std::endl;
    std::cout << "     ####        ###        ###  ##############  ###        ### " << std::endl;
    std::cout << "    ####         ###        ###  ##############  ###        ### " << std::endl;
    std::cout << "   ####           ###      ###             ###   ###        ### " << std::endl;
    std::cout << "  ####             ###    ###              ###    ###      ###  " << std::endl;
    std::cout << " ##############     ########               ###     ###    ###   " << std::endl;
    std::cout << " ##############       ####                 ###       ######     " << std::endl;
    std::cout << std::endl;
    
    // Help
    std::cout << "Control:" << std::endl;
    std::cout << "WASD: A - Left, S - Down, W - Up, D - Right" << std::endl;
    std::cout << "Vim:  H - Left, J - Down, K - Up, L - Right" << std::endl;
    std::cout << std::endl;
}
