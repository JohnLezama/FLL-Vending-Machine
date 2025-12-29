#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
#include <Servo.h>
SoftwareSerial mySerial(A4, A5);

int globalX = 0;
int globalY = 0;

char firstLetter = '0';
char secondLetter = '1';

LiquidCrystal_I2C lcd(0x3F, 16, 2);

#define ENABLE
#define stepX 2
#define dirX 5
#define stepY 3
#define dirY 6
#define stepZ 4
#define dirZ 7
#define enPin 8
#define DOWN_BUTTON 9
#define SELECT_BUTTON 10
#define UP_BUTTON 11
#define electroMagnet 12

Servo arm;

void setup() {
  pinMode(stepX,OUTPUT);
  pinMode(dirX,OUTPUT);
  pinMode(stepY,OUTPUT);
  pinMode(dirY,OUTPUT);
  pinMode(stepZ,OUTPUT);
  pinMode(dirZ,OUTPUT);
  pinMode(enPin,OUTPUT);
  pinMode(electroMagnet, OUTPUT);
  pinMode(UP_BUTTON, INPUT_PULLUP);
  pinMode(SELECT_BUTTON, INPUT_PULLUP);
  pinMode(DOWN_BUTTON, INPUT_PULLUP);

  digitalWrite(enPin,LOW);
  digitalWrite(dirX,HIGH);
  digitalWrite(dirY,HIGH);
  digitalWrite(dirZ,HIGH);
  lcd.init();
  lcd.clear();
  lcd.backlight();
  mySerial.begin(9600);
  arm.attach(13);

}

void loop() {
  lcd.setCursor(1,0);
  lcd.print(firstLetter);
  lcd.setCursor(0,1);
  lcd.print(">");
  lcd.print(secondLetter);
  if (mySerial.available()){
      char input = mySerial.read();
      switch(input){
        case '1':
          collectAndInsertPackage(0, 100);
          break;
        case '2':
          collectAndInsertPackage(300, 100);
          break;
        case '3':
          collectAndInsertPackage(600, 100);
          break;
        case '4':
          collectAndInsertPackage(0, 400);
          break;
        case '5':
          collectAndInsertPackage(300, 400);
          break;
        case '6':
          collectAndInsertPackage(600, 400);
          break;
        case '7':
          collectAndInsertPackage(0, 700);
          break;
        case '8':
          collectAndInsertPackage(300, 700);
          break;
        case '9':
          collectAndInsertPackage(600, 700);
          break;
        case 'A':
          collectAndInsertPackage(0, 1000);
          break;
        case 'B':
          collectAndInsertPackage(300, 1000);
          break;
        case 'C':
          collectAndInsertPackage(600, 1000);
          break;
      }
  }
  if(digitalRead(SELECT_BUTTON) == LOW){
    switch(secondLetter){
      case '1':
        dropOffPackage(0, 100);
        break;
      case '2':
        dropOffPackage(300, 100);
        break;
      case '3':
        dropOffPackage(600, 100);
        break;
      case '4':
        dropOffPackage(0, 400);
        break;
      case '5':
        dropOffPackage(300, 400);
        break;
      case '6':
        dropOffPackage(600, 400);
        break;
      case '7':
        dropOffPackage(0, 700);
        break;
      case '8':
        dropOffPackage(300, 700);
        break;
      case '9':
        dropOffPackage(600, 700);
        break;
      case 'A':
        dropOffPackage(0, 1000);
        break;
      case 'B':
        dropOffPackage(300, 1000);
        break;
      case 'C':
        dropOffPackage(600, 1000);
        break;
    }
  }
  if(digitalRead(DOWN_BUTTON) == LOW){
    if(secondLetter != 'C'){
      if(secondLetter=='9'){
        secondLetter = 'A';
        firstLetter++;
      }
      else if(firstLetter == '9'){
        firstLetter = 'A';
        secondLetter++;
      }
      else{
        secondLetter++;
        firstLetter++;
      }
    }
  }
  if(digitalRead(UP_BUTTON) == LOW){
    if(firstLetter!='0'){
      if(firstLetter =='A'){
        firstLetter = '9';
        secondLetter--;
      }
      else if(secondLetter == 'A'){
        secondLetter = '9';
        firstLetter--;
      }
      else{
        firstLetter--;
        secondLetter--;
      }
    }
  }
}

void dropOffPackage(int x, int y){
  step(y-globalY, stepY);
  globalY = y;
  step(x-globalX, stepX);
  globalX=x;
  grabOject();
  step(800-globalX, stepX);
  globalX = 800;
  step(1000-globalY, stepY);
  globalY = 1000;
  digitalWrite(electroMagnet, LOW);
  delay(1000);
}

void collectAndInsertPackage(int x, int y){
  step(y-globalY, stepY);
  globalY = y;
  step(800-globalX, stepX);
  globalX=800;
  collectObject();
  step(-x, stepX);
  globalX=800-x;
  insertObject();
}

void step(int steps, int pin){
  if(steps>0){
    digitalWrite(pin+3, HIGH);
  }
  else{
    digitalWrite(pin+3, LOW);
    steps*=-1;
  }
  for(int x = 0; x<steps; x++){
    digitalWrite(pin, HIGH);
    delayMicroseconds(1000);
    digitalWrite(pin, LOW);
    delayMicroseconds(1000);
  }
}

void insertObject(){
  step(500, stepZ);
  digitalWrite(electroMagnet,LOW);
  delay(500);
  step(100, stepX);
  step(-100, stepX);
  step(-500, stepZ);
}

void grabOject(){
  step(500, stepZ);
  digitalWrite(electroMagnet,HIGH);
  delay(1000);
  step(-500, stepZ);
}

void collectObject(){
  delay(1000);
  arm.write(90);
  digitalWrite(electroMagnet, HIGH);
  delay(1000);
  arm.write(0);
}