#include<SPI.h>
#include "RF24.h"
#include "nRF24L01.h"
#include "Arduino.h"
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>

RF24 myRadio(7, 8);

const int buttonPin = 2;  //Interrupt
int buttonState = 0;


byte addresses[][6] = {"1"};
int flag=0;
struct package
{
  char c=' ';
  boolean connection = false;
  int check_send=0;
  int read_num=0;
};

typedef struct package Package;
Package data;

void Transmit()
{
    myRadio.stopListening();
    myRadio.openWritingPipe(addresses[0]);
    boolean check=myRadio.write(&data, sizeof(data));
    if(check)
    {
      Serial.println("Ok");
      Serial.print("Data Transmitted: ");
    Serial.println(data.read_num);
    }
    else
    {
      Transmit();
    }
    
    
}
void Receive()
{
  myRadio.startListening();
    if(myRadio.available())
    {
      myRadio.read(&data, sizeof(data));
    }
    else if(data.connection == false)
    {
      Serial.println("NO DATA RECEIVED");
      Receive();
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

  


void Vibrate()
{
  switch(data.c)
  {
    case 'L':
    
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

    case 'l':

    digitalWrite(5, HIGH);
    delay(1500);
    digitalWrite(5, LOW);
    delay(1000);
    
   // for(int x=0; x<=153; x++)
   // {
     // analogWrite(5, x);
    //  delay(10);
   // }
    
    //delay(800);
    //analogWrite(5, 0);
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

    case 'y':
    data.c=' ';
    enterSleep();
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

void setup() {
  Serial.begin(9600);
  delay(100);
  myRadio.begin();
  myRadio.setChannel(115);
  myRadio.setPALevel(RF24_PA_MAX);
  myRadio.setDataRate(RF24_250KBPS);
  myRadio.openReadingPipe(1, addresses[0]);
  myRadio.printDetails();

pinMode(5, OUTPUT); 
  pinMode(buttonPin, INPUT); 
 attachInterrupt(0, pin_ISR, CHANGE);
  }

void loop() 
{
  
  Receive();
  delay(50);
  Serial.print("Number Received: ");
  Serial.println(data.check_send);
   if(data.connection == false)
   {
    data.read_num=data.check_send+3;
    Transmit();
    myRadio.setRetries(15,15);
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
