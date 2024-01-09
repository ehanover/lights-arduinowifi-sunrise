/*
https://www.arduino.cc/reference/en/libraries/arduino-timer/
https://www.arduino.cc/reference/en/libraries/wifinina/wifi.gettime/ (not needed because simple delay should be accurate enough)

https://www.htmlminifiers.com/html-minifier
https://htmlminifier.com/
https://www.toptal.com/developers/javascript-minifier

https://www.timeanddate.com/date/timeadd.html?type=add
*/

#include <WiFiNINA.h>
#include <Adafruit_NeoPixel.h>
#include <arduino-timer.h>
#include "wifi_secrets.h"
#include "html_page.h"

#define DEFAULT_MAX_BRIGHTNESS 150 // TODO adjust max brightness

#define LED_PIN 9
#define LED_COUNT 60
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

int status = WL_IDLE_STATUS; // connection status
WiFiServer server(80); // server socket
WiFiClient client = server.available();

unsigned char httpRequestType;
int postContentLength;
unsigned char postContent[8]; // TODO ensure sufficient size

Timer<2, millis> timer;
Timer<2, millis>::Task sunriseTask; // needs to be declared in this file because used here


void setup() {
  pinMode(LED_PIN, OUTPUT);
  strip.begin();
  strip.setBrightness(DEFAULT_MAX_BRIGHTNESS); // sets the "max" available brightness to max. Use this for power usage, eye comfort, etc only once
  strip.fill(rgbToColor(100, 0, 0));
  strip.show();

  Serial.begin(9600);
  while (!Serial);
  Serial.println("====================================");
  Serial.println("Serial connection established");

  enableWiFi();
  connectWiFi();
  server.begin();
  printWifiStatus();
  flash(rgbToColor(0, 100, 0), 1000, true);

  timer.every(300, serveClient); // start checking the client connection every X ms
  // postContent[0] = 3;
  // postContent[1] = 0; // colors
  // postContent[2] = 70;
  // postContent[3] = 180;
  // postContent[4] = 230; // speed
  // postContent[5] = 210;

  // postContent[0] = 4;
  // postContent[1] = 0;
  // postContent[2] = 250;
  // postContent[3] = 100;
  // setLightsFromPOST();
}

void loop() {
  // client = server.available();
  // serveClient();

  timer.tick();
  updateLights();

  delay(20);
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
    Serial.println("Communication with WiFi module failed! Halting.");
    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < "1.0.0") {
    Serial.println("Please upgrade the firmware. Proceeding.");
  }
}

void connectWiFi() {
  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(SECRET_SSID, SECRET_PASS);

    // wait X seconds for connection:
    delay(6000);
  }
}

void setLightsFromPOST() {
  if(sunriseTask) {
    timer.cancel(sunriseTask);
  }

  unsigned char b0 = postContent[0];
  if(b0 == 0) {
    strip.clear();
    strip.show();
  } else if(b0 == 1) {
    strip.setBrightness(DEFAULT_MAX_BRIGHTNESS);
    strip.fill(rgbToColor(postContent[1], postContent[2], postContent[3]));
    strip.show();
  } else if(b0 == 2) {
    sunriseStart();
  } else if(b0 == 3) {
    swirlStart();
  } else if(b0 == 4) {
    raveStart();
  } else if(b0 == 5) {
    // sirenStart();
  } else {
    Serial.println("ERROR: unrecognized POST content byte 0");
  }
}

void updateLights() {
  unsigned char b0 = postContent[0];
  if(b0 == 2) {
    // sunrise no-op
  } else if(b0 == 3) {
    swirlUpdate();
  } else if(b0 == 4) {
    raveUpdate();
  } else if(b0 == 5) {
    // sirenUpdate();
  } else {
    delay(500);
  }
}

void __attribute__((optimize("O2"))) handleGET() {
  // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK) and a content-type so the client knows what's coming, then a blank line:
  client.println("HTTP/1.1 200 OK");
  client.println("Content-type: text/html");
  client.println("Cache-Control: public"); // I don't know if this does anything
  client.println();

  // client.print(html_page);
  for(int i=0; i<sizeof(html_page)/sizeof(char); i++) {
    // https://www.nongnu.org/avr-libc/user-manual/group__avr__pgmspace.html
    // TODO optimize this so the page loads faster - what's the bottleneck?
    char c = pgm_read_byte_near(html_page + i);
    client.print(c);
    // Serial.print(c);
  }

  client.println(); // The HTTP response ends with another blank line
}

void __attribute__((optimize("O2"))) handlePOST() {
  Serial.print("Got post chars=");
  for(int i=0; i<postContentLength; i++) {
    char c = client.read();
    postContent[i] = c;
    Serial.print((unsigned char)c);
    Serial.print(" ");
  }
  Serial.println();

  setLightsFromPOST();

  client.println("HTTP/1.1 200 OK");
  client.println("Content-type:text/html");
  client.println();
}

bool serveClient(void* args) {
  client = server.available();
  if (!client) {
    return true;
  }

  // TODO ensure connected to wifi here

  Serial.println("new request");
  httpRequestType = 0;
  postContentLength = 0;

  String currentLine = ""; // make a String to hold incoming data from the client
  while (client.connected()) { // loop while the client's connected
    if (client.available()) { // if there's bytes to read from the client,
      char c = client.read();
      // Serial.write(c);

      if (c == '\n') {
        // Serial.println(currentLine);

        // Check for keywords in the latest line, as completed lines come in
        if(currentLine.indexOf("GET / ") >= 0) {
          httpRequestType = 0;
          Serial.println("  (detected incoming GET)");
        } else if(currentLine.indexOf("POST") >= 0) {
          httpRequestType = 1;
          Serial.println("  (detected incoming POST)");
        } else if(currentLine.indexOf("Content-Length") >= 0) {
          postContentLength = currentLine.substring(16).toInt();
          Serial.print("  (detected content-length = ");
          Serial.print(postContentLength);
          Serial.println(")");
        }

        // if the current line is blank, you got two newline characters in a row - that's the end of the client HTTP request prep
        if (currentLine.length() == 0) {
          // handle the appropriate type of request
          if(httpRequestType == 0) {
            Serial.println("(handling GET)");
            handleGET();
          } else if(httpRequestType == 1) {
            Serial.println("(handling POST)");
            handlePOST();
          } else {
            Serial.println("(UNSUPPORTED REQUEST TYPE!)");
          }

          break; // disconnect
        } 
        
        // if you got a newline, then reset currentLine
        currentLine = "";

      } else if (c != '\r') {
        // if you got anything else but a carriage return character, add it to the end of the currentLine
        currentLine += c;
      }
    }
  }

  client.stop(); // close the connection
  Serial.println("client disconnected");
  return true; // to repeat the action - false to stop
}