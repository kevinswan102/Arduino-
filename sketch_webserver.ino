
//Kevin Swan
#include <ESP8266WiFi.h>  // This will include the WiFi library relative for the ESP8266 device.
#include <ESP8266mDNS.h> // Include the mDNS library
#include <ESP8266WebServer.h>  // Include the WebServer library

ESP8266WebServer server(80);  // Create a webserver object that listens for HTTP request on port 80
const char* ssid = "";  // The SSID (name) of the Wi-Fi network you want to connect to
const char* password = "";  // The password of the Wi-Fi network
const int ledPin= 2; //Led pin # being used

void handleRoot() {
  server.send(200, "text/html", "<form action=\"/LED\" method=\"POST\"><input type=\"submit\" value=\"Toggle LED\"></form>");   // Send HTTP status 200 (Ok) and add "Toggle LED" to button
}

void handleNotFound() {
  server.send(404, "text/plain", "404: Not found");   // Send HTTP status 404 when URI handler is not found}
}

void handleLED(){
  digitalWrite(ledPin,!digitalRead(ledPin));      // Toggle LED
  server.sendHeader("Location","/");        // Browser can go back to home page with this header 
  server.send(303);                         // Send it back to the browser 
}

void setup() {
  Serial.begin(115200);  // Start the Serial communication to send messages to the computer. 9600 is the default but some people don’t find it fast enough and have been OK using 115200.
  delay(10);    // wait 10 milliseconds before proceeding.
  Serial.println('\n'); // begin a new line on the console
  pinMode(ledPin, OUTPUT); //for toggling led
  digitalWrite(ledPin, LOW);
  
  WiFi.begin(ssid, password); // Connect to the network outlined by the fields at the beginning
  Serial.print("Connecting to "); // displays a console status message so we know that we sent the connect request
  Serial.print(ssid);   // tell us what SSID we are connecting to
  Serial.println(" ..."); 

int i = 0;  // We are going to use i as a counter to measure how quickly we can connect
  while (WiFi.status() != WL_CONNECTED) {
  // Wait for the Wi-Fi to connect.  The WiFi object has a function that we can use to query its status.  // There are a few statuses we can get, but we’re only concerned with connected.
    delay(1000);  //wait one second
    Serial.print(++i);  // print how many seconds have elapsed
    Serial.print(' ');  // print a space
    }

  Serial.println('\n');  // some formatting for our output
  Serial.println("Connection established!");  // we are connected!
  Serial.print("IP address:\t");  //what is our ip address?
  Serial.println(WiFi.localIP()); // Send the IP address of the ESP8266 to the computer

if (!MDNS.begin("esp8266")) {  // Start the mDNS responder for esp8266.local   
  Serial.println("Error setting up MDNS responder!");
}
  Serial.println("mDNS responder started"); // Our device has a name now!

//HTTP server start
  server.on("/", HTTP_GET, handleRoot);     // Calls handleRoot function if "/" is requested
  server.on("/LED", HTTP_POST, handleLED);  // Calls handleLED function if a POST request is made to URI "/LED"
  server.onNotFound(handleNotFound);        // When a client requests an unknown URI 
  server.begin(); // Actually start the server
  Serial.println("HTTP server started");
}

void loop(void) {  
 server.handleClient();
} 
