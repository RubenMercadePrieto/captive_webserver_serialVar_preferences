// Remote Wifi AP debugging and configuration


//------------ DNS server for Captive Portal --------------
//https://makexyz.fun/esp32-captive-portal/
#include <ESPmDNS.h>
#include <DNSServer.h>
const byte DNS_PORT = 53;
DNSServer dnsServer;

//------------ Wife Access Point --------------
#include <WiFi.h>
//IPAddress apIP(192,168,4,1); //doesnt work on mobiles...
IPAddress apIP(8, 8, 4, 4);
IPAddress subnet(255, 255, 255, 0);

//------------ Webserver and Webserial --------------
// ESPAsyncWebServer is selected because WebSerial uses this webserver library, not my favorite
//https://randomnerdtutorials.com/esp32-webserial-library/
#include <AsyncTCP.h>
#include "ESPAsyncWebServer.h"
#include <WebSerial.h>
AsyncWebServer server(80);

//------------ Preferences --------------
//https://randomnerdtutorials.com/esp32-save-data-permanently-preferences/
#include <Preferences.h>
Preferences preferences;


const char* ssid = "FirebeetleESP32E"; // Your WiFi AP SSID
const char* password = "HolaHolaHola"; // Your WiFi Password

// variables to hold the parsed data
//const byte numChars = 32;
char messageFromPC[50] = {0};
int32_t integerFromPC = 0;
int32_t TimeMeasSecond = 0;

#include "Captive_Webserial_func.h"

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);

  WiFi.mode(WIFI_AP);
  //  WiFi.softAP("ESP32Captive");
  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(apIP, apIP, subnet);
  dnsServer.start(DNS_PORT, "*", WiFi.softAPIP());

  if (!MDNS.begin("makexyzfun")) {
    Serial.println("Error starting mDNS");
    return;
  }


  // WebSerial is accessible at "<IP Address>/webserial" in browser
  WebSerial.begin(&server);
  /* Attach Message Callback */
  WebSerial.msgCallback(recvMsg);

  //server index webpage showing the value of variables
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/html", index_html, processor);
  });

  //important, Captive addHandler after starting webserial
  server.addHandler(new CaptiveRequestHandler()).setFilter(ON_AP_FILTER);//only when requested from AP
  server.begin();
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
  Serial.println("This demo expects 2 pieces of data in webserial - variable name and an integer within < >");
  Serial.println("Enter data in this style: TimeMeasSecond,12");
  Serial.println("Accepted variables are: TimeMeasSecond and TimeMeasMin");
  Serial.println();

  // ---------- Config Preferences -----------
  preferences.begin("ESP32Config", false);

  // Remove all preferences under the opened namespace
  //preferences.clear();
  //Obtained all saved variables in Preferences
  TimeMeasSecond = preferences.getInt("TimeMeasSecond", 0);
  // Close the Preferences
  preferences.end();
  Serial.print("TimeMeasSecond from Preferences: ");   Serial.println(TimeMeasSecond);
}

void loop() {
  dnsServer.processNextRequest();
}
