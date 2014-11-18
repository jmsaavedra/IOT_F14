/*
Simple Node REST API POST tester


 */


// include all Libraries needed:
#include <Bridge.h>
#include <HttpClient.h>

// SERVER INFO / DEFINITIONS

//SERVERADDRESS is YOUR IP on your local network.
// OR if you have a server on the cloud, it is the address/IP to that server
#define SERVERADDRESS    "192.168.2.243:8080" //server address (port 8080)

//timer variables
unsigned long postingInterval = 5000;  //delay between server updates
unsigned long lastRequest = 0;         // when you last made a request
unsigned long currTime = 0;            //what time is it right now
boolean serverResponded = false;       // keeping track of connection success/failure

//your data variables
int analogSensorVal;
int sineVal;
int sineMod = 20;
int squareVal;
int rampVal;

//global string to hold PUT request
String dataString = "";


void setup() {
  // start serial port:
  Bridge.begin();
  Console.begin();

  //***************************************************************//
  // comment out this line if you want Yun to run w/o console open:
  //while (!Console);   // wait for Network Serial to open
  //***************************************************************//

  Console.println("booting up Yun");

  // Do a first update immediately
  updateData();
  sendData();
  lastRequest = millis();
}

void loop() {
  // get a timestamp so you can calculate reading and sending intervals:
  currTime = millis();

  // if the sending interval has passed since your
  // last connection, then connect again and send data:
  if (currTime - lastRequest >= postingInterval) {
      if(serverResponded) Console.println("\n>>SERVER RESPONDED");
      else Console.println("\n>>LAST REQUEST FAILED");
      updateData();
      sendData();
      lastRequest = currTime;
  }
  
  delay(10); //just a little chill time between loops, not truly necessary
  
  /** here you can do lots of stuff like **/
  // - update any i2c or digital sensors
  // - show sensor data visualized/expressed locally
  // - maybe check someone else datastream? make a getData() function and timer?
  
}


//read any sensors, do any math
void updateData() {
  
  // poll an analog pin, store in variable
  analogSensorVal = analogRead(0); 
  
  //** generate some fake data waveforms **//
  // sine wave:
  //http://en.wikipedia.org/wiki/Sine_wave
  sineVal += sineMod;
  if(sineVal > 255 || sineVal <= 0) sineMod *= -1;

  //other waveforms: http://en.wikipedia.org/wiki/Non-sinusoidal_waveform
  //ramp/saw wave:
  rampVal += abs(sineMod);
  if(rampVal > 260) rampVal = 0;

  //square wave:
  if(squareVal > 0) squareVal = 0;
  else squareVal = 255;


  //** update values to the dataString to be sent **//
  dataString = "name=sine";       // must match query that is expected by Node
  dataString += "&data=";         // must match query that is expected by Node
  dataString += sineVal;
  //** building our query string here. it should look like: **//
  //http://MY_IP_ADDRESS:8080/input?name=sine&data=104
  
  
//  dataString += "&ramp=";     // must match name as setup on phant server
//  dataString += rampVal;
//  dataString += "&sensor1=";  // must match name as setup on phant server
//  dataString += analogSensorVal;
}

// this method makes a HTTP request to the server:
void sendData() {
  // form the string for the API header parameter:
  // form the string for the URL parameter:
 
  String url = SERVERADDRESS; //start the URL with the server and port
  url +="/input?";   //because we're inputting data into the server
  url += dataString; //finally add in our dataString that we built above
  
  /** this should finish as: **/
  //http://MY_IP_ADDRESS:8080/input?name=sine&data=104

  // Send the server an HTTP GET request
  HttpClient client;
  Console.print("\n>>> Attempting Server Request \nclient.get:  ");
  Console.println(url);
  client.get(url);

  // If there's incoming data from the net connection,
  // send it out the Console:
  serverResponded = false; //start this at false after server request
  Console.println(">> Server Response: "); //expecting something to be received
  while (client.available()) {
    char c = client.read();
    Console.print(c);
    serverResponded = true; //we did in fact get something back
  }
  Console.flush();
}


