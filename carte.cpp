#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h> 
  
   const int ipadd[4] = {103, 97, 67, 25}; 
   
   int servoOpen = 180;
   int servoClose = 0; 
   int delayBeforeClosing = 1000; 
   
    #define SS_PIN 10 
    #define RST_PIN 9 
    
    const int servoPin = 6; 
    const int ledPin = 13; 
    Servo servo; 
    MFRC522 rfid(SS_PIN, RST_PIN);
    bool approved = 0, oldApproved = 0; 
   
    byte nuidPICC[4]; 
     void setup() {
        Serial.begin(9600); 
        SPI.begin(); 
        rfid.PCD_Init(); 
        pinMode(ledPin, OUTPUT); 
        servo.write(servoClose); 
        servo.attach(servoPin); 
        Serial.println(F("Scan RFID NUID...")); 
    }
    void loop() { 
        readRFID(); 
        if (nuidPICC[0] == ipadd[0] && nuidPICC[1] == ipadd[1]  && nuidPICC[2] == ipadd[2] && nuidPICC[3] == ipadd[3]) {
            approved = true;
            if (approved != oldApproved) {
                Serial.println(F("Card approved!")); 
                digitalWrite(ledPin, HIGH); servo.write(servoOpen);
                delay(delayBeforeClosing);
            } 
            nuidPICC[0] = 0; 
            nuidPICC[1] = 0;
            nuidPICC[2] = 0; 
            nuidPICC[3] = 0; 
        } 
        else { 
            approved = false; 
                if (approved != oldApproved) { 
                    Serial.println(F("Card incorrect!")); digitalWrite(ledPin, LOW); 
                    servo.write(servoClose);
               } 
        } 
        oldApproved = approved; 
    } 
    void readRFID() {  
            if ( ! rfid.PICC_IsNewCardPresent()) return; 
        
            if ( !rfid.PICC_ReadCardSerial()) return; 
            if (rfid.uid.uidByte[0] != nuidPICC[0] || rfid.uid.uidByte[1] != nuidPICC[1] || rfid.uid.uidByte[2] != nuidPICC[2] || rfid.uid.uidByte[3] != nuidPICC[3] ){
                Serial.println(F("A new card has been detected.")); 
                for (byte i = 0; i < 4; i++) { 
                    nuidPICC[i] = rfid.uid.uidByte[i]; 
                } 
                Serial.print(F("RFID tag in dec: ")); 
                printDec(rfid.uid.uidByte, rfid.uid.size); 
                Serial.println(); 
            } 
                rfid.PICC_HaltA(); 
                rfid.PCD_StopCrypto1(); } 
                
    void printDec(byte *buffer, byte bufferSize) {
            for (byte i = 0; i < bufferSize; i++) { 
                    Serial.print(buffer[i] < 0x10 ? " 0" : " "); 
                    Serial.print(buffer[i], DEC); 
            } 
    } 



