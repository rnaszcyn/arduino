/*
  Button testing code.
  Goal is to get the logic correct for determining the state of multiple buttons and recording the state

*/

void setRgbLedValue( int red, int green, int blue);
void debugButtonStates();
void bleAdvRequest();
void printIntPins();

const unsigned int maxButtons = 3;
const int redLedPolePin = 5;
const int greenLedPolePin = 6;
const int blueLedPolePin = 9;
const int toggleFwdPin = 21;
const int toggleBakPin = 22;
const int bleAdvButtonPin = 3;

// variables will change:
unsigned int buttonPins[maxButtons];    // array to store the pin numbers in
int buttonStates[maxButtons];         // array to store the current button states in
bool intrPinsPrint = LOW;

void setup() {

  // set up and start serial debugging info
  Serial.begin(115200);
  Serial.println("Button testing \n");
  Serial.println("--------------------------\n");
  
  // set up the pins that will be used by the buttons.
  buttonPins[0] = 18;
  buttonPins[1] = 19;
  buttonPins[2] = 20;
  
  // For each, button pin, enable for input and use the internal pullup resistor 
  for(int i=0; i<maxButtons; i++)
    pinMode( buttonPins[i], INPUT_PULLUP);  // will use the internal pullup resistor for each pin

  // setup toggle switch
  pinMode( toggleFwdPin, INPUT_PULLUP);
  pinMode( toggleBakPin, INPUT_PULLUP);

  // set up pins for RGB LED and set to green
  // pinMode( redLedPolePin, OUTPUT);
  // pinMode( greenLedPolePin, OUTPUT);
  // pinMode( blueLedPolePin, OUTPUT);
  // setRgbLedValue( 0, 255, 0);

  // set up bluetooth advertising toogle and interrupt
  pinMode( 8,OUTPUT); // built in led on 32u4 data logger
  
  // pinMode( bleAdvButtonPin, INPUT_PULLUP);
  digitalWrite( bleAdvButtonPin, HIGH );
  if ( digitalPinToInterrupt(bleAdvButtonPin) != -1) {
    digitalWrite( 8, HIGH);
    attachInterrupt(digitalPinToInterrupt(bleAdvButtonPin) , bleAdvRequest, CHANGE);
    Serial.println("BLE interrupt set\n");
  } else {
    digitalWrite( 13, HIGH);
    Serial.write("FAILURE setting BLE interrupt on requested pin\n");
  }
  interrupts();
}

void loop() {
  // setRgbLedValue( 0, 255, 0);
  
    // printIntPins();
  
  for(int i=0; i<maxButtons; i++) {
    buttonStates[i] = digitalRead(buttonPins[i]);
  }
  debugButtonStates();
  delay(1000);
}

/*
 * Interrupt routine called when the designated bluetooth button is pressed
 */
void bleAdvRequest() {
  digitalWrite( 13, HIGH);
  Serial.write("Bluetooth bonding requested -");
  if( digitalRead( bleAdvButtonPin ) == HIGH )
    Serial.write("HIGH\n");
  else
    Serial.write("LOW\n");
}

void debugButtonStates() {
  Serial.write( String(millis()).c_str() );
  if( digitalRead(toggleBakPin) == HIGH ) Serial.write(" - BAK");
  if( digitalRead(toggleFwdPin) == HIGH ) Serial.write(" - FWD");
  Serial.write(" - ");
  for(int i=0; i<maxButtons; i++) {
      if( buttonStates[i] == HIGH )
        Serial.write("0");
       else
        Serial.write("1");
  }
  Serial.write("\n");
}

void printIntPins() {
  for(int i=0; i<24; i++){
    Serial.write("pin ");
    Serial.write(String(i).c_str());
    Serial.write("maps to ");
    Serial.write(String(digitalPinToInterrupt(i)).c_str());
    if (digitalPinToInterrupt(i) == -1)
      Serial.write(" - INVALID\n");
    else
      Serial.write(" - valid\n");
  }
}
void setRgbLedValue( int redVal, int greenVal, int blueVal) {
  analogWrite( redLedPolePin, redVal );
  analogWrite( greenLedPolePin, greenVal );
  analogWrite( blueLedPolePin, blueVal );
}

