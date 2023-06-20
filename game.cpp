#include "game.h"
#include "util.h"
#include "sound.h"
#include <ncurses.h>

//Text-und Zeitanzeige
int textX = 5;
int textY = 0;
int level = 1;

//Counter für laufende Spielzeit
int timeCount = 1;
//Counter-Zeit für nächstes Hinderniss/Level
int lvl2 = 40;

// Spielfeldgröße
const int FIELD_WIDTH = 20;
const int FIELD_HEIGHT = 10;

// Spielerposition
int playerX = 1;
int playerY = FIELD_HEIGHT - 2;

// Sprungstatus
bool isJumping = false;
int jumpHeight = 4;
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


    if (timeCount > lvl2){
        mvprintw(obstacleY, obstacleX, "HH");
    } else {
    // Zeichne das Hindernis
    mvprintw(obstacleY, obstacleX, "H");
    }

    // Zeichne den Boden
    for (int x = 0; x < FIELD_WIDTH; ++x) {
        mvprintw(FIELD_HEIGHT - 1, x, "=");
    }

    //Levelanzeige
    if (timeCount > lvl2){
        mvprintw(textY, textX, " Level 2");
    } else {
        mvprintw(textY, textX, " Level 1");
    }
    
    refresh();
}

// Funktion zum Springen des Spielers
void jump() {
    // Ursprünglich: "Überprüfe, ob der Spieler bereits springt oder ob er Boden erreicht hat:" // if (isJumping|| playerY >= FIELD_HEIGHT - 2) { return; }

    // Starte den Sprung
    if (!isJumping) {
        isJumping = true;
        jumpCount = 0;
        sound_play("roblox_jump.wav");
    }
}

void gameOverArt() {
    // Set initial coordinates
    int x = 0;
    int y = 0;

    sound_play("game_over_sound.wav");
    
    // Loop for moving the ASCII art
    for (int i = 0; i < 8; ++i) {
        clear();  // Clear the screen

        // Print ASCII art using mvprintw
        mvprintw(y, x,       "---------------------");
        mvprintw(y + 1, x, "     GAME OVER     ");
        mvprintw(y + 2, x, "---------------------");

        
        refresh();  // Refresh the screen to display the output

        // Update coordinates for next iteration
        x += 1;
        y += 1;

        // Delay for a short period to control the speed
        napms(90);
    }
    msleep(1000);
}

// Funktion zur Aktualisierung des Spiels
void update() {
    // Bewege das Hindernis nach links
    obstacleX--;
    //Vergangene Zeit
    timeCount++;
    

    // Überprüfe, ob das Hindernis den Spieler erreicht hat
    if (obstacleX == playerX && obstacleY == playerY) {
        gameOverArt();
        gameOver = true;
        return;
    }
    //Überprüfe größeres Hinderniss nach Zeit
    if (timeCount > lvl2){
    if (obstacleX + 1 == playerX && obstacleY == playerY) {
        gameOverArt();
        gameOver = true;
        return;
    }
    }

    // Überprüfe, ob das Hindernis den linken Rand erreicht hat
    if (obstacleX <= 0) {
        obstacleX = FIELD_WIDTH - 3;
    }

    //Wenn Spieler springt aktualisiere Sprunghöhe
    if (isJumping) {
        jumpCount++;
        playerY--;

        // Überprüfe, ob die maximale Sprunghöhe erreicht wurde
        if (jumpCount >= jumpHeight) {
            isJumping = false; // Beende den Sprung
            jumpCount = 0; // Setze den Sprungzähler zurück
        }
    } else {
        // Wenn der Spieler nicht springt, fällt er nach unten
        if (playerY < FIELD_HEIGHT - 2) {
            playerY++;
        }
    }
    
    // Begrenze die Spielerposition nach unten
    if (playerY >= FIELD_HEIGHT - 2) {
        playerY = FIELD_HEIGHT - 2;
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
            case ' ':
                jump();
                break;
            case 'q':
                gameOverArt();
                gameOver = true;
                break;
        }

        update();
    }

    // Beende NCurses
    endwin();
}
