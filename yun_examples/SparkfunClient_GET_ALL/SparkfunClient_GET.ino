/*
Simple data.sparkfun.com example
go there to get the public and private keys needed to make this example work

GET Request example: Pull data from your stream
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

 #define PUBLICKEY        "foo"                 // replace your sparkfun public key here
 #define PRIVATEKEY       "0000"                // replace your sparkfun secret key here
 */


// set up net client info:
const unsigned long postingInterval = 10000;  //delay between updates to xively.com
unsigned long lastRequest = 0;      // when you last made a request
String dataString = "";

void setup() {
  // start serial port:
  Bridge.begin();
  Console.begin();

  //***************************************************************//
  // comment out this line if you want Yun to run w/o console open:
  while (!Console);   // wait for Network Serial to open
  //***************************************************************//

  Console.println("data.sparkfun client");

  // Do a first update immediately
  getData();
  lastRequest = millis();
}

void loop() {
  // get a timestamp so you can calculate reading and sending intervals:
  long now = millis();

  // if the request interval has passed since your
  // last connection, then connect again and get data:
//  if (now - lastRequest >= postingInterval) {
//    getData();
//    lastRequest = now;
//  }
}


// this method makes a HTTP connection to the server:
void getData() {
  // form the string for the API header parameter:
  // form the string for the URL parameter:
  String url = "http://data.sparkfun.com/output/";
  url += PUBLICKEY;
  url += ".csv";
  //url += "?page=1"; //uncomment this if you only want 1 page of data!

  // Send the HTTP GET request
  HttpClient client;
  //Process xively;
  Console.print("\n\nRequesting data... ");
  Console.println(url);
  client.get(url);

  // If there's incoming data from the net connection,
  // send it out the Console:
  while (client.available()) {
    char c = client.read();
    Console.print(c);
  }
  Console.flush();
}


