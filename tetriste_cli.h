#include "tetriste.h"

#ifndef TETRISTE_CLI_H
#define TETRISTE_CLI_H

// Main
void startCLI();
// Utilities
void askUpdateRankings(Game* currentGame);
// Game menus
void displayGameInfo(Game *currentGame, Piece **nextPieces, int scoreAdded);
void displayMainMenu();
void displayColorMenu();
void displayShapeMenu();


#endif //TETRISTE_CLI_H