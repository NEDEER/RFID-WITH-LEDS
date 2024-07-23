#include <SPI.h>
#include <MFRC522.h>

// Define pins for the RFID reader
#define RST_PIN 9
#define SS_PIN 10

// Initialize the MFRC522 instance
MFRC522 mfrc522(SS_PIN, RST_PIN);

// Define LED pins
const int greenLED = 6;
const int redLED = 7;

// RFID card UIDs (Replace with your actual card UIDs)
byte blueCardUID[] = {0x43, 0x52, 0x64, 0xA8};  // Blue card UID
byte whiteCardUID[] = {0x13, 0x2A, 0x73, 0xA6}; // White card UID

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  
  // Initialize LED pins
  pinMode(greenLED, OUTPUT);
  pinMode(redLED, OUTPUT);
  
  Serial.println("Place your RFID card near the reader...");
}

void loop() {
  // Look for new cards
  if (mfrc522.PICC_IsNewCardPresent()) {
    // Select one of the cards
    if (mfrc522.PICC_ReadCardSerial()) {
      Serial.print("Card UID: ");
      for (byte i = 0; i < mfrc522.uid.size; i++) {
        Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
        Serial.print(mfrc522.uid.uidByte[i], HEX);
      }
      Serial.println();
      
      // Check if the card UID matches the blue card
      if (memcmp(mfrc522.uid.uidByte, blueCardUID, mfrc522.uid.size) == 0) {
        digitalWrite(greenLED, LOW);
        digitalWrite(redLED, HIGH);
        Serial.println("Blue Card Detected - Red LED ON");
        delay(2000); // Delay for 2 seconds
        digitalWrite(redLED, LOW);
      }
      // Check if the card UID matches the white card
      else if (memcmp(mfrc522.uid.uidByte, whiteCardUID, mfrc522.uid.size) == 0) {
        digitalWrite(greenLED, HIGH);
        digitalWrite(redLED, LOW);
        Serial.println("White Card Detected - Green LED ON");
        delay(2000); // Delay for 2 seconds
        digitalWrite(greenLED, LOW);
      }
      else {
        digitalWrite(greenLED, LOW);
        digitalWrite(redLED, LOW);
        Serial.println("Unknown Card Detected");
      }
      
      // Halt PICC
      mfrc522.PICC_HaltA();
    }
  }
}
