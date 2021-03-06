/*
Simple data.sparkfun.com example

go there to get the public and private keys needed to make this example work

 */


// include all Libraries needed:
#include <Bridge.h>
#include <HttpClient.h>
#include "passwords.h"      // contains my passwords, see below

/* NOTE: passwords.h is not included with this repo because it contains my passwords.
 You need to create it for your own version of this application.  To do so, make
 a new tab in Arduino, call it passwords.h, and include the following variables and constants:

 #define PUBLICKEY        "foo"                 // replace your phant public key here
 #define PRIVATEKEY       "0000"                // replace your phant secret key here 
*/

// SERVER INFO / DEFINITIONS
#define SERVERADDRESS    "http://thingternet.club" //where does this phant instance live?
//#define SERVERADDRESS    "http://data.sparkfun.com" // BOOO!!!!
// *** port info ***
//#define PORT           8080 
// We're doing serverside port forwarding for regular HTTP (Port 80) requests
// Port 80 is the default port for any standard HTTP request (aka browsers and the WWW)
// read more about server ports: http://goo.gl/esYhI
// list of standard port configurations: http://goo.gl/30BU

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

  Console.println("booting up phant.io client");

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
  dataString = "sine=";       // must match name as setup on phant server
  dataString += sineVal;
  dataString += "&square=";   // must match name as setup on phant server
  dataString += squareVal;
  dataString += "&ramp=";     // must match name as setup on phant server
  dataString += rampVal;
  dataString += "&sensor1=";  // must match name as setup on phant server
  dataString += analogSensorVal;
}

// this method makes a HTTP request to the server:
void sendData() {
  // form the string for the API header parameter:
  // form the string for the URL parameter:
//  String url = "http://data.sparkfun.com/input/"; //BOOOOO
  String url = SERVERADDRESS;
  url +="/input/";   //because we're inputting data into the server
  url += PUBLICKEY;
  url += "?private_key=";
  url += PRIVATEKEY;
  url += "&";
  url += dataString; //finally add in our dataString

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


