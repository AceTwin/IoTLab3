// the setup function runs once when you press reset or power the board

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include "ESP8266HTTPClient.h"
//#include <HttpClient.h>
#include "RestClient.h"
#define USE_SERIAL Serial
IPAddress server(192, 168, 4, 1); //IP of webserver

//Setup WiFi stuff
const char* ssid = "ssid"; //Enter Wifi ssid inbetween quotes
const char* password = "password"; //Enter Wifi password inbetween quotes

//Initialize light connections
int red = D5; //Red
int yellow = D3; //Yellow
int green = D2; //Green

void setup() {
  //Wifi stuff
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  USE_SERIAL.setDebugOutput(true);
  WiFi.begin(ssid, password);
  Serial.println("");
 
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500); //5 second wait
    Serial.print(".");
  }

  //Show connection details
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  // initialize digital pins as output
  pinMode(red, OUTPUT);
  pinMode(yellow, OUTPUT);
  pinMode(green, OUTPUT);

  //Turn off all LEDs initially
  digitalWrite(red, LOW);
  digitalWrite(yellow, LOW);
  digitalWrite(green, LOW);
}

// the loop function runs over and over again forever
void loop() {
  RestClient client = RestClient("192.168.1.21/state/",5000);
  String reply = "";
  int statusCode = client.get("/", &reply);
  Serial.println(statusCode);

  HTTPClient http;

  if(WiFi.status() == WL_CONNECTED){
    http.begin("192.168.1.21", 5000, "/state");
    http.addHeader("Content-Type", "text/html");
    int httpCode= http.GET();
    Serial.println(String(httpCode));
    if(httpCode == HTTP_CODE_OK) {
      Serial.print("HTTP response code ");
      Serial.println(httpCode);
      String response = http.getString();
      Serial.println(response);
     }
    Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    http.end();
  
    String payload;
    payload = http.getString();
    Serial.print(payload);
    //Get content off of website
    
    Serial.print("HTTP Response code:");
    Serial.println(String(httpCode));
    delay(2000);
    Serial.print("HTTP Response code:");
    Serial.println(String(httpCode));
/*    if (httpCode > 0) {
      payload = http.getString();
      Serial.print("HTTP Response code:");
      Serial.println(String(httpCode));
      Serial.print("HTTP Response:");
      Serial.print(payload);
    }

    http.end(); //Close connection*/

  /*
  int len;
  String led;
 
  // Read all the lines of the reply from server and print them to Serial
    
    len = payload.length(); //Getting the length of the line
    led = &payload[len - 1]; //Read the position, find the state
  Serial.println(payload);
  Serial.println();
  Serial.println("This is the led:" + led);
  dostuff(led); //Call the light function

  Serial.println("<-- Headers complete -->");
  Serial.println();
  Serial.println("closing connection");
  */}
   delay (100);
}

void dostuff(String state){
  //state is red
  if (state == "1")
  {
  digitalWrite(red, HIGH);   // turn the LED on (HIGH is the voltage level)
  }                       
  
  //state is yellow
  if (state == "2")
  {
  digitalWrite(yellow, HIGH);
  }
   
  //state is green
  if (state == "3")
  {
  digitalWrite(green,HIGH);
  }

  //state is off
  if (state == "0")
  {
  //// turn the LEDs off by making the voltage LOW
  digitalWrite(red, LOW);    
  digitalWrite(yellow, LOW);
  digitalWrite(green, LOW);
  }

  if (state =="4") //Traffic Light Cycle
  {
      //Red
      digitalWrite(red, HIGH);     // red on
      delay(1000);                //wait
      digitalWrite(red, LOW);     //red off  

      //Yellow
      digitalWrite(yellow, HIGH); //yellow on
      delay(1000);     
      digitalWrite(yellow, LOW);  //yellow off

      //Green
      digitalWrite(green,HIGH);   //green on
      delay(1000);  
      digitalWrite(green, LOW);
  }

  Serial.print("Works");
}


//Reference: https://www.arduino.cc/en/Tutorial-0007/BlinkingLED
//Wifi Reference: http://cyaninfinite.com/tutorials/getting-started-with-the-wemos-d1-esp8266-wifi-board/#Running-a-Simple-Web-Server
//Read HTTP:  https://www.esp8266.com/viewtopic.php?f=32&t=6463
//https://techtutorialsx.com/2016/07/17/esp8266-http-get-requests/
