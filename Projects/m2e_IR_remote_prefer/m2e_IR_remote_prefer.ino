// ---------------------------------- make2explore.com -------------------------------------------------------//
// Project           - Tiny Interactive IR Remote using M5Stack AtomS3
// Created By        - info@make2explore.com
// Last Modified     - 13/06/2025 17:36:00 @admin
// Software          - C/C++, Arduino IDE, Libraries
// Hardware          - M5Stack AtomS3 Development board, IR Receiver Relay Board -1CH     
// Sensors Used      - TSOP1738 IR Receiver
// Source Repo       - github.com/make2explore
// ===========================================================================================================//

// Preprocessor Directives for some settings and Pin Def
#define DISABLE_CODE_FOR_RECEIVER
#define SEND_PWM_BY_TIMER
#define ATOMS3_IR_TX_PIN 4

// Include Necessary Libraries
#include "M5AtomS3.h"
#include <IRremote.hpp>

bool isOn = false;
bool isDisplayOn = true;
bool isFirstClick = true;

unsigned long lastInteractionTime = 0;
const unsigned long INACTIVITY_TIMEOUT_MS = 60000;  // 60 seconds

void resetInactivityTimer() {
    lastInteractionTime = millis();
}

void drawText(const char* text, uint32_t bgColor, uint32_t textColor) {
    if (!isDisplayOn) {
        AtomS3.Display.setBrightness(255);
        isDisplayOn = true;
    }

    AtomS3.Display.fillScreen(bgColor);
    AtomS3.Display.setTextColor(textColor);
    AtomS3.Display.setTextDatum(middle_center);
    AtomS3.Display.setFont(&fonts::Orbitron_Light_24);
    AtomS3.Display.setTextSize(1);
    AtomS3.Display.drawString(text, AtomS3.Display.width() / 2,
                              AtomS3.Display.height() / 2);
    resetInactivityTimer();  // activity
}

void showReadyScreen() {
    if (!isDisplayOn) {
        AtomS3.Display.setBrightness(255);
        isDisplayOn = true;
    }

    AtomS3.Display.fillScreen(BLACK);
    AtomS3.Display.setTextColor(GREEN);
    AtomS3.Display.setFont(&fonts::Orbitron_Light_24);
    AtomS3.Display.setTextDatum(middle_center);
    AtomS3.Display.setTextSize(1);

    int screenWidth = AtomS3.Display.width();
    int screenHeight = AtomS3.Display.height();
    int wordHeight = 28;
    int spacing = 6;
    int totalTextHeight = wordHeight * 3 + spacing * 2;
    int startY = (screenHeight - totalTextHeight) / 2 + wordHeight / 2;

    AtomS3.Display.drawString("Click", screenWidth / 2, startY + (wordHeight + spacing) * 0);
    AtomS3.Display.drawString("To",    screenWidth / 2, startY + (wordHeight + spacing) * 1);
    AtomS3.Display.drawString("Start", screenWidth / 2, startY + (wordHeight + spacing) * 2);

    resetInactivityTimer();  // activity
}

void showOnScreen() {
    drawText("ON", 0x00FF00, 0x000000);
}

void showOffScreen() {
    drawText("OFF", 0xFF0000, 0xFFFFFF);
}

void setup() {
    Serial.begin(115200);
    auto cfg = M5.config();
    AtomS3.begin(cfg);
    delay(100);

    AtomS3.Display.setBrightness(255);
    IrSender.begin(DISABLE_LED_FEEDBACK);
    IrSender.setSendPin(ATOMS3_IR_TX_PIN);

    showReadyScreen();
    Serial.println("System Ready: Waiting for Click to ON");
    isOn = false;
    resetInactivityTimer();
}

void loop() {
    AtomS3.update();
    unsigned long now = millis();

    // Sleep on inactivity
    if (isDisplayOn && (now - lastInteractionTime > INACTIVITY_TIMEOUT_MS)) {
        Serial.println("Inactivity timeout → Display OFF");
        AtomS3.Display.setBrightness(0);
        isDisplayOn = false;
    }

    // Long press to manually turn off display
    if (AtomS3.BtnA.pressedFor(1000)) {
        if (isDisplayOn) {
            Serial.println("Long press → Display OFF");
            AtomS3.Display.setBrightness(0);
            isDisplayOn = false;
        }
        delay(1000);
        return;
    }

    // Button pressed
    if (AtomS3.BtnA.wasPressed()) {
        resetInactivityTimer();

        if (!isDisplayOn) {
            Serial.println("Display was OFF → Waking up");
            AtomS3.Display.setBrightness(255);
            isDisplayOn = true;
            // Optionally redraw the last state
            if (isFirstClick) {
                showReadyScreen();
            } else if (isOn) {
                showOnScreen();
            } else {
                showOffScreen();
            }
            return;
        }

        if (isFirstClick) {
            isFirstClick = false;
            isOn = true;
            Serial.println("First click → Sending ON");
            IrSender.sendNEC(0x00, 0x40, 0);
            showOnScreen();
            return;
        }

        isOn = !isOn;
        if (isOn) {
            Serial.println("Toggled → ON");
            IrSender.sendNEC(0x00, 0x40, 0);
            showOnScreen();
        } else {
            Serial.println("Toggled → OFF");
            IrSender.sendNEC(0x00, 0x19, 0);
            showOffScreen();
        }
    }
}
// ---------------------------------- make2explore.com----------------------------------------------------//
