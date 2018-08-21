/*  Copyright (C)  2018  Chirag Parmar, Avyay Sah ,and Anith Patel.
    Permission is granted to copy, distribute and/or modify this document
    under the terms of the GNU Free Documentation License, Version 1.3
    or any later version published by the Free Software Foundation;
    with no Invariant Sections, no Front-Cover Texts, and no Back-Cover Texts.
    A copy of the license is included in the section entitled "GNU
    Free Documentation License". */


#include <AltSoftSerial.h>
#include<SPI.h>
#include "RF24.h"
#include "nRF24L01.h"
#include "Arduino.h"
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>

const int buttonPin = 2;  //Interrupt
int buttonState = 0;

RF24 myRadio(2, 3);
byte addresses[][6] = {"1"};
 
AltSoftSerial BTserial;  
boolean NL = true;

int send_num=random(0,6);
struct package
{
  char c=' ';
  boolean connection=false;
  int check_send=send_num;
  int read_num=0;
};

typedef struct package Package;
Package data;


boolean Checkconnection(int checks)
{
  int check2=checks-3;
  Serial.print("Check2=");
  Serial.println(check2);
  if(check2 ==send_num)
  {
    return true;
  }
  else
  {
    return false;
  }
}

void enterSleep(void)
{
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  sleep_mode();
}

void exitSleep(void)
{
  sleep_disable();
  power_all_enable();
}

void pin_ISR() {
  buttonState = digitalRead(buttonPin);
  if(buttonState == 1){
    exitSleep();
    }  
}

void Transmit()
{
  myRadio.stopListening();
  myRadio.openWritingPipe(addresses[0]);
  boolean check=myRadio.write(&data, sizeof(data));
  if(!check && data.connection==false)
  {
    Transmit();
  }
  else
  {
    Serial.println("ok");
    
  }
}

void Receive()
{
    myRadio.startListening();
    if(myRadio.available())
    {
      myRadio.read(&data, sizeof(data));
      data.connection=Checkconnection(data.read_num);
    }
    else
    {
      Serial.println("Data Not Received");
      Receive();
    }
}

void Vibrate()
{

  switch(data.c)
  {
    case 'R':
    
    //for(int x=0; x<=153; x++)
    //{
     // analogWrite(5, x);
      //delay(10);
    //}
    digitalWrite(5, HIGH);
    delay(1500);
    digitalWrite(5, LOW);
    delay(1000);
    
    //for(int x=0; x<=153;x++)
    //{
      //analogWrite(5,x);
     // delay(10); 
    //}
    
    //delay(500);
    //analogWrite(5, 0);
    data.c=' ';
    break;

    case 'r':

    digitalWrite(5, HIGH);
    delay(1500);
    digitalWrite(5, LOW);
    delay(1000);
    
   // for(int x=0; x<=153; x++)
   // {
     // analogWrite(5, x);
    //  delay(10);
   // }
    
    delay(800);
    analogWrite(5, 0);
    data.c=' ';
    break;

    case 's':

    digitalWrite(5, HIGH);
    delay(1500);
    digitalWrite(5, LOW);
    delay(1000);

    //delay(180);

    //for(int x=0; x<=153; x++)
    //{
      //analogWrite(5, x);
     // delay(10);
   // }
    
   // delay(800);
    //analogWrite(5, 0);
    data.c=' ';
    break;

    case 'w':

    digitalWrite(5, HIGH);
    delay(1500);
    digitalWrite(5, LOW);
    delay(1000);

    //delay(180);

    //for( int x=0; x<2; x++)
    //{
      //for(int y=0; y<=153; y++)
      //{
        //analogWrite(5, y);
        //delay(10);
      //}
      //for(int z=153; z>=0; z--)
      //{
        //analogWrite(5, z);
        //delay(10);
      //}
      //delay(100);
    //}
    
    data.c=' ';
    break;

    case 'x':
    data.c=' ';
    enterSleep();
    break;
    

    case 'd':

    digitalWrite(5, HIGH);
    delay(1500);
    digitalWrite(5, LOW);
    delay(1000);

    //delay(160);

    //for(int x=1;x<=2;x++)
    //{
      //for(int y=0;y<=153;y++)
      //{
        //analogWrite(5,y);
      //}
      //delay(500);
      //analogWrite(5, 0);
      //delay(300);
    //}
    data.c=' ';
    break;

    default:
    Serial.println("Wrong character entered");
    data.c=' ';
  }
  
  
}

 
 
void Bluetooth()
{
   if (BTserial.available())
    {
      delay(10);
        data.c = BTserial.read();
    }
 
 
    
    if (Serial.available())
    {
      delay(10);
        data.c = Serial.read();
 
       
        if (data.c!=10 & data.c!=13 ) 
        {  
             BTserial.write(data.c);
        }
 
        if (NL) { Serial.print("\r\n>");  NL = false; }
        Serial.write(data.c);
        if (data.c==10) { NL = true; }
    }
 }
 
void setup() 
{
    Serial.begin(9600);
    Serial.print("Sketch:   ");   Serial.println(__FILE__);
    Serial.print("Uploaded: ");   Serial.println(__DATE__);
    Serial.println(" ");
 
    BTserial.begin(9600);  
    Serial.println("BTserial started at 9600");
    delay(1000);

  myRadio.begin();
  myRadio.setChannel(115);
  myRadio.setPALevel(RF24_PA_MAX);
  myRadio.setDataRate(RF24_250KBPS);
  myRadio.openReadingPipe(1,addresses[0]);
  delay(1000);

  myRadio.printDetails();
  pinMode(5, OUTPUT);
  digitalWrite(4, HIGH);
  digitalWrite(7, HIGH);

   pinMode(buttonPin, INPUT);
  // Attach an interrupt to the ISR vector
  attachInterrupt(0, pin_ISR, CHANGE);
}
 
void loop()
{
  Bluetooth();
  Transmit();
  delay(50);
  myRadio.setRetries(15,15);
  Serial.print("Number Transmitted: ");
  Serial.println(data.check_send);
  if(data.connection==false)
  {
    Receive();
    delay(500);
  }
  else
  {
    Serial.println("Connection Established");
    Serial.print("Character Received: ");
    Serial.println(data.c);
    Vibrate();
  }
  
}
