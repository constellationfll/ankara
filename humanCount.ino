#define MAX_STEPS 2
int humanCount = 0;
int humMulti = 0;
void human(int totalStep) {
  
  if (abs(totalStep) != 0 && abs(totalStep) > 3) {
    humMulti = totalStep / MAX_STEPS;
  }

  if (abs(totalStep) == MAX_STEPS) {
    humMulti = 1;
  }

  if (totalStep > 0) {
    humanCount = humanCount + humMulti;
  }

  if (totalStep < 0) {
    humanCount = humanCount - humMulti;
  }
  printSI("İnsan Sayisi : ",humanCount);
   sendData(humanCount);
}