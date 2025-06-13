// ---------------------------------- make2explore.com -------------------------------------------------------//
// Project           - How to Check IR codes of your Remote
// Created By        - info@make2explore.com
// Last Modified     - 13/06/2025 17:36:00 @admin
// Software          - C/C++, Arduino IDE, Libraries
// Hardware          - Arduino Uno board, IR Remote    
// Sensors Used      - TSOP1738 IR Receiver
// Source Repo       - github.com/make2explore
// ===========================================================================================================//

// Include Necessary Libraries
#include <IRremote.hpp>

const int IR_RECEIVE_PIN = 9;

void setup() {
  Serial.begin(9600);
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);
}

void loop() {
  if (IrReceiver.decode()) {
    Serial.println("----------------------------------");
    Serial.print("Protocol: ");
    Serial.println(IrReceiver.decodedIRData.protocol);

    Serial.print("Address: 0x");
    Serial.println(IrReceiver.decodedIRData.address, HEX);

    Serial.print("Command: 0x");
    Serial.println(IrReceiver.decodedIRData.command, HEX);

    Serial.print("Raw 32-bit: 0x");
    Serial.println(IrReceiver.decodedIRData.decodedRawData, HEX);

    Serial.print("Decimal: ");
    Serial.println(IrReceiver.decodedIRData.decodedRawData);

    IrReceiver.resume();
  }
}
// ---------------------------------- make2explore.com----------------------------------------------------//