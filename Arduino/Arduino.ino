#include <LedControl.h>
#include <DHT.h>

LedControl lc=LedControl(12,11,10,4); // these are the pinouts in this order: (DataIn, CLK, LOAD, NUMBER of modules used)

// 0 is top left corner, 1 is top right corner, 2 is bottom left corner and 3 is bottom right corner
#define DHTPIN 4 
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

int t = 0;
int h = 0;
int buttonCount = 0;

unsigned long lastRead;

byte eyeOpen[8] = {
 0b00111100,
 0b01111110,
 0b11100111,
 0b11000011,
 0b11000011,
 0b11100111,
 0b01111110,
 0b00111100
};

byte eyeBlink[8] = {
  0b00000000,
  0b00000000,
  0b11111111,
  0b11111111,
  0b11111111,
  0b11111111,
  0b00000000,
  0b00000000
};

byte eyeLefttriangle[8]{
  0b00000000,
  0b00110000,
  0b00111000,
  0b00111100,
  0b00111110,
  0b00000000,
  0b00000000,
  0b00000000
};

byte eyeRighttriangle[8]{
  0b00000000,
  0b00001100,
  0b00011100,
  0b00111100,
  0b01111100,
  0b00000000,
  0b00000000,
  0b00000000
};
byte mouthLeft[8] = {
  0b00000000,
  0b00000000,
  0b00111000,
  0b00111100,
  0b00011111,
  0b00001111,
  0b00000111,
  0b00000000
};

byte mouthRight[8] = {
  0b00000000,
  0b00000000,
  0b00011100,
  0b00111100,
  0b11111000,
  0b11110000,
  0b11110000,
  0b11100000
};

byte mouthLeftshock[8] = {
  0b00000000,
  0b00011111,
  0b00110000,
  0b01100000,
  0b01100000,
  0b00110000,
  0b00011111,
  0b00000000
};
byte mouthRightshock[8] = {
  0b00000000,
  0b11111000,
  0b00001100,
  0b00000110,
  0b00000110,
  0b00001100,
  0b11111000,
  0b00000000
};

byte loadPos1[8] = {
  0b00000000,
  0b00000000,
  0b00000000,
  0b11110000,
  0b11110000,
  0b11110000,
  0b00000000,
  0b00000000
};

byte loadPos2[8] = {
  0b00000000,
  0b00000000,
  0b00000000,
  0b00001111,
  0b00001111,
  0b00001111,
  0b00000000,
  0b00000000
};

int LDR = A1;                             //LDR PINOUT
int buttonPin = 7;

void drawEyes(byte shape[8]){
 for (int a = 0; a<8; a++){
  lc.setRow(0, a, shape[a]);       // top left
  lc.setRow(1, a, shape[a]);       // top right (EYES)
 }
}

void drawMouth(){
  for (int b = 0; b<8; b++){
    lc.setRow (2,b, mouthLeft[b]);             //bottom left (MOUTH)
    lc.setRow (3,b, mouthRight[b]);             //bottom right
  }
}
void drawShock(){
  for (int g = 0; g<8; g++){
    lc.setRow(2,g,mouthLeftshock[g]);
    lc.setRow(3, g, mouthRightshock[g]);
  }
}
void loadingBar(){
  for (int c = 0; c<8; c++){
    lc.setRow (0, c, loadPos1[c]);
    lc.setRow (1, c, loadPos1[c]);
  }
  delay(200);
  for (int d = 0; d<8; d++){
    lc.setRow (0, d, loadPos2[d]);
    lc.setRow (1, d, loadPos2[d]);
  }
}

void wink(int wink, int open){
  while(wink > 1 || open > 1 ){
    Serial.println("ONLY EYES CAN BE WINKED!!");
  }
  for (int e = 0; e<8; e++){
    lc.setRow(wink, e, eyeBlink[e]);
    lc.setRow(open, e, eyeOpen[e]);
  }
}
void sadFace(){
  drawEyes(eyeBlink);
  for (int h= 0; h<8; h++){
    lc.setRow (2, h, mouthLeft[h]);
    lc.setRow(3, h, mouthRight[h]); // need to flip
  }
}
void angryFace(){
  for(int j = 0; j<8; j++){
    lc.setRow(0, j, eyeLefttriangle[j]);
    lc.setRow(1, j, eyeRighttriangle[j]);
  }
  drawMouth();
}
void setup(){

   Serial.begin(9600);
   dht.begin();
   pinMode(LDR, INPUT);
   pinMode(buttonPin, INPUT_PULLUP);

  for(int i = 0; i<4; i++){

    lc.shutdown(i,false);
    lc.setIntensity(i,5);
    lc.clearDisplay(i);
  }

  loadingBar();
  delay(100);
  drawEyes(eyeBlink);
  drawMouth();
  delay(200);
  drawEyes(eyeOpen);
  delay(100);
  wink(1,0);
  delay(200);
  wink(0,1);


}

void loop(){


  if (micros() - lastRead >= 300){
    drawEyes(eyeBlink);
    delay(100);               // to change
    drawEyes(eyeOpen);
  }

  if (digitalRead(buttonPin) == HIGH) {
    buttonCount++;
    if(buttonCount > 4){
      buttonCount = 0;
    }
  }

  int light = analogRead(LDR);

  if (light < 100) {
    Serial.println("getting dark!!");
    for ( int f = 0; f<5; f++){
      drawEyes(eyeBlink);
      delay(10);
      drawEyes(eyeOpen);
    }

    drawEyes(eyeBlink);
    drawShock();

  if (micros() - lastRead >= 2000000 && buttonCount == 1){
    t = dht.readTemperature();
    h = dht.readHumidity();



  }
    
  }

  lastRead = micros();
}