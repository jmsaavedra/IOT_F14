

#include <Bridge.h>
#include <YunClient.h>

YunClient client;

  //********* CHANGE THIS ***********//
#define SERVER_ADDRESS "10.0.0.5"
#define SERVER_PORT 5000

unsigned long postingInterval = 500;  //delay between server updates
unsigned long lastPost = 0;         // when you last made a request
unsigned long currTime = 0;            //what time is it right now

String incomingDataString = ""; //this will hold raw incoming data string
boolean serverResponded = false;

void setup() {
  //pin setup
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  
  //open the bridge to wifi on the yun, open the Console
  Bridge.begin();
  Console.begin();
  

  //connect to YOUR SERVER'S IP ADDRESS + PORT
  client.connect(SERVER_ADDRESS, SERVER_PORT);
}

String hi = "hello from yun! count: ";
int count = 0;

int LEDValue = 0;

void loop() {
  
  // current time elapsed so you can calculate post intervals:
  currTime = millis();
  
  //check if it's time to post an update to the server
  if (currTime - lastPost >= postingInterval) {
    if (client.connected()){
      String data = hi+count;
      client.print(data);
      count++;
    } else {
      //no connection, try to make one again:
      Console.println("\nattempting to connect to server");
      client.connect(SERVER_ADDRESS, SERVER_PORT);
      delay(2000); //delay 2 seconds before trying another server reconnect
    } 
    lastPost = currTime;
  }
  
  //while client.available()==true (meaning >= 1 char of data awaits us) then:
  while (client.available()) {  
    //if(!serverResponded) Console.println(">>> Server Says : <<<"); //expecting something to be received
    char c = client.read();     //read this char
    if(isDigit(c)){
      LEDValue = String(c).toInt();
      digitalWrite(13, LEDValue);
      //Console.print("received digit!");
    }
    incomingDataString += c; //add this char to our dataString
    serverResponded = true; //regardless of data, we got a response
  } 
  
  if(serverResponded){
    Console.print("received from server: ");
    Console.println(incomingDataString);
    //if(incomingDataString == "toggle") LEDValue = !LEDValue; //if you're looking for a string
    incomingDataString = ""; //clear out our data string for next server message
    serverResponded = false; //start this at false after server request
    //if(incomingDataString == "1") digitalWrite(13, HIGH);
    //else digitalWrite(13, LOW);
  }
}

