
//Index html webpage
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
  <title>Captive Portal Demo</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
  table, th, td {
  border: 1px solid black;
  }
  th, td {
  padding: 15px;
  }
  td:nth-child(even), th:nth-child(even) {
  background-color: #D6EEEE;
  }
</style>
  </head><body><center>
  <h3>Configuration ESP32 - Captive Portal Demo</h3>
  <br><br>
  <table><tr><th>Variable</th><th>Value </th></tr>
  <tr><td>   TimeMeasSecond:
  </td><td>  %TIMEMEASSECOND% 
  </td></tr></table>
  <br><br>
  Access to <a href="/webserial"> Webserial</a/> for debugging and changing variables.
  <br><br>
  <small>Prof. Ruben Mercade Prieto - 2022</small>
</body></html>)rawliteral";

// Replaces placeholder with variable values
//https://randomnerdtutorials.com/esp32-dht11-dht22-temperature-humidity-web-server-arduino-ide/
String processor(const String& var) {
  //Serial.println(var);
  if (var == "TIMEMEASSECOND") {
    return String(TimeMeasSecond);
  }
  //  else if(var == "HUMIDITY"){
  //    return readDHTHumidity();
  //  }
  return String();
}

class CaptiveRequestHandler : public AsyncWebHandler {
  public:
    CaptiveRequestHandler() {}
    virtual ~CaptiveRequestHandler() {}

    bool canHandle(AsyncWebServerRequest *request) {
      //request->addInterestingHeader("ANY");
      return true;
    }
    void handleRequest(AsyncWebServerRequest *request) {
      request->redirect("/");
      //      request->redirect("/webserial");
    }
};


void parseWebSerialData(char* tempChars) {      // split the data into its parts
  char * strtokIndx; // this is used by strtok() as an index

  strtokIndx = strtok(tempChars, ",");     // get the first part - the string
  strcpy(messageFromPC, strtokIndx); // copy it to messageFromPC
  Serial.print("Variable "); Serial.println(messageFromPC);

  strtokIndx = strtok(NULL, ","); // this continues where the previous call left off
  //  Serial.print("Integer "); Serial.println(strtokIndx);
  if (strtokIndx != NULL) {
    integerFromPC = atoi(strtokIndx);     // convert this part to an integer
    Serial.print("Integer "); Serial.println(integerFromPC);
  }
  else {
    Serial.println("No value recieved for variable (strtokIndx was NULL)");
  }
}

void printAcceptedVariablesWebSerial() {
  WebSerial.println("Enter data in this style: TimeMeasSecond,12");
  WebSerial.println("Accepted variables are: TimeMeasSecond and TimeMeasMin");
}

void understandWebSerialData() {  //comparing char with string
  if (strcmp(messageFromPC, "TimeMeasSecond") == 0) {
    Serial.println("TimeMeasSecond recieved correctly");
    TimeMeasSecond = integerFromPC;
    Serial.print("New TimeMeasSecond: ");  Serial.println(TimeMeasSecond);
    WebSerial.print("New TimeMeasSecond: ");  WebSerial.println(TimeMeasSecond);
    // Store the variable to the Preferences
    preferences.begin("ESP32Config", false);
    preferences.putInt("TimeMeasSecond", TimeMeasSecond);
    preferences.end();
  }
  else if (strcmp(messageFromPC, "TimeMeasMin") == 0) {
    Serial.println("TimeMeasMin recieved correctly");
    TimeMeasSecond = integerFromPC * 60;
    Serial.print("New TimeMeasSecond: ");  Serial.println(TimeMeasSecond);
    WebSerial.print("New TimeMeasSecond: ");  WebSerial.println(TimeMeasSecond);
    preferences.begin("ESP32Config", false);
    preferences.putInt("TimeMeasSecond", TimeMeasSecond);
    preferences.end();
  }
  else {
    Serial.println("Recieved an invalid variable");
    WebSerial.println("Recieved an invalid variable");
    printAcceptedVariablesWebSerial();
  }
}

/* Message callback of WebSerial */
void recvMsg(uint8_t *data, size_t len) {
  WebSerial.print("Received Data...   ");
  String d = "";
  for (int i = 0; i < len; i++) {
    d += char(data[i]);
  }
  WebSerial.println(d);
  Serial.print("Received Data: "); Serial.println(d);

  //control the bulitin LED light of Firebeetle
  if (d == "ON") {
    digitalWrite(LED_BUILTIN, HIGH);
  }
  if (d == "OFF") {
    digitalWrite(LED_BUILTIN, LOW);
  }

  char tempChars[len + 1];
  d.toCharArray(tempChars, len + 1);
  Serial.print("tempChars: "); Serial.println(tempChars);
  parseWebSerialData(tempChars);
  understandWebSerialData();
}
