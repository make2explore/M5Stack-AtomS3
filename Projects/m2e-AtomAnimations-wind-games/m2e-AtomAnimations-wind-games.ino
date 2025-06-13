// ---------------------------------- make2explore.com -------------------------------------------------------//
// Project           - Windows XP - Card Games on M5Stack AtomS3
// Created By        - info@make2explore.com
// Last Modified     - 13/06/2025 17:36:00 @admin
// Software          - C/C++, Arduino IDE, Libraries
// Hardware          - M5Stack AtomS3 Development board     
// Sensors Used      - NA
// Source Repo       - github.com/make2explore
// ===========================================================================================================//

// Include Necessary Libraries and Animation Byte Arrays
#include "M5AtomS3.h"
#include "wins.h"
#include "wind.h"
#include "gm.h"
#include "game.h"

// === Animation Variables ===
int frameIndex = 0;
int ani = 0;
bool gameMode = false;

// Animation data
int aniDelay[3] = {120, 120, 120};       // delays for winf, wind, gm
int aniFrames[3] = {26, 7, 35};          // frame counts for winf, wind, gm

// === Card Game Variables ===
int n = 2;
int cards[5];
int positions[4][5] = {
    {2, 64, 0, 0, 0},
    {2, 42, 82, 0, 0},
    {2, 32, 62, 92, 0},
    {2, 22, 42, 62, 82}
};

void drawGame() {
    AtomS3.Display.fillScreen(BLACK);
    AtomS3.Display.drawString("New Game!", 3, 2, 2);
    AtomS3.Display.drawString("HIT ME", 42, 110, 2);
    for (int i = 0; i < n; i++) {
        AtomS3.Display.pushImage(positions[n - 2][i], 22, 60, 84, card[cards[i]]);
    }
}

void resetGame() {
    n = 2;
    for (int i = 0; i < n; i++) {
        cards[i] = random(0, 52);
    }
    drawGame();
}

void setup() {
    auto cfg = M5.config();
    AtomS3.begin(cfg);
    AtomS3.Display.setSwapBytes(1);
    AtomS3.Display.setBrightness(120);

    // Init random cards
    for (int i = 0; i < n; i++) {
        cards[i] = random(0, 52);
    }
}

void loop() {
    AtomS3.update();

    // === GAME MODE ===
    if (gameMode) {
        if (AtomS3.BtnA.wasPressed() && n < 5) {
            n++;
            cards[n] = random(0, 52);
            drawGame();
        }

        // Long press resets the game
        if (AtomS3.BtnA.pressedFor(1000)) {
            resetGame();
        }

        return;
    }

    // === Animation Drawing ===
    switch (ani) {
        case 0:
            AtomS3.Display.pushImage(0, 0, 128, 128, wins[frameIndex]);
            break;
        case 1:
            AtomS3.Display.pushImage(0, 0, 128, 128, wind[frameIndex]);
            break;
        case 2:
            AtomS3.Display.pushImage(0, 0, 128, 128, gm[frameIndex]);
            break;
    }

    frameIndex++;

    if (frameIndex == aniFrames[ani]) {
        frameIndex = 0;

        if (ani == 0) {
            ani = 1;  // After wins, play wind in loop
        } else if (ani == 2) {
            gameMode = true;  // After gm, enter game
            resetGame();
            return;
        }
    }

    delay(aniDelay[ani]);

    // Button triggers gm animation when wind is playing
    if (ani == 1 && AtomS3.BtnA.wasPressed()) {
        ani = 2;
        frameIndex = 0;
    }
}

// ---------------------------------- make2explore.com----------------------------------------------------//