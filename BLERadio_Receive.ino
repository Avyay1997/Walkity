#include<SPI.h>
#include "RF24.h"
#include "nRF24L01.h"

RF24 myRadio(7, 8);

byte addresses[][6] = {"1"};

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
    }
    Serial.print("Data Transmitted: ");
    Serial.println(data.read_num);
    
}
void Receive()
{
  myRadio.startListening();
    while(myRadio.available())
    {
      myRadio.read(&data, sizeof(data));
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
    Serial.print("Wrong character entered");
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
  }

void loop() 
{
  Receive();
  delay(500);
  Serial.print("Number Received: ");
  Serial.println(data.check_send);
   if(data.connection == false)
   {
    data.read_num=data.check_send+3;
    Serial.print("Updated Number: ");
    Serial.println(data.read_num);
    Transmit();
    myRadio.setRetries(15,15);
    delay(1000);
   }
   else
   {
    Serial.println("Connection Established");
    Vibrate();
   }
    
}
