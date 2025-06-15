// ---------------------------------- make2explore.com -------------------------------------------------------//
// Project           - Displaying Animations on M5Stack AtomS3
// Created By        - info@make2explore.com
// Last Modified     - 13/06/2025 17:36:00 @admin
// Software          - C/C++, Arduino IDE, Libraries
// Hardware          - M5Stack AtomS3 Development board     
// Sensors Used      - NA
// Source Repo       - github.com/make2explore
// ===========================================================================================================//

// Include Necessary Libraries and Animation Byte Arrays
#include "M5AtomS3.h"
#include "m2e.h"
#include "matrix.h"
#include "walle.h"
#include "thank.h"

// Variavles for frames
int n = 0;
int ani = 0;

int aniDelay[4] = {120, 120, 120, 120};
int aniFrames[4] = {18, 14, 14, 11};

// Init and Setup AtomS3
void setup() {
    auto cfg = M5.config();
    AtomS3.begin(cfg);
    AtomS3.Display.setSwapBytes(1);
    AtomS3.Display.setBrightness(120);
}

void loop() {
    AtomS3.update();

    if (AtomS3.BtnA.wasPressed()) {
        ani = (ani + 1) % 4;  // Cycle between 0 to 3
        n = 0;                // Reset frame index when animation changes
    }

    switch (ani) {
        case 0:
            AtomS3.Display.pushImage(0, 0, 128, 128, m2e[n]);
            break;
        case 1:
            AtomS3.Display.pushImage(0, 0, 128, 128, matrix[n]);
            break;
        case 2:
            AtomS3.Display.pushImage(0, 0, 128, 128, walle[n]);
            break;
        case 3:
            AtomS3.Display.pushImage(0, 0, 128, 128, thank[n]);
            break;
    }

    n++;
    if (n == aniFrames[ani]) n = 0;

    delay(aniDelay[ani]);
}

// ---------------------------------- make2explore.com----------------------------------------------------//