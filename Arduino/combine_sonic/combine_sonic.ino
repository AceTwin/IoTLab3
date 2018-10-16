//Combine test_ap and test_sonic

/* Server
*  We write the server first because we need its IP address
* Ref:https://arduino.stackexchange.com/questions/18176/peer-to-peer-communication
* https://arduino-esp8266.readthedocs.io/en/latest/esp8266wifi/server-examples.html
*/

#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>

const char* ssid = "echosound";       // ssid of server (Access Point (AP))
const char* password = "";        // password of server (Access Point (AP))
WiFiServer server(80);            //Service Port

int ledPin = 2; // GPIO2 of Server ESP8266
int Status = 0; // This is the state we are going to set for the client to read via HTML

//variables for Sonic  
  // defines pins numbers
  const int trigPin = D5;
  const int echoPin = D2;
  // defines variables
  long duration;
  int distance;

String prepareHtmlPage()  // This is the HTML page we will send
{
  String htmlPage =
     String("HTTP/1.1 200 OK\r\n") +
            "Content-Type: text/html\r\n" +
            "Connection: close\r\n" +  // the connection will be closed after completion of the response
           /* "Refresh: 5\r\n" +  // refresh the page automatically every 5 sec*/
            "\r\n" +
            "<!DOCTYPE HTML>" +
            "<html>" +
            String(Status) +
            "</html>" +
            "\r\n";
  return htmlPage;
}
String InfoPage()  // This is the HTML page we will send
{
  String htmlPage =
     String("HTTP/1.1 200 OK\r\n") +
            "Content-Type: text/html\r\n" +
            "Connection: close\r\n" +  // the connection will be closed after completion of the response
           /* "Refresh: 5\r\n" +  // refresh the page automatically every 5 sec*/
            "\r\n" +
            "<!DOCTYPE HTML>" +
            "<html>" +
            "This is an IOT website.  You are not using me properly!" +
            "</html>" +
            "\r\n";
  return htmlPage;
}
void setup() 
{
  //Sonic
    pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
    pinMode(echoPin, INPUT); // Sets the echoPin as an Input

//Server
    delay(1000);
    Serial.begin(115200);    // to use tools->serial monitor

    pinMode(ledPin, OUTPUT);   // set GPIO 2 as an output

    WiFi.mode(WIFI_AP_STA);  // Set WiFi to AP and station mode

    // Connect to the WiFi network
    Serial.println();
    Serial.print("Connecting to: "); Serial.println(ssid);
    WiFi.softAP(ssid, password);

    // Display the server address
    Serial.print("Connected, My address: ");
    Serial.print("http://");
    Serial.print(WiFi.softAPIP());
    Serial.println("/");

    // Tell the server to begin listening for incoming connections
    server.begin();
    Serial.println("Server listening for incoming connections");
}  

void loop() 
{
    
//** Sonic Code **//
    // Clears the trigPin
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(echoPin, HIGH);
    // Calculating the distance
    distance= duration*0.034/2;
    // Prints the distance on the Serial Monitor
    Serial.print("Distance: ");
    Serial.println(distance);

//** Server code ** //
    
    // Check if a client has connected
    WiFiClient client = server.available();
    if (!client) // if not available, return
    {
        return;
    }

    // Wait until the client sends some data
    Serial.println();
    Serial.println("Client connected");
    if(client.available())
    {
      while (client.connected())
      {
        // read line by line what the client (web browser) is requesting
        if (client.available())
        {
          String line = client.readStringUntil('\r');
          Serial.print(line);

          if (line.indexOf("/state") != -1) //Browse to /state

          {
            client.println(prepareHtmlPage());
            break;
          }
          if (line.length() == 1 && line[0] == '\n') // Browse to root page
          {
            client.println(InfoPage());
            break;
          }
        }
      }
    delay(1); // give the web browser time to receive the data


    //client.flush();
    client.stop();
    Serial.println("Client disconnected");
  }
}