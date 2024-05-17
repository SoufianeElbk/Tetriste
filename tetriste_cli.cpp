#include <iostream>
#include <conio.h>
#include <windows.h>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include "tetriste.h"
#include "tetriste_cli.h"

using namespace std;

void startCLI() {
    //Initialisation du générateur de nombres aléatoires
    srand(time(NULL));
    Game *currentGame;

    int continueGame = 1;
    Piece **nextPieces = new Piece *[5];

    int keepNextPieces; // Est utilisé pour savoir si nous devons générer une nouvelle pièce (uniquement si nous avons effectué une insertion lors de l'itération précédente)
    int scoreAdded = 0; // Est utilisé pour suivre le score ajouté par la dernière action et l'afficher lors de la prochaine itération de la boucle

    cout << "Voulez-vous charger une partie (o/n) ? ";
    char loadChoice;
    loadChoice = getch();
    cout << endl;

    if(loadChoice == 'o') {
        string name;
        cout << "Nom du fichier de sauvegarde : ";
        cin >> name;
        currentGame = loadGame(nextPieces, name);
        if(currentGame == nullptr) {
            cout << "Impossible de charger la partie. Sortie..." << endl;
            delete[] nextPieces;
            return;
        }
    } 
    else {
        cout << "\033[0m\033[0;0H\033[2J";
        cout << "Choose the difficulty of the game:\n";
        cout << "1. Easy (2 colors, 2 shapes)\n";
        cout << "2. Medium (3 colors, 3 shapes)\n";
        cout << "3. Hard (4 colors, 4 shapes)\n";
        cout << "4. Custom\n" ;
        cout << "Your choice: " ;
        int difficulty, colorCount, shapeCount;
        cin >> difficulty;
        cout << "\n";

        switch (difficulty) {
            case 1:
                colorCount = 2;
                shapeCount = 2;
                break;
            case 2:
                colorCount = 3;
                shapeCount = 3;
                break;
            case 3:
                colorCount = 4;
                shapeCount = 4;
                break;
            case 4:
                do {
                    cout << "Number of colors (2-4): ";
                    cin >> colorCount;
                    cout << "Number of shapes (2-4): ";
                    cin >> shapeCount;
                    cout << "\n";
                } while (colorCount < 2 || colorCount > 4 || shapeCount < 2 || shapeCount > 4);
                break;
            default:
                break;
        }
        currentGame = initGame(colorCount, shapeCount);

        for (int i = 0; i < 5; i++) {
            nextPieces[i] = generatePiece(currentGame->colorCount, currentGame->shapeCount);
        }
    }
    while (continueGame) {

        // Par défaut, nous gardons nextPieces tel quel pour l'exécution suivante de la boucle (décalage par couleur/forme, quitter ou action incorrecte)
        keepNextPieces = 1;

        cout << "\033[0m\033[0;0H\033[2J";
        displayGameInfo(currentGame, nextPieces, scoreAdded);
        cout << "\n\n";
        displayMainMenu();
        char choice;
        choice = getch();
        cout << endl;
        int isByShift = 1; // Est utilisé pour savoir si la fonction updateBoard doit faire un multiplicateur *2 si l'action était un décalage

    switch (choice) {
        case 'j':
            leftInsert(currentGame, nextPieces[4]);
            isByShift = keepNextPieces = 0;
            break;
        case 'k':
            rightInsert(currentGame, nextPieces[4]);
            isByShift = keepNextPieces = 0;
            break;
        case 'b':
            shiftByColor(currentGame, Color::BLUE);
            break;
        case 'y':
            shiftByColor(currentGame, Color::YELLOW);
            break;
        case 'r':
            shiftByColor(currentGame, Color::RED);
            break;
        case 'g':
            shiftByColor(currentGame, Color::GREEN);
            break;
        case 's':
            shiftByShape(currentGame, Shape::SQUARE);
            break;
        case 'd':
            shiftByShape(currentGame, Shape::DIAMOND);
            break;
        case 'c':
            shiftByShape(currentGame, Shape::CIRCLE);
            break;
        case 't':
            shiftByShape(currentGame, Shape::TRIANGLE);
            break;
        case 'q':
            isByShift = continueGame = 0;

            cout << "Voulez-vous sauvegarder votre progression (o/n) ? ";
            char saveChoice;
            // cin >> saveChoice;
            saveChoice = getch();
            cout << endl;
            if (saveChoice == 'o') {
                string name;
                cout << "Nom de la sauvegarde : ";
                cin >> name;
                updateRankings(currentGame->score, name);
                saveGame(currentGame, nextPieces, name);
            }
            break;
        default:
            // Inutile de dire que le choix est invalide car l'exécution suivante de la boucle effacera le terminal
            break;
    }

        if (!keepNextPieces) {
            for (int i = 4; i > 0; i--) {
                nextPieces[i] = nextPieces[i - 1];
            }
            nextPieces[0] = generatePiece(currentGame->colorCount, currentGame->shapeCount);
        }

        scoreAdded = updateBoard(currentGame, isByShift);
        if (scoreAdded == -1) {
            cout << "\033[0m\033[0;0H\033[2J";
            cout << "------------------------------------------------------------------\n";
            cout << "VOUS AVEZ GAGNÉ !\nScore final : " << currentGame->score << endl;
            cout << "------------------------------------------------------------------\n";
            askUpdateRankings(currentGame);
            // saveGame(currentGame, nextPieces, name);
            continueGame = 0;
        }
        if (currentGame->piecesCount >= 15) {
            cout << "\033[0m\033[0;0H\033[2J";
            cout << "------------------------------------------------------------------\n";
            cout << "VOUS AVEZ PERDU !\nScore final : " << currentGame->score << endl;
            cout << "------------------------------------------------------------------\n";
            askUpdateRankings(currentGame);
            // saveGame(currentGame, nextPieces, name);
            continueGame = 0;
        }
        if (currentGame->piecesCount == 0) {
            cout << "\033[0m\033[0;0H\033[2J";
            cout << "------------------------------------------------------------------\n";
            cout << "VOUS AVEZ GAGNÉ !\nScore final : " << currentGame->score << endl;
            cout << "------------------------------------------------------------------\n";
            askUpdateRankings(currentGame);
            continueGame = 0;
        }
    }
    //Libération de la mémoire
    for (int i = 0; i < 5; i++) {
        delete nextPieces[i];
    }
    delete[] nextPieces;

    freeGame(currentGame);
}


void askUpdateRankings(Game* currentGame) {
    cout << "Voulez-vous mettre à jour les classements (o/n) ? ";
    char updateChoice;
    updateChoice = getch(); 
    cout << endl;
    if (updateChoice == 'o') {
        string name;
        cout << "Nom du joueur : ";
        cin >> name;
        updateRankings(currentGame->score, name);
    }
}

void displayGameInfo(Game *currentGame, Piece **nextPieces, int scoreAdded) {
    cout << "\t\t\t---------------------------------  TETRISTE  ---------------------------------\n";
    cout << endl;
    cout << endl;
    if(scoreAdded == 0) {
        cout << "\t\t\t\t\t\t\t\t\t\t\tScore actuel : " << currentGame->score << endl;
    } else if(scoreAdded <= 5){ // Insertions
        cout << "\t\t\t\t\t\t\t\t\t\t\tScore actuel : " << currentGame->score << " (+" << scoreAdded << ")\n";
    } else {                    // Combos et/ou décalages
        cout << "\t\t\t\t\t\t\t\t\t\t\tScore actuel : " << currentGame->score << " (Incroyable ! +" << scoreAdded << ")\n";
    }
    cout << endl;
    cout << endl;
    cout << "Prochaines pièces : ";

    for (int i = 0; i < 5; i++) {
        cout << nextPieces[i]->displayStr << "\t";
    }
    cout << "\033[31m<-- Élément suivant\033[0m\n";
        cout << endl;
        cout << endl;
    Piece *currentPiece = currentGame->head;
    cout << "-------------------------------------------------------------------------------------------------\n";
    for (int i = 0; i < currentGame->piecesCount; i++) {
        cout << "\t" << currentPiece->displayStr << " ";
        currentPiece = currentPiece->next;
    }
    cout << "\n-------------------------------------------------------------------------------------------------";

    cout << "\n\n";
}

void displayMainMenu() {
    cout << "\t\t\t\t\tj. Insérer à gauche\n";
    cout << "\t\t\t\t\tk. Insérer à droite\n";
    cout << "\t\t\t\t\tb. Décaler par couleur \033[34mbleu\033[0m\n";
    cout << "\t\t\t\t\ty. Décaler par couleur \033[33mjaune\033[0m\n";
    cout << "\t\t\t\t\tr. Décaler par couleur \033[31mrouge\033[0m\n";
    cout << "\t\t\t\t\tg. Décaler par couleur \033[32mvert\033[0m\n";
    cout << "\t\t\t\t\ts. Décaler par forme carré ■\n";
    cout << "\t\t\t\t\td. Décaler par forme losange ◊\n";
    cout << "\t\t\t\t\tc. Décaler par forme circle ●\n";
    cout << "\t\t\t\t\tt. Décaler par forme triangle ▲\n";
    cout << "\t\t\t\t\tq. Quitter\n\n";
}

