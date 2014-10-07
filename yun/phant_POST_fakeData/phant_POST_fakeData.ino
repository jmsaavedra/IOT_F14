/*
Simple data.sparkfun.com example

go there to get the public and private keys needed to make this example work

 */


// include all Libraries needed:
#include <Bridge.h>
#include <HttpClient.h>
#include "passwords.h"      // contains my passwords, see below

/*
  NOTE: passwords.h is not included with this repo because it contains my passwords.
 You need to create it for your own version of this application.  To do so, make
 a new tab in Arduino, call it passwords.h, and include the following variables and constants:

 #define PUBLICKEY        "foo"                 // replace your sparkfun public key here
 #define PRIVATEKEY       "0000"                // replace your sparkfun secret key here
 */


// set up net client info:
unsigned long postingInterval = 25000;  //delay between updates
unsigned long lastRequest = 0;      // when you last made a request
unsigned long currTime = 0;
String dataString = "";
int sineVal;
int sineMod = 20;
int squareVal;
unsigned long lastSquare = 0;
int rampVal;

void setup() {
  // start serial port:
  Bridge.begin();
  Console.begin();

  //***************************************************************//
  // comment out this line if you want Yun to run w/o console open:
  //while (!Console);   // wait for Network Serial to open
  //***************************************************************//

  Console.println("data.sparkfun client");

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
      updateData();
      sendData();
      lastRequest = currTime;
  }
}

void updateData() {
  //sine wave:
  sineVal += sineMod;
  if(sineVal > 255 || sineVal <= 0) sineMod *= -1;

  if(squareVal == 255){
    squareVal = 0;
  } else squareVal = 255;
  
  rampVal += abs(sineMod);
  if(rampVal > 260) rampVal = 0;
  
  // convert the readings to a String to send it:
  dataString = "sine=";
  dataString += sineVal;
  dataString += "&square=";
  dataString += squareVal;
  dataString += "&ramp=";
  dataString += rampVal;
}

// this method makes a HTTP connection to the server:
void sendData() {
  // form the string for the API header parameter:
  // form the string for the URL parameter:
  String url = "http://data.sparkfun.com/input/";
  url += PUBLICKEY;
  url += "?private_key=";
  url += PRIVATEKEY;
  url += "&";
  url += dataString;

  // Send the HTTP GET request

  HttpClient client;
  Console.print("\n\nSending data... ");
  Console.println(url);
  client.get(url);
  Console.println("done!");

  // If there's incoming data from the net connection,
  // send it out the Console:
  while (client.available()) {
    char c = client.read();
    Console.print(c);
  }
  Console.flush();
}


