//RECIEVER//
#include <Servo.h>
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#define no_of_cells 3

struct packet {
  byte posw;
  byte posx;
  byte posy;
  byte posz;
} State;

RF24 radio(9, 10);
const uint64_t pipe =183 ;
Servo m1;   //create esc objects
Servo m2;
Servo m3;
Servo m4;
int w,x,y,z;
float ack=0.0;    //battery voltage

void reset(){
  x=1500;y=1000;z=1000;w=1500; //This is neccesary incase of transmission failure 
}
void setup(void) {
  m1.attach(3);
  m2.attach(5);
  m3.attach(6);
  m4.attach(7);
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(1, pipe);
  radio.startListening(); 
  radio.enableAckPayload();
  reset();    //intialize the values
  
}

void loop(void) {
  if (radio.available()) {
     Serial.println("radio available");
    radio.read(&State, sizeof(State));
      w=map(State.posw,0,255,1060,2000);
      x=map(State.posx,0,255,2000,1060);
      y=map(State.posy,0,255,1060,2000);
      z=map(State.posz,0,255,1060,2000);
      
    m1.writeMicroseconds(z);
    m2.writeMicroseconds(x);
    m3.writeMicroseconds(w);
    m4.writeMicroseconds(y);
    /*Serial.print(z);                  //incase any problem occurs uncomment and check values in Serial Monitor
    Serial.print(F(", "));
    Serial.print(x);
    Serial.print(F(", "));
    Serial.print(w);
    Serial.print(F(", "));
    Serial.println(y);*/
    ack=analogRead(A5)* (5.04/920)*no_of_cells ;    //battery voltage calculations
    Serial.println(ack);
    radio.writeAckPayload(1, &ack, sizeof(ack));      //send battery voltage to transmitter
  delay(5);
  }
else{
  reset();  //reset values and write to the esc
   m1.writeMicroseconds(z);
    m2.writeMicroseconds(x);
    m3.writeMicroseconds(w);
    m4.writeMicroseconds(y);
  Serial.println("no radio available");
  
  }
}
