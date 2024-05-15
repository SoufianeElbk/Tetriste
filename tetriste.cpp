#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string>
#include <string.h>
#include <fstream>
#include <vector>
#include <conio.h>
#include "tetriste.h"

using namespace std;

// Initialize a new game and adds the first piece
Game* initGame(int colorCount, int shapeCount) {
    Piece* newPiece = generatePiece(colorCount, shapeCount);
    newPiece->next = newPiece;
    newPiece->prevShape = newPiece;
    newPiece->nextShape = newPiece;
    newPiece->prevColor = newPiece;
    newPiece->nextColor = newPiece;

    Game* newGame = new Game();
    newGame->head = newPiece;
    newGame->score = 0;
    newGame->piecesCount = 1;
    newGame->colorCount = colorCount;
    newGame->shapeCount = shapeCount;
    return newGame;
}

// Generate a new piece with random color and shape
Piece* generatePiece(int colorCount, int shapeCount) {
    Color color = static_cast<Color>(rand() % colorCount);
    Shape shape = static_cast<Shape>(rand() % shapeCount);

    Piece* newPiece = new Piece();
    newPiece->color = color;
    newPiece->shape = shape;

    newPiece->next = NULL;
    newPiece->prevShape = NULL;
    newPiece->nextShape = NULL;
    newPiece->prevColor = NULL;
    newPiece->nextColor = NULL;

    newPiece->displayStr = getDisplayStr(color, shape);

    return newPiece;
}

string getDisplayStr(Color color, Shape shape) {
    string shapes[] = {"■", "◊", "●", "▲"};
    string colors[] = {"\033[34m", "\033[33m", "\033[31m", "\033[32m"};
    string resetColor = "\033[0m";

    string displayStr = colors[static_cast<int>(color)] + shapes[static_cast<int>(shape)] + resetColor;

    return displayStr;
}

// Inserts a piece on the left side of the board
void leftInsert(Game* game, Piece* toInsert) {
    rightInsert(game, toInsert);
    game->head = toInsert;
}

// Inserts a piece on the right side of the board
void rightInsert(Game* game, Piece* toInsert) {
    Piece* tail = getTail(game);

    tail->next = toInsert;
    toInsert->next = game->head;

    updateShapes(toInsert);
    updateColors(toInsert);
    game->piecesCount++;
}

// Returns the last displayed piece "tail"
Piece* getTail(Game* game) {
    Piece* tail = game->head;
    while (tail->next != game->head) {
        tail = tail->next;
    }
    return tail;
}

// Updates the doubly circular linked list of shapes for a given piece that is added to the board
void updateShapes(Piece* piece) {
    Piece* current = piece->next;
    while (current->shape != piece->shape) {
        current = current->next;
    }
    if (current == piece) {
        piece->prevShape = piece;
        piece->nextShape = piece;
    } else {
        piece->prevShape = current->prevShape;
        piece->prevShape->nextShape = piece;
        piece->nextShape = current;
        piece->nextShape->prevShape = piece;
    }
}

// Updates the doubly circular linked list of colors for a given piece that is added to the board
void updateColors(Piece* piece) {
    Piece* current = piece->next;
    while (current->color != piece->color) {
        current = current->next;
    }
    if (current == piece) {
        piece->prevColor = piece;
        piece->nextColor = piece;
    } else {
        piece->prevColor = current->prevColor;
        piece->prevColor->nextColor = piece;
        piece->nextColor = current;
        piece->nextColor->prevColor = piece;
    }
}

// Décale toutes les pièces d'une couleur spécifiée vers la gauche
void shiftByColor(Game* game, Color color) {
    // Le premier morceau de la couleur à partir de la gauche du plateau
    Piece *firstColor = game->head;
    while (firstColor->next != game->head) {
        if (firstColor->color == color) {
            break;
        }
        firstColor = firstColor->next;
    }
    if (firstColor->color != color) {
        cout << "Aucun morceau de cette couleur n'est présent sur le plateau !" << endl;
        return;
    }
    Piece *lastColor = firstColor->prevColor;

    // S'il y a au moins 2 morceaux de la même couleur, nous les décalons
    if (firstColor != lastColor) {
        // Décalage circulaire des couleurs en échangeant les formes et displayStr par paires
        Piece *current = firstColor;
        while (current != lastColor) {
            int tmp = static_cast<int>(current->shape);
            current->shape = current->nextColor->shape;
            current->nextColor->shape = static_cast<Shape>(tmp);

            string tmp2 = current->displayStr;
            current->displayStr = current->nextColor->displayStr;
            current->nextColor->displayStr = tmp2;

            current = current->nextColor;
        }

        // Mise à jour de la liste doublement chaînée circulaire des formes
        Piece *heads[4] = {nullptr};
        Piece *tails[4] = {nullptr};

        current = game->head;
        do {
            int i = static_cast<int>(current->shape);
            if(heads[i] == nullptr) {
                heads[i] = current;
                tails[i] = current;
            } else {
                tails[i]->nextShape = current;
                current->prevShape = tails[i];
                tails[i] = current;
            }
            current = current->next;
        } while (current != game->head);

        // Nous relions toutes les queues à toutes les têtes de la même forme
        for(int i = 0 ; i < game->shapeCount ; i++) {
            if(heads[i] != nullptr) {
                tails[i]->nextShape = heads[i];
                heads[i]->prevShape = tails[i];
            }
        }
    }
}

// Décale toutes les pièces d'une forme spécifiée vers la gauche
void shiftByShape(Game* game, Shape shape) {
    // Le premier morceau de la forme à partir de la gauche du plateau
    Piece *firstShape = game->head;
    while (firstShape->next != game->head) {
        if (firstShape->shape == shape) {
            break;
        }
        firstShape = firstShape->next;
    }
    if (firstShape->shape != shape) {
        cout << "Aucun morceau de cette forme n'est présent sur le plateau !" << endl;
        return;
    }
    Piece *lastShape = firstShape->prevShape;

    // S'il y a au moins 2 morceaux de la même forme, nous les décalons
    if (firstShape != lastShape) {
        // Décalage circulaire des formes en échangeant les couleurs et displayStr par paires
        Piece *current = firstShape;
        while (current != lastShape) {
            int tmp = static_cast<int>(current->color);
            current->color = current->nextShape->color;
            current->nextShape->color = static_cast<Color>(tmp);

            string tmp2 = current->displayStr;
            current->displayStr = current->nextShape->displayStr;
            current->nextShape->displayStr = tmp2;

            current = current->nextShape;
        }

        // Mise à jour de la liste doublement chaînée circulaire des couleurs
        Piece *heads[4] = {nullptr};
        Piece *tails[4] = {nullptr};

        current = game->head;
        do {
            int i = static_cast<int>(current->color);
            if(heads[i] == nullptr) {
                heads[i] = current;
                tails[i] = current;
            } else {
                tails[i]->nextColor = current;
                current->prevColor = tails[i];
                tails[i] = current;
            }
            current = current->next;
        } while (current != game->head);

        // Nous relions toutes les queues à toutes les têtes de la même couleur
        for(int i = 0 ; i < game->colorCount ; i++) {
            if(heads[i] != nullptr) {
                tails[i]->nextColor = heads[i];
                heads[i]->prevColor = tails[i];
            }
        }
    }
}

// Renvoie la taille de la séquence la plus longue de forme ou de couleur à partir de la pièce
int getPieceSequenceSize(Game *game, Piece *piece) {
    int colorSequence = 1, shapeSequence = 1;
    Piece *current = piece->next;
    // Il a été décidé que la longueur maximale d'une séquence est de 5
    for (int i = 1; i < 5; i++) {
        if (current == game->head)
            break;
        if (colorSequence == i && current->color == piece->color) colorSequence++;
        if (shapeSequence == i && current->shape == piece->shape) shapeSequence++;
        current = current->next;
    }
    if (colorSequence > shapeSequence) return colorSequence;
    return shapeSequence;
}

/* Vérifie si certaines pièces doivent être supprimées (3 pièces ou plus de la même couleur/forme)
 * Retourne : 0 si rien n'a changé, -1 si le jeu est gagné, le score ajouté sinon
*/
int updateBoard(Game *game, int isByShift) {
    int initialScore = game->score;
    // Pas besoin de vérifier s'il y a moins de 3 pièces
    if (game->piecesCount < 3) {
        return 0;
    }
    Piece *currentPiece = game->head;
    Piece *beforeCurrent = nullptr;
    int combinationSize;
    Piece *toDelete[5];
    Piece *tail = getTail(game);
    int combo = 0;
    while (currentPiece->next != game->head) {
        combinationSize = getPieceSequenceSize(game, currentPiece);
        // Si au moins 3 pièces de la même couleur ou forme sont présentes, les supprimer
        if (combinationSize >= 3) {
            // S'il y a des combos, le score augmentera de manière exponentielle (3^x ou 4^x etc)
            combo++;
            game->score += pow(combinationSize, combo) * (isByShift ? 2 : 1);
            // Si toutes les pièces du plateau sont supprimées, c'est une victoire, le jeu se terminera donc tout sera correctement libéré
            if (game->piecesCount == combinationSize) {
                return -1;
            }
            game->piecesCount -= combinationSize;
            // Mise à jour des liaisons pour les formes et les couleurs
            for (int i = 0; i < combinationSize; i++) {
                currentPiece->prevShape->nextShape = currentPiece->nextShape;
                currentPiece->nextShape->prevShape = currentPiece->prevShape;
                currentPiece->prevColor->nextColor = currentPiece->nextColor;
                currentPiece->nextColor->prevColor = currentPiece->prevColor;
                toDelete[i] = currentPiece;
                currentPiece = currentPiece->next;
            }
            // Suppression des pièces du plateau
            for (int i = 0; i < combinationSize; i++) {
                freePiece(toDelete[i]);
            }
            // Suppression des pièces de la liste chaînée simple
            if (beforeCurrent == nullptr) { // Dans ce cas, la tête est supprimée
                game->head = currentPiece;
                tail->next = game->head;
            } else {
                beforeCurrent->next = currentPiece;
                if (currentPiece == game->head) tail = beforeCurrent; // Vérification si la queue a été supprimée
            }
            // Nous devons revérifier depuis le début pour voir si une nouvelle combinaison a été créée à partir de cette suppression
            beforeCurrent = nullptr;
            currentPiece = game->head;
        } else {
            beforeCurrent = currentPiece;
            currentPiece = currentPiece->next;
        }
    }
    return game->score - initialScore;
}


void saveGame(Game *game, Piece **nextPieces, string name){
    ofstream file(name + ".txt");
    
    if (!file.is_open()) {
        cout << "Impossible d'ouvrir le fichier de sauvegarde !" << endl;
        return;
    }

    file << "TETRISTE" << endl;
    file << game->colorCount << " " << game->shapeCount << endl;
    file << game->score << " " << game->piecesCount << endl;

    for (int i = 0; i < 5; i++) {
        file << static_cast<int>(nextPieces[i]->color) << " " << static_cast<int>(nextPieces[i]->shape) << endl;
    }

    Piece* current = game->head;
    for (int i = 0; i < game->piecesCount; i++) {
        file << static_cast<int>(current->color) << " " << static_cast<int>(current->shape) << endl;
        current = current->next;
    }

    file.close();
}

// Renvoie le jeu s'il a été chargé avec succès, NULL sinon (fichier inexistant ou corrompu/invalidé)
Game* loadGame(Piece** nextPieces, string name) {
    string magic;
    ifstream file(name + ".txt");
    if (!file.is_open()) {
        cout << "Cette sauvegarde n'existe pas !" << endl;
        return nullptr;
    }

    getline(file, magic);
    
    if (magic != "TETRISTE") {
        cout << "Cette sauvegarde est invalide ou corrompue !" << endl;
        file.close();
        return nullptr;
    }
    

    Game* newGame = new Game();
    file >> newGame->colorCount >> newGame->shapeCount;
    file >> newGame->score >> newGame->piecesCount;
    
    for (int i = 0; i < 5; i++) {
        int color, shape;
        file >> color >> shape;
        nextPieces[i] = new Piece();
        nextPieces[i]->color = static_cast<Color>(color);
        nextPieces[i]->shape = static_cast<Shape>(shape);
        nextPieces[i]->displayStr = getDisplayStr(static_cast<Color>(color), static_cast<Shape>(shape));
    }
    
    Piece* current = nullptr;
    for (int i = 0; i < newGame->piecesCount; i++) {
        int color, shape;
        file >> color >> shape;
        Piece* newPiece = new Piece();
        newPiece->color = static_cast<Color>(color);
        newPiece->shape = static_cast<Shape>(shape);
        newPiece->displayStr = getDisplayStr(static_cast<Color>(color), static_cast<Shape>(shape));
        // Piece* newPiece = generatePiece(color, shape);
        // newPiece->displayStr = getDisplayStr(static_cast<Color>(color), static_cast<Shape>(shape));
        if (i == 0) {
            newGame->head = newPiece;
            newPiece->next = newPiece;
            newPiece->prevShape = newPiece;
            newPiece->nextShape = newPiece;
            newPiece->prevColor = newPiece;
            newPiece->nextColor = newPiece;
            current = newPiece;
        } else {
            rightInsert(newGame, newPiece);
            newGame->piecesCount--; // Parce que rightInsert l'incrémente
            current = newPiece;
        }
    }

    current->next = newGame->head;
    file.close();
    return newGame;
}


void updateRankings(int score, string name) {
    string rankingFile = "Ranking.txt";
    ifstream f(rankingFile);
    ofstream tempFile("temp.txt");

    if (!f.is_open() || !tempFile.is_open()) {
        cout << "Erreur : impossible de créer un fichier de classement." << endl;
        return;
    }

    string magic;
    getline(f, magic);

    if (magic != "TETRISTE") {
        cout << "Le fichier de classement est invalide ou corrompu." << endl;
        f.close();
        tempFile.close();
        return;
    }

    int scoreCount, scoreCopy;
    string nameCopy;
    vector<int> scores;
    vector<string> names;
    bool alreadyRanked = false, willBeReplaced = false;

    f >> scoreCount;
    for (int i = 0; i < scoreCount; i++) {
        f >> scoreCopy >> nameCopy;

        if (name == nameCopy) {
            alreadyRanked = 1;
            if (score > scoreCopy) willBeReplaced = 1;
        }

        scores.push_back(scoreCopy);
        names.push_back(nameCopy);
    }
    f.close();

    tempFile << "TETRISTE" << endl;
    int numberOfScores = (scoreCount < 10) ? scoreCount + 1 - alreadyRanked : 10;
    tempFile << numberOfScores << endl;

    int counter = 0;
    int position = 0;
    while (counter < scoreCount && scores[position] >= score) {
        tempFile << scores[position] << " " << names[position] << endl;
        counter++;
        position++;
    }

    if (counter < numberOfScores && (!alreadyRanked || willBeReplaced)) {
        tempFile << score << " " << name << endl;
        counter++;
    }

    while (counter < numberOfScores) {
        if (names[position] == name) {
            position++;
            continue;
        }
        tempFile << scores[position] << " " << names[position] << endl;
        counter++;
        position++;
    }

    tempFile.close();
    if (remove(rankingFile.c_str()) != 0) {
        cout << "Erreur : impossible de supprimer l'ancien fichier de classement." << endl;
        return;
    }
    if (rename("temp.txt", rankingFile.c_str()) != 0) {
        cout << "Erreur : impossible de renommer le fichier temporaire." << endl;
    }
}

// Free the memory allocated to a piece
void freePiece(Piece* piece) {
    delete(piece);
}

// Free the memory allocated to all the pieces of a game, and the game itself
void freeGame(Game* game) {
    Piece* current = game->head;
    for (int i = 0; i < game->piecesCount; i++) {
        Piece* next = current->next;
        freePiece(current);
        current = next;
    }
    free(game);
}