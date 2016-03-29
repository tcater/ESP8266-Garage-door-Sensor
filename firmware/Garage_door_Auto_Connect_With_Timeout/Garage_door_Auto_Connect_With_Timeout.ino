#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <WiFiClient.h>

//needed for library
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager

// ----------------pin values--------------------------
const int shutdownPin = 14; // defines the hold pin (set High to trip the MOSFET and power down).
const int tiltPin = 13; //define the tilt sensor

// ----------------iot values--------------------------
String iotID = "GDJohn";
const char* iotNotify = "0";
const char* iotStatus ;

// ----------------host values--------------------------
const char* host = "www.tristancater.com";


void setup() {
  // put your setup code here, to run once:
  pinMode(shutdownPin, OUTPUT);
  pinMode(tiltPin, INPUT);
 
  digitalWrite(shutdownPin, HIGH); //ensure pin is high so that board doesn't immediately shutdown

  Serial.begin ( 74880 );
  
  //WiFiManager
  //Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;
  //reset settings - for testing
  //wifiManager.resetSettings();

  //sets timeout until configuration portal gets turned off
  //useful to make it all retry or go to sleep
  //in seconds
  wifiManager.setTimeout(180);
  
  //fetches ssid and pass and tries to connect
  //if it does not connect it starts an access point with the specified name
  //here  "AutoConnectAP"
  //and goes into a blocking loop awaiting configuration
  if(!wifiManager.autoConnect("DoorConnectAP","password")) {
    Serial.println("failed to connect and hit timeout");
    Serial.println ( "Shutting down" );
    delay(1000);
    digitalWrite(shutdownPin, LOW); //Shutdown
    delay(1000);
  } 

  //if you get here you have connected to the WiFi
  
  if(digitalRead(tiltPin) == 0){
    iotStatus = "closed";
  }else{
    iotStatus = "open";
  }
  
  Serial.print("connecting to ");
  Serial.println(host);

  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  int tries = 0;
  client.connect(host,httpPort);

  // We now create a URI for the request
  String url = "/iot/server.php?iotID=" + iotID + "&iotMSG=" + iotStatus + "&iotNotify=" + iotNotify  ;
    
  Serial.print("Requesting URL: ");
  Serial.println(url);
  
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
  "Host: " + host + "\r\n" + 
  "Connection: close\r\n\r\n");
  delay(10);
  
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
  String line = client.readStringUntil('\r');
  Serial.print(line);
  }
  
  Serial.println();
  Serial.println("closing connection");  
  delay(1000);

  Serial.println ( "Shutting down" );
  delay(1000);
  digitalWrite(shutdownPin, LOW); //Shutdown
  delay(2000);
}

void loop () {
  //nothing to see here
  Serial.print("uh hello...");
  Serial.println("");
  delay(10000);
}

