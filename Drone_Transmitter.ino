//TRANSMITTER//
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include <LiquidCrystal.h>

struct packet {
  byte posw;
  byte posx;
  byte posy;
  byte posz;
} State;

RF24 radio(9, 10);
const uint64_t pipe =183 ;
float ack=0.0; //for current battery voltage

const int rs = 7, en = 6, d4 = 5, d5 = 4, d6 = 3, d7 = 2;     //lcd configurations
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup(void) {
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(pipe);
    radio.enableAckPayload();
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0,0);
  // Print a message to the LCD.
  lcd.print("BUBBERSONS DRONE");
  lcd.setCursor(0,1);
  // Print a message to the LCD.
  lcd.print("Thr");
   lcd.setCursor(4,1);
    lcd.print(analogRead(A3));
  lcd.setCursor(9,1);
  // Print a message to the LCD.
  lcd.print("Yaw"); 
   lcd.setCursor(13,1);
    lcd.print(State.posy);
  delay(1000);
  lcd.setCursor(0,1);
    lcd.print("Batt");

}

void loop(void) {
   
    
    State.posw = map(analogRead(A0),0, 1023, 0, 255);  //mark all your inputs I have only used 4 
    State.posx = map(analogRead(A1), 0, 1023, 255, 0);
    State.posy = map(analogRead(A2), 0, 1023, 0, 255);
    State.posz =  map(analogRead(A3),0, 1023, 0, 255); //esc-min,max
    radio.write(&State, sizeof(State));
    
    lcd.setCursor(13,1);
    lcd.print(State.posy);
    if ( radio.isAckPayloadAvailable() ) {
            radio.read(&ack, sizeof(ack));
      Serial.println(ack);        
         lcd.setCursor(4, 1);
  lcd.print(ack);     //print battery voltage
 }
        else {
            Serial.println("  Acknowledge but no data ");
        }          
    
    Serial.println(State.posz);

    
    }
