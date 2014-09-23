/*
Simple data.sparkfun.com example

go there to get the public and private keys needed to make this example work

 */


// include all Libraries needed:
//#include <Process.h>
#include <Bridge.h>
#include <HttpClient.h>
#include "passwords.h"      // contains my passwords, see below

/*
  NOTE: passwords.h is not included with this repo because it contains my passwords.
 You need to create it for your own version of this application.  To do so, make
 a new tab in Arduino, call it passwords.h, and include the following variables and constants:

 #define APIKEY        "foo"                  // replace your sparkfun secret key here
 #define FEEDID        "0000"                  // replace your sparkfun public key here
 */


// set up net client info:
const unsigned long postingInterval = 10000;  //delay between updates to xively.com
unsigned long lastRequest = 0;      // when you last made a request
String dataString = "";

void setup() {
  // start serial port:
  Bridge.begin();
  Console.begin();

  while (!Console);   // wait for Network Serial to open
  Console.println("data.sparkfun client");

  // Do a first update immediately
  updateData();
  sendData();
  lastRequest = millis();
}

void loop() {
  // get a timestamp so you can calculate reading and sending intervals:
  long now = millis();

  // if the sending interval has passed since your
  // last connection, then connect again and send data:
  if (now - lastRequest >= postingInterval) {
    updateData();
    sendData();
    lastRequest = now;
  }
}

void updateData() {
  // convert the readings to a String to send it:
    dataString = "pressure=";
  dataString += random(5) + 100;
  dataString += "&temperature=";
  dataString += random(10) + 20;
  // add pressure:

}

// this method makes a HTTP connection to the server:
void sendData() {
  // form the string for the API header parameter:
 // form the string for the URL parameter:
  String url = "http://data.sparkfun.com/input/";
  url += FEEDID;
  url += "?private_key=";
  url += APIKEY;
  url += "&";
  url += dataString;

  // Send the HTTP GET request

  HttpClient client;
  //Process xively;
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




