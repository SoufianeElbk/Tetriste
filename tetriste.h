#ifndef TETRISTE_H
#define TETRISTE_H

#include <string>
using namespace std;

enum class Color {
    BLUE,
    YELLOW,
    RED,
    GREEN
};

enum class Shape {
    SQUARE,
    DIAMOND,
    CIRCLE,
    TRIANGLE
};

class Piece {
public:
    Color color;         // The piece's color
    Shape shape;         // The piece's shape
    Piece* next;         // Circular linked list for pieces
    Piece* prevShape;    // Double circular linked list for shapes
    Piece* nextShape;
    Piece* prevColor;    // Double circular linked list for colors
    Piece* nextColor;
    string displayStr; // How the piece must be displayed in the terminal
};

class Game {
public:
    Piece* head;         // The first piece displayed from the left in the game
    int score;
    int piecesCount;
    int colorCount;      // The (difficulty) number of colors in the game
    int shapeCount;      // The (difficulty) number of shapes in the game
};

Game* initGame(int colorCount, int shapeCount);
Piece* generatePiece(int colorCount, int shapeCount);

string getDisplayStr(Color color, Shape shape);

int getPieceSequenceSize(Game* game, Piece* piece);
void leftInsert(Game* game, Piece* toInsert);
void rightInsert(Game* game, Piece* toInsert);
void shiftByColor(Game* game, Color color);
void shiftByShape(Game* game, Shape shape);
void updateShapes(Piece* piece);
void updateColors(Piece* piece);
int updateBoard(Game* game, int isByShift);
Piece* getTail(Game* game);
void saveGame(Game* game, Piece** nextPieces, string name);
Game* loadGame(Piece** nextPieces, string name);
void updateRankings(int score, string name);
void freePiece(Piece* piece);
void freeGame(Game* game);

#endif // TETRISTE_H