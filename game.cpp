#include "game.h"
#include "util.h"
#include <ncurses.h>

// Spielfeldgröße
const int FIELD_WIDTH = 20;
const int FIELD_HEIGHT = 10;

// Spielerposition
int playerX = 1;
int playerY = FIELD_HEIGHT - 2;

// Sprungstatus
bool isJumping = false;
int jumpHeight = 3;
int jumpCount = 0;

// Hindernisposition
int obstacleX = FIELD_WIDTH - 3;
int obstacleY = FIELD_HEIGHT - 2;

// Spielstatus
bool gameOver = false;

// Funktion zum Zeichnen des Spielfelds
void drawField() {
    clear();

    // Zeichne den Spieler
    mvprintw(playerY, playerX, "P");

    // Zeichne das Hindernis
    mvprintw(obstacleY, obstacleX, "H");

    // Zeichne den Boden
    for (int x = 0; x < FIELD_WIDTH; ++x) {
        mvprintw(FIELD_HEIGHT - 1, x, "=");
    }

    refresh();
}

// Funktion zum Bewegen des Spielers
void movePlayer(int dx, int dy) {
    // Überprüfe Kollisionen mit Hindernis
    if (playerX + dx == obstacleX && playerY + dy == obstacleY) {
        gameOver = true;
        return;
    }

    // Bewege den Spieler
    playerX += dx;
    playerY += dy;

    // Überprüfe, ob der Spieler den Boden erreicht hat
    if (playerY >= FIELD_HEIGHT - 2) {
        isJumping = false;
        jumpCount = 0;
        playerY = FIELD_HEIGHT - 2;
    }
}

// Funktion zum Springen des Spielers
void jump() {
    // Überprüfe, ob der Spieler bereits springt oder den Boden erreicht hat
    if (isJumping || playerY >= FIELD_HEIGHT - 2) {
        return;
    }

    // Starte den Sprung
    isJumping = true;
}

// Funktion zur Aktualisierung des Spiels
void update() {
    // Bewege das Hindernis nach links
    obstacleX--;

    // Überprüfe, ob das Hindernis den Spieler erreicht hat
    if (obstacleX == playerX && obstacleY == playerY) {
        gameOver = true;
        return;
    }

    // Überprüfe, ob das Hindernis den linken Rand erreicht hat
    if (obstacleX <= 0) {
        obstacleX = FIELD_WIDTH - 3;
    }

    // Wenn der Spieler gerade springt, aktualisiere die Sprunghöhe
    if (isJumping) {
        jumpCount++;
        if (jumpCount >= jumpHeight) {
            isJumping = false;
            jumpCount = 0;
        }
    }
}

void showSplashScreen() {
    clear();

    // Zeichne den Splash-Screen
    mvprintw(FIELD_HEIGHT / 2, (FIELD_WIDTH - 9) / 2, "Jump n Run");

    refresh();
    msleep(2000); // Warte 2 Sekunden

    clear();
}

void gameLoop() {
    // Initialisiere NCurses
    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();
    curs_set(0);
    timeout(100);

    // Zeige den Splash-Screen
    showSplashScreen();

    // Hauptspielschleife
    while (!gameOver) {
        drawField();

        int input = getch();
        switch (input) {
            case KEY_UP:
                movePlayer(0, -1);
                break;
            case ' ':
                jump();
                break;
            case 'q':
                gameOver = true;
                break;
        }

        update();
    }

    // Beende NCurses
    endwin();
}
