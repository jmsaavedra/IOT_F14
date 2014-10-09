/*
Simple phant.io example

GET data from server. Build the URL using the SERVERADDRESS+PUBLICKEY to request
.csv, .json, .jsonp, or .atom data returned

 */


// include all Libraries needed:
#include <Bridge.h>
#include <HttpClient.h>

//no need to bother with passwords.h, this is a public feed
#define PUBLICKEY        "keoG2q2KDQf3mpLxXEdWS1evp3j" // replace your phant public key here

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
  while (!Console);   // wait for Network Serial to open
  //***************************************************************//

  Console.println("booting up phant.io client");

  // Do a first update immediately
  getData();
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
      getData();
      lastRequest = currTime;
  }
}



// this method makes a HTTP request to the server:
void getData() {
  // form the string for the API header parameter:
  // form the string for the URL parameter:
//  String url = "http://data.sparkfun.com/input/"; //BOOOOO
  String url = SERVERADDRESS;
  url +="/output/";   //because we're outputting data FROM the server
  url += PUBLICKEY;
  url += ".csv"; //".json" //".jsonp"
  url += "?page=1"; //just the lastest 50kb worth of data
  
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


