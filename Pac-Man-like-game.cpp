#include <iostream>
#include <conio.h>
#include <windows.h>
#include <cctype>
#include <cstdlib>
#include <ctime>

using namespace std;

const int WIDTH = 22; // Board width
const int HEIGHT = 11; // Board height
const int NUM_GHOSTS = 2; // Number of ghosts
// Board 
char board[HEIGHT][WIDTH] = {
    "#####################",
    "#..........#........#",
    "#.###...#.###.#.###.#",
    "#.#...#.....#...#...#",
    "#.###.#...###.#.###.#",
    "#...................#",
    "#.###.#.###...#.###.#",
    "#...#...#...#.......#",
    "#.###.#.###.#...###.#",
    "#.......#...........#",
    "#####################"
};
// Initial locations of Pac-Man and ghosts
int pacmanX = 1, pacmanY = 1;
int ghostX[NUM_GHOSTS] = { 10, 15 };
int ghostY[NUM_GHOSTS] = { 5, 5 };
int score = 0; // Initial score
bool gameOver = false;

void drawBoard() {
    system("cls");
    cout << "PAC-MAN (WASD to move, Q to quit)" << endl;
    cout << "Score: " << score << endl << endl;

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH - 1; x++) {
            bool printed = false;

            if (x == pacmanX && y == pacmanY) {
                cout << 'P'; // Represents Pac-Man
                printed = true;
            }
            else {
                for (int i = 0; i < NUM_GHOSTS; i++) {
                    if (x == ghostX[i] && y == ghostY[i]) {
                        cout << 'G'; // Represents ghosts
                        printed = true;
                        break;
                    }
                }
                if (!printed) {
                    cout << board[y][x];
                }
            }
        }
        cout << endl;
    }
}

void movePacman(int dx, int dy) {
    int newX = pacmanX + dx;
    int newY = pacmanY + dy;

    if (board[newY][newX] != '#') {
        pacmanX = newX;
        pacmanY = newY;

        if (board[newY][newX] == '.') {
            score += 10;
            board[newY][newX] = ' ';
        }
    }
}
// Moving the ghosts
void moveGhosts() {
    for (int i = 0; i < NUM_GHOSTS; i++) {
        int dx = 0, dy = 0;
        int tries = 0;
        do {
            int dir = rand() % 4;
            dx = (dir == 0) ? -1 : (dir == 1) ? 1 : 0;
            dy = (dir == 2) ? -1 : (dir == 3) ? 1 : 0;

            int newX = ghostX[i] + dx;
            int newY = ghostY[i] + dy;

            // Checking for walls ("#")
            if (board[newY][newX] == '#') {
                continue;
            }

            // Checking for other ghosts
            bool occupied = false;
            for (int j = 0; j < NUM_GHOSTS; j++) {
                if (j != i && newX == ghostX[j] && newY == ghostY[j]) {
                    occupied = true;
                    break;
                }
            }
            if (!occupied) {
                ghostX[i] = newX;
                ghostY[i] = newY;
                break;
            }

            tries++;
        } while (tries < 10);
    }
}
// Checking for lose
void checkCollision() {
    for (int i = 0; i < NUM_GHOSTS; i++) {
        if (pacmanX == ghostX[i] && pacmanY == ghostY[i]) {
            gameOver = true;
            system("cls");
            cout << "YOU LOSE! Final score: " << score << endl;
        }
    }
}
// Checking for win
void checkWin() {
    bool dotsRemaining = false;
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (board[y][x] == '.') {
                dotsRemaining = true;
                break;
            }
        }
        if (dotsRemaining) break;
    }

    if (!dotsRemaining) {
        gameOver = true;
        system("cls");
        cout << "YOU WIN! Final score: " << score << endl;
    }
}

int main() {
    srand(time(0));

    cout << "PAC-MAN (WASD to move, Q to quit)" << endl;
    cout << "Press any key to start...";
    _getch();

    while (!gameOver) {
        drawBoard();

        // Controling the Pac-Man using w, a, s, d keys
        if (_kbhit()) {
            char input = _getch();
            input = tolower(input);

            switch (input) {
            case 'w': movePacman(0, -1); break;
            case 's': movePacman(0, 1);  break;
            case 'a': movePacman(-1, 0); break;
            case 'd': movePacman(1, 0);  break;
            case 'q': gameOver = true; break;
            }

            checkWin();
        }

        moveGhosts();
        checkCollision();

        Sleep(200);
    }

    return 0;
}
