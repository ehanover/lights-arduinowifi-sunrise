#include <WiFiNINA.h>
#include "wifi_secrets.h"
#include "html_page.h"

const char ssid[] = SECRET_SSID;
const char pass[] = SECRET_PASS;

int status = WL_IDLE_STATUS;      //connection status
WiFiServer server(80);            //server socket
WiFiClient client = server.available();


void setup() {
  Serial.begin(9600);
  while (!Serial);
  Serial.println("====================================");
  Serial.println("Serial connection established");

  enableWiFi();
  connectWiFi();

  server.begin();
  printWifiStatus();
}

void loop() {
  client = server.available();

  if (client) {
    printWEB();
  } else {
    Serial.println("looping");
    delay(200);
  }
}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");

  Serial.print("Serving on http://");
  Serial.println(ip);
}

void enableWiFi() {
  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < "1.0.0") {
    Serial.println("Please upgrade the firmware");
  }
}

void connectWiFi() {
  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait X seconds for connection:
    delay(5000);
  }
}

void printWEB() {
  if (!client) {
    return;
  }
  Serial.println("new client");

  String currentLine = "";       // make a String to hold incoming data from the client
  while (client.connected()) {   // loop while the client's connected
    if (client.available()) {    // if there's bytes to read from the client,
      char c = client.read();    // read a byte, then
      Serial.write(c);           // print it out the serial monitor
      if (c == '\n') {           // if the byte is a newline character

        // if the current line is blank, you got two newline characters in a row.
        // that's the end of the client HTTP request, so send a response:
        if (currentLine.length() == 0) {

          // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK) and a content-type so the client knows what's coming, then a blank line:
          client.println("HTTP/1.1 200 OK");
          client.println("Content-type:text/html");
          client.println();
          client.print(html_page);


          // The HTTP response ends with another blank line
          client.println();
          // break out of the while loop (disconnect from the client)
          break;
        } else {  // if you got a newline, then clear currentLine:
          currentLine = "";
        }
      } else if (c != '\r') {  // if you got anything else but a carriage return character, add it to the end of the currentLine
        currentLine += c;
      }

      if (currentLine.endsWith("GET /H")) {
        Serial.println("/H");
      } else if (currentLine.endsWith("GET /L")) {
        Serial.println("/H");
      } else {
        Serial.print("Line is ");
        Serial.println(currentLine);
      }
    }
  }

  // close the connection:
  client.stop();
  Serial.println("client disconnected");
}