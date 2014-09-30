/*
Simple data.sparkfun.com example
go there to get the public and private keys needed to make this example work

GET Request example: Pulls the most recent values from your data.sparkfun datastream!
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
const unsigned long requestingInterval = 10000;  //delay between server requests
unsigned long lastRequest = 0;      // when you last made a request

String incomingDataString = ""; //this will hold incoming data string

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


//---- the loop!
void loop() {
  // get a timestamp so you can calculate reading and sending intervals:
  long now = millis();

  // if the request interval has passed since your
  // last connection, then connect again and get data:
  if (now - lastRequest >= requestingInterval) {
    getData();
    lastRequest = now;
  }
}


//------ this method makes a HTTP connection to the server, and listens for a reply
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
  
  
  boolean foundData = false; //set to false, we've not found any data yet
  Console.println(">>> RAW SERVER REPLY: ");
  // while there's incoming data from the net connection:
  while (client.available()) {
    
    char c = client.read(); //read this char
    Console.print(c);  //print this char
    if(foundData) incomingDataString += c; //add this char to our dataString, if Found Data
    if ( c == '\n' ) { //look for a new line character
      //Console.println(">> NEW LINE"); //for debugging our messages
      if(foundData) break; //we only want the MOST RECENT line of data (just one!), exit this loop!
      foundData = true; //foundData only true AFTER THE FIRST LINE (info line)
    }
  } //no more incoming data -OR- we called "break;"
  
  Console.println("<<<");
  //now we're done with that while() loop, parse out what we got from it.
  parseData(incomingDataString); //parse out the incomingDataString
  Console.flush(); //delete anything remaining in the console (older data)
}



//----- this function parses out your datums!
void parseData(String data){
  
  Console.println("\nparsing data from: ");
  Console.println(data);
  
  //first find all our commas. if you have third, fourth, fifth vals... add them here
  int firstValIndex = data.indexOf(','); //find first comma from beginning of string
  int secondValIndex = data.indexOf(',', firstValIndex+1); //find second comma starting at last comma
  
  //pull out our values as strings
  String firstValString = data.substring(0, firstValIndex); 
  String secondValString = data.substring(firstValIndex+1, secondValIndex);

  //convert our values to integers (so we can compare, etc)
  int firstVal = firstValString.toInt();
  int secondVal = secondValString.toInt();
  
  //print out dem vals
  Console.print("firstVal: ");
  Console.println(firstVal);  
  Console.print("secondVal: ");
  Console.println(secondVal);
  Console.print("\n-----------------------------");
  incomingDataString = ""; //clear out this data string, we're done with it! get it ready for another message.
}


