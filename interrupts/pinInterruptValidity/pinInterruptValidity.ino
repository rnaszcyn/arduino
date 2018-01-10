/*
  Pin interrupt testing code.
  Goal is to jist determine if the connected board supports pin interrupts as well as
  what pins are vaild for such.
*/

const int maxPinNumber = 17; // set the maximum number of addressable pins here
int runFlag = HIGH;

void setup() {
  Serial.begin(115200);

}

void loop() {
  if( runFlag == HIGH ) {
    runFlag = LOW;
    //
    for(int i=0; i<=maxPinNumber; i++){
      Serial.write("pin ");
      Serial.write(String(i).c_str());
      Serial.write(" maps to ");
      Serial.write(String(digitalPinToInterrupt(i)).c_str());
      if (digitalPinToInterrupt(i) == -1)
        Serial.write(" - INVALID\n");
      else
        Serial.write(" - valid\n");
     }
   }

}
