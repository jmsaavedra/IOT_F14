/*
Simple data.sparkfun.com example
go there to get the public and private keys needed to make this example work

GET Request example: Pull data from your stream
*/


// include all Libraries needed:
//#include <Process.h>
#include <Bridge.h>
#include <HttpClient.h>

//the PUBLICKEY (URL) for the feed you're interested in: https://data.sparkfun.com/streams/PUBLICKEYISHERE
#define PUBLICKEY "roRN133Dajs6DJJlom1K" 


// set up net client info:
const int postingInterval = 15000;  //delay between server requests
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
  /* get a timestamp so you can calculate reading and sending intervals: */
  long currTime = millis();

  /* if the request interval has passed since your
  last connection, then connect again and get data: */
  if (currTime - lastRequest >= postingInterval) {
    getData();
    lastRequest = currTime;
  }
}


// this method makes a HTTP connection to the server:
void getData() {
  // form the string for the API header parameter:
  // form the string for the URL parameter:
  String url = "http://data.sparkfun.com/output/";
  url += PUBLICKEY;
  url += ".csv";
  url += "?page=1"; //uncomment this if you only want 1 page worth of data!

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


