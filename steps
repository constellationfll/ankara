int stepMag[10];
String stepDirection[10];
int stepCount = 0;
int toplam = 0;
void saveSteps(int stepmag,String stepdirection){
  stepMag[stepCount] = stepmag;
  stepDirection[stepCount] =  stepdirection;
  stepCount++;
  Serial.println(stepCount);
}
void inOrOut(){
  for(int i = 0;i<stepCount;i++){
    //toplam = toplam + stepMag[i];
    calcSteps(stepDirection[i],stepMag[i]);
     printSI(stepDirection[i],stepMag[i]);
  }
}

void clearStepData(){
stepMag[10] = {0};
stepDirection[10] = {""};
stepCount = 0;
}