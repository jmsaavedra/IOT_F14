#include <Console.h>

const int ledPin = 13; // the pin that the LED is attached to
int incomingByte;      // a variable to read incoming serial data into
int myVar = 0;
bool counterOn = false;

void setup() {
  // initialize serial communication:
  Bridge.begin();
  Console.begin(); 

  while (!Console){
    ; // wait for Console port to connect.
  }
  Console.println("You're connected to the Console!!!!");
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // see if there's incoming serial data:
  if (Console.available() > 0) {
    // read the oldest byte in the serial buffer:
    incomingByte = Console.read();
    // if it's a capital H (ASCII 72), turn on the LED:
    if (incomingByte == 'H') {
      digitalWrite(ledPin, HIGH);
      Console.println("LED13 SET HIGH");
    } 
    // if it's an L (ASCII 76) turn off the LED:
    if (incomingByte == 'L') {
      digitalWrite(ledPin, LOW);
      Console.println("LED13 SET LOW");
    }
        // if it's an L (ASCII 76) turn off the LED:
    if (incomingByte == '=') {
      counterOn = !counterOn;
      Console.print("counter set to: ");
      Console.println(counterOn);
    }
  }
  if(counterOn){
    Console.println(myVar);
    if(myVar < 100) myVar++;
    else myVar = 0;
  }
  delay(100);
}
