/*
Simple phant.io GET example

  - GET data from server. Build the URL using the SERVERADDRESS+PUBLICKEY to request
  - data returned as.csv, .json, .jsonp, or .atom data returned
  - PARSING: After detecting 2 '\n' chars in the raw server response,
       we send the string to be parsed out to int variables.
       
  ** you can check your output in browser by going to any of the following: **
    http://SERVERADDRESS/output/PUBLICKEY.csv         //entire history as a .csv
    http://SERVERADDRESS/output/PUBLICKEY.csv?page=1  //latest 50kb
    http://SERVERADDRESS/output/PUBLICKEY/latest.csv  //just the latest data post
    http://SERVERADDRESS/output/PUBLICKEY/STREAM_NAME/latest.csv //just the 'sine' value

 */


// include all Libraries needed:
#include <Bridge.h>
#include <HttpClient.h>

//no need to bother with passwords.h, this is a public feed
#define PUBLICKEY        "keoG2q2KDQf3mpLxXEdWS1evp3j" // replace your phant public key here

//***name of the datastream you want to check on ***//
#define STREAM_NAME      "square"                      

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

//global string to hold GET request
String incomingDataString = ""; //this will hold raw incoming data string

//your global data variables
int myStreamVal = 0;

void setup() {
  // start serial port:
  Bridge.begin();
  Console.begin();

  //***************************************************************//
  // uncomment this line if you want Yun to wait for you to open the console before continuing
  // while (!Console);   // wait for Network Serial to open
  //***************************************************************//

  Console.println("booting up phant.io client");

  // Do a first update immediately
  getData();
  delay(10);
  lastRequest = millis();
}

void loop() {
  // current time elapsed so you can calculate request intervals:
  currTime = millis();

  // if the sending interval has passed since your
  // last connection, then connect again and send data:
  if (currTime - lastRequest >= postingInterval) {
    if (serverResponded) Console.println("\n>>SERVER RESPONDED");
    else Console.println("\n>>LAST REQUEST FAILED");
    getData();
    lastRequest = currTime;
  }
  delay(50); //chill pill on updating currTime

  //do other stuff, light up, turn motor, etc with the data returned
  analogWrite(13, myStreamVal);
}



// this method makes a HTTP request to the server:
void getData() {

  // Create the String for the URL we will attempt a GET from:
  String url = SERVERADDRESS; // server definition
  //  String url = "http://data.sparkfun.com/output/"; //BOOOOO
  url += "/output/";         // because we're outputting data FROM the server
  url += PUBLICKEY;          // add the publickey
  url += "/";
  url += STREAM_NAME;
  url += "/latest.csv";      //"latest" gets only the LATEST DATA POINT ENTRY    //".json" //".jsonp"

  // Send the server an HTTP GET request
  HttpClient client;
  Console.print("\n>>> Attempting Server Request \nclient.get:  ");
  Console.println(url); //print our url
  client.get(url);      //make server request with that exact url

  //** Now we're expecting a reply from the server **//

  serverResponded = false; //start this at false after server request
  Console.println(">>> Server Response : <<<"); //expecting something to be received

  while (client.available()) {  //if client.available()==true (meaning >= 1 char of data awaits us) then:
    char c = client.read();     //read this char
    Console.print(c);           //print this char
    incomingDataString += c; //add this char to our dataString, if Found Data
    serverResponded = true; //regardless of data, we got a response
  } //no more incoming data -OR- we called "break;"

  //now we're done with that while() loop, no more chars remain. now parse out what we got from it.
  myStreamVal = incomingDataString.toInt();
  
  Console.print("current value of \'");
  Console.print(STREAM_NAME);
  Console.print("\' = ");
  Console.println(myStreamVal);
  
  incomingDataString = ""; //clear out the datastring
  Console.flush(); //delete anything remaining in the console (older data)
}
