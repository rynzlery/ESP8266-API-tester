/* HTTP GET Script */
/* https://stackoverflow.com/questions/41371156/esp8266-and-post-request */

#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <FirebaseArduino.h>
#include <ESP8266HTTPClient.h>

const char* ssid     = "SSID";
const char* password = "WIFIPASSWORD";
const String host    = "url";
#define FIREBASE_HOST "firebasehost.firebaseio.com"
#define FIREBASE_AUTH "firebasepassword"

void setup () {
  pinMode(LED_BUILTIN, OUTPUT);
  
  Serial.begin(115200);
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print("Connecting..");
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}
 
void loop() {
  Serial.println(host);

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(host);
    int httpCode = http.GET();
    Serial.println("Connection ...");
    
    if (httpCode > 0) {
      String payload = http.getString();
      Serial.println(payload);
      if (payload != "200") {
        digitalWrite(LED_BUILTIN, LOW);
      }
      else {
        digitalWrite(LED_BUILTIN, HIGH);
      }
      Firebase.pushString("logs", payload + " :: " + getTime());
    }else {
      Serial.println("FAILED TO GET DATA");
      digitalWrite(LED_BUILTIN, LOW);
      Firebase.pushString("logs", getTime());
    }
    http.end();
    delay(30000);
  }
}




String getTime() {
  WiFiClient client;
  while (!!!client.connect("google.com", 80)) {
    Serial.println("connection failed, retrying...");
  }

  client.print("HEAD / HTTP/1.1\r\n\r\n");
 
  while(!!!client.available()) {
     yield();
  }

  while(client.available()){
    if (client.read() == '\n') {    
      if (client.read() == 'D') {    
        if (client.read() == 'a') {    
          if (client.read() == 't') {    
            if (client.read() == 'e') {    
              if (client.read() == ':') {    
                client.read();
                String theDate = client.readStringUntil('\r');
                client.stop();
                return theDate;
              }
            }
          }
        }
      }
    }
  }
}
