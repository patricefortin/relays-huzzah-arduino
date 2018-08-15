#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

const char* ssid = "MY_WIFI_SSID";
const char* password = "MY_WIFI_PASSWORD";

ESP8266WebServer server(80);

#define PIN_LEFT 14
#define PIN_RIGHT 16

String getHeaderStyles() {
  String buttonStyle = " .button {"
    " font: bold 40px Arial;"
    " text-decoration: none;"
    " background-color: #EEEEEE;"
    " color: #333333;"
    " padding: 6px 12px 6px 12px;"
    " border-top: 1px solid #CCCCCC;"
    " border-right: 1px solid #333333;"
    " border-bottom: 1px solid #333333;"
    " border-left: 1px solid #CCCCCC;"
    " display: block;"
    " margin: 200px;"
    " white-space: nowrap;"
    "}";
    
  return wrapInTag("style", buttonStyle);;
}

String wrapInTag(String tag, String str) {
  return "<" + tag + ">" + str + "</" + tag + ">";
}

String wrapInHtml(String str) {
  return "<html><head>" + getHeaderStyles() + "</head><body>" + str + "</body></html>";
}

String setHigh(uint8_t pin) {
  digitalWrite(pin, HIGH);
}

void setLow(uint8_t pin) {
  digitalWrite(pin, LOW);
}

void handleSetPins() {
  String target = "";
  String value = "";
  for (uint8_t i = 0; i < server.args(); i++) {
    if (server.argName(i) == "target") {
      target = server.arg(i);
    } 
    if (server.argName(i) == "value") { 
      value = server.arg(i);
    }
  }

  if (target == "left" || target == "right") {
    uint8_t pin;
    if (target == "left") pin = PIN_LEFT;
    if (target == "right") pin = PIN_RIGHT;
   
    if (value == "1" || value == "on" || value == "high") setHigh(pin);
    if (value == "0" || value == "off" || value == "low") setLow(pin);
  }
}

String renderButton(String target, String value, String label) {
  String style = " style='display: block; margin: 200px;'";
  String classes = " class='button'";
  
  String href = " href='/?target=" + target + "&value=" + value + "'";
  return "<a" + classes + style + href + ">" + label + "</a>";
}

String renderButtons() {
  String buttonLeftOn = renderButton("left", "1", "LEFT ON");
  String buttonLeftOff = renderButton("left", "0", "LEFT OFF");
  String buttonsLeft = wrapInTag("p", buttonLeftOn + " " + buttonLeftOff);
  
  String buttonRightOn = renderButton("right", "1", "RIGHT ON");
  String buttonRightOff = renderButton("right", "0", "RIGHT OFF");
  String buttonsRight = wrapInTag("p", buttonRightOn + " " + buttonRightOff);
  
  return wrapInTag("table", 
          wrapInTag("tr", 
              wrapInTag("td", buttonsLeft) 
            + wrapInTag("td", buttonsRight)
          ));
}

String renderJsonSuccess() {
  return "{\"status\":\"success\"}";
}

void handleRoot() {
  handleSetPins();
  String buttons = renderButtons();
  String html = wrapInHtml(buttons);
  server.send(200, "text/html", html);
}

void handleApi() {
  handleSetPins();
  String response = renderJsonSuccess();
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "application/json", response);
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void setupSerial() {
  Serial.begin(115200);
}

void setupPins() {
  Serial.println("Setup Pins");
  
  pinMode(PIN_LEFT, OUTPUT);
  pinMode(PIN_RIGHT, OUTPUT);
  
  digitalWrite(PIN_LEFT, LOW);
  digitalWrite(PIN_RIGHT, LOW);
}

void setupWiFi() {
  Serial.println("Setup WiFi");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password); 

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void setupHttpServer() {
  Serial.println("Setup HTTP Server");
  
  server.on("/", handleRoot);
  server.on("/api", handleApi);
  server.onNotFound(handleNotFound);
  server.begin();
  
  Serial.println("HTTP server started");  
}

void setup() {
  setupSerial();
  setupPins();
  setupWiFi();
  setupHttpServer();
}

void loop(void) {
  server.handleClient();
}

