#include <SoftwareSerial.h>

#define BUT_HORIZONTAL 8
#define BUT_VERTICAL 6
#define INIT_BUT_PIN 5
#define TIMEOUT 80
#define FINAL_TIMEOUT 3000

boolean ones[BUT_HORIZONTAL] [BUT_VERTICAL] = {false};
boolean button[BUT_HORIZONTAL][BUT_VERTICAL] = {false};
boolean buttonnew[BUT_HORIZONTAL][BUT_VERTICAL] = {true};
boolean firstStart = false;

int latchPin = 3;
int clockPin = 4;
int dataPin = 2;

boolean dizi[] = {1, 1, 1, 1, 1, 1, 1, 1};
int x = 0;
boolean start = false;

unsigned long t = 0;

unsigned long ti = 0;
unsigned long tf = 0;
boolean tonesi = false;

unsigned long ti1 = 0;
unsigned long tf1 = 0;
boolean tonesi1 = false;

boolean anydata = false;
boolean newones = false;
boolean veri = false;

boolean stepones = false;
boolean oldstart = false;
boolean ayakBasili = false;

int pointXmax = 0;
int pointYmax = 0;
int pointXmin = 0;
int pointYmin = 0;

int sayac = 0;

boolean upEdge = false;
boolean downEdge = false;
boolean a = false;
SoftwareSerial BtSerial(12, 11);
void setup() {
  Serial.begin(9600);

  for (int i = INIT_BUT_PIN; i < INIT_BUT_PIN + BUT_VERTICAL; i++) {
    pinMode(i, INPUT_PULLUP);
  }
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  // Kenar Sınırlarını belirler
  BtSerial.begin(9600);
  edgeDetection();
 isReady();
}

void loop() {
  t =  millis();
  edgeDetection();
  readButtons();
 
  if (downEdge) {
    if (upEdge) {
      if (finalTimer(upEdge)) {
        human(sayac);
        downEdge = false;
        upEdge = false;
        sayac = 0;
      }
    }
  }


  if (anydata && !isAnyButtonPressed()) {
    if (!tonesi && tf == 0) {
      ti = t;
      tonesi = true;
    }
    if (tf < TIMEOUT) {
      tf = t - ti;
    } else {
      tf = 0;
      ti = 0;
      tonesi = false;
      anydata = false;
      stepones = true;
    }
  }
  else {
    tf = 0;
    ti = 0;
    t = 0;
    tonesi = false;
  }

  if (start) {
    for (int i = 0; i < BUT_HORIZONTAL; i++) {
      for (int k = 0; k < BUT_VERTICAL; k++) {
        readButtons();
        veri = Button( buttonnew[i][k], i, k);
       // Serial.println(veri);
        if (veri) {
          point(i,k);
          oldstart = true;
          anydata = true;
        }
        if (anydata && veri) {
          tf = 0;
          ti = 0;
          t = 0;
          tonesi = false;
        }
      }
    }
  }
  if (anydata) {
    if (!isAnyButtonPressed() && !ayakBasili) {
      setMinMaxPoint();
      getDirection(pointXmin, pointXmax, 0, 0);
      ayakBasili = true;
      
    }
  }
  else {
    ayakBasili = false;
  }

  if (!isAnyButtonPressed() && !tonesi) {
    if (stepones || !start) {
      Serial.println("x");
      inOrOut();
      clearStepData();
      clearPointData();
      stepones = false;
    }
  }
}

boolean finalTimer(boolean c) {
  boolean timeout = false;
  if (c) {
    if (!tonesi1 && tf1 == 0) {
      ti1 = t;
      tonesi1 = true;
    }
    if (tf1 < FINAL_TIMEOUT) {
      tf1 = t - ti1;
      printSI("timer", tf1);
    } else {
      tf1 = 0;
      ti1 = 0;
      tonesi1 = false;
      timeout = true;
    }
  }
  return timeout;
}

boolean isAnyButtonPressed() {
 
  boolean y = false;
  for (int i = 0; i < BUT_HORIZONTAL; i++) {
    for (int k = 0; k < BUT_VERTICAL; k++) {
      readButtons();
      if (!buttonnew[i][k]) {
        y = true;
        break;
      }
    }
  }
  return y;
}

void readButtons() {
  buttonShift(BUT_HORIZONTAL);
  for (int i = 0; i < BUT_VERTICAL; i++) {
    if (x > 0) {
      buttonnew[x - 1][i] = digitalRead(i + INIT_BUT_PIN);
      //    Serial.println(buttonnew[x-1][i]);
    }
  }

  for (int i = 0; i < BUT_HORIZONTAL; i++) {
    for (int k = 0; k < BUT_VERTICAL; k++) {
      if (buttonnew[i][k]) {
        start = true;
      }
      else {
        start = false;
        break;
      }
    }
  }
}

boolean Button(boolean value, int x, int y) {
  boolean reverse = false;
  button[x][y] = value;
  if (!button[x][y] && !ones[x][y]) {
    reverse = true;
    ones[x][y] = true;
  }
  if (button[x][y]) {
    ones[x][y] = false;
  }
  return reverse;
}

void buttonShift(int max_vertical) {
  shiftReset(max_vertical);
  
  digitalWrite(latchPin, 0);        //latchpin "0" konumuna getirilir
  
  for (int i = 0 ; i < 8; i++)
  {
    digitalWrite(dataPin, dizi[i]);
    digitalWrite(clockPin, 1);      //saat darbesi ile yazma ve kaydırma işlemi yapılır
    digitalWrite(clockPin, 0);
   // delay(100);
  }
  digitalWrite(latchPin, 1);        //8 bitli dizi çıkışa verilir
  //while(1);
}

void shiftReset(int max_vertical) {

  for (int i = 0 ; i < 8 ; i++) {
    if (i == x) {
      dizi[x] = 0;
    }
    else {
      dizi[i] = 1;
    }
  
    
  }
  x++;
  if (x >max_vertical) {
    x = 0;
    hardReset();
  }
}
void hardReset() {
  digitalWrite(latchPin, 0);        //latchpin "0" konumuna getirilir
  for (int i = 0 ; i < 8 ; i++)
  {
    digitalWrite(dataPin, 1);       //datapin'e x değeri verilir
    digitalWrite(clockPin, 1);      //saat darbesi ile yazma ve kaydırma işlemi yapılır
    digitalWrite(clockPin, 0);
  }
  digitalWrite(latchPin, 1);
}

void printSS(String a, String b) {
  Serial.print(a);

  Serial.println(b);
}
void printSI(String a, int b) {
  Serial.print(a);

  Serial.println(b);
}
void printII(int a, int b) {
  Serial.print(a);

  Serial.println(b);
}