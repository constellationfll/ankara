
int direct = 0;
int magx = 0;
int magy = 0;
int magxMax =  BUT_HORIZONTAL - 1;
int magyMax =  BUT_VERTICAL - 1;
int index = 0;
int pointX[BUT_HORIZONTAL * BUT_VERTICAL];
int pointY[BUT_HORIZONTAL * BUT_VERTICAL];
int oldPointXmin = 0;
int oldPointXmax = 0;


boolean controlMaxPoint = false;

String directs = "null";

int fact = 0;
void point(int x, int y) {
  pointX[index] = y;
  pointY[index] = x;
  index++;
  
  if (isOnDownEdge(x, y)) {
    downEdge = true;
    Serial.println("aşağı");
    printSI("aşağı : ",x);
  }
  
  if (isOnUpEdge(x, y)) {
    printSI("yukarı : ",x);
    upEdge = true;
   Serial.println("yukarı");
  }
}

void setMinMaxPoint() {
  pointXmin = pointX[0];
  pointYmin = pointY[0];


  for (int i = 0; i < index; i++) {
    if (pointX[i] != 0 || pointY[i] != 0) {
      pointXmax = pointX[i];
      pointYmax = pointY[i];
      oldPointXmin = pointXmax;
    }
  }
  if (pointX[index - 1] == 0 && pointY[index - 1] == 0) {
    pointXmax = 0;
    pointYmax = 0;
  }

}
int getIndex() {
  return index;
}

void clearPointData() {
  for (int i = 0; i < index; i++) {
    pointX[i] = 0;
    pointY[i] = 0;
  }
  index = 0;
  directs = "null";
}

void getDirection(int mi , int ma, int ni, int na) {
  magx = mi - ma;
  magy = ni - na;

  if ((mi - ma) < 0) {
    directs = "İçeri";
  }
  if ((mi - ma) > 0) {
    directs = "Disari";
  }

  saveSteps(magx, directs);
}

int calcSteps(String value, int magxa) {
  if (value.equals("İçeri")) {
    if (abs(magxa) < magxMax) {
      fact = 1;
    }
  }

  if (value.equals("Disari")) {
    if (abs(magxa) < magxMax) {
      fact = -1;
    }
  }
  if (value.equals("null")) {
    if (abs(magxa) == 0) {
      fact = 0;
    }
  }
  if (abs(magxa) == magxMax) {
    fact = 0;
  }


  sayac = sayac + fact;
 
  printSI("adım sayisi : ",sayac);
}