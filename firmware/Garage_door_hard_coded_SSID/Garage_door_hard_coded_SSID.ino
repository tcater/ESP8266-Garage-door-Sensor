#include <ESP8266WiFi.h>
#include <WiFiClient.h>

// ----------------wifi values--------------------------
const char* ssid = "**YOUR SSID HERE**";
const char* password = "**YOUR PASSWORD HERE**";

// ----------------pin values--------------------------
const int shutdownPin = 14; // defines the hold pin (set High to trip the MOSFET and power down).
const int tiltPin = 13; //define the tilt sensor

// ----------------iot values--------------------------
String iotID = "GarageDoor";
const char* iotNotify = "0";

// ----------------host values--------------------------
const char *host = "**Your server side host**";

void goWifi(){
  WiFi.begin ( ssid, password );
  Serial.println ( "" );
  // Wait for connection
  int i=0;
  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
    i++;
    if(i>60){return;}
  }
  Serial.println ( "" );
  Serial.print ( "Connected to " );
  Serial.println ( ssid );
  Serial.print ( "IP address: " );
  Serial.println ( WiFi.localIP() );
}

void report(String sndMSG ){
  Serial.print("connecting to ");
  Serial.println(host);

  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  int tries = 0;
  while (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    if(tries == 3){
      digitalWrite(shutdownPin, HIGH); //Give up and go home
      Serial.println("Giving up");
    }
    goWifi(); //if the connection fails assume it's because the module has lost it's connection and reestablish the connection
    tries++;
  }
  
  // We now create a URI for the request
    String url = "/iot/server.php?iotID=" + iotID + "&iotMSG=" + sndMSG + "&iotNotify=" + iotNotify  ;
    
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
}

void setup () {
  pinMode(shutdownPin, OUTPUT);
  pinMode(tiltPin, INPUT);
 
  digitalWrite(shutdownPin, HIGH); //ensure pin is high so that board doesn't immediately shutdown

  Serial.begin ( 74880 );
 
  delay(1000); //delay while things get settled

  if(WiFi.status() != WL_CONNECTED){
    goWifi();
  }
  
  if(digitalRead(tiltPin) == 0){
    Serial.println ( "The garage door is closed" );
    report("The+garage+door+is+closed");
  }else{
    Serial.println ( "The garage door is open" );
    report("The+garage+door+is+open");
  }

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

