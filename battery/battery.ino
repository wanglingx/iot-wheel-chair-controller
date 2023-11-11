#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#define PIN_RELAY_1  5 // the Arduino pin, which connects to the IN1 pin of relay module
#define PIN_RELAY_2  4// the Arduino pin, which connects to the IN2 pin of relay module
#define PIN_RELAY_3  3// the Arduino pin, which connects to the IN3 pin of relay module
#define PIN_RELAY_4  2 // the Arduino pin, which connects to the IN4 pin of relay module

#define WLAN_SSID       "xxxxxxxx"      // Your SSID
#define WLAN_PASS       "xxxxxx"        // Your password

#define AIO_SERVER      "io.adafruit.com" //Adafruit Server
#define AIO_SERVERPORT  1883                   
#define AIO_USERNAME    "xxxxx"           // Username
#define AIO_KEY         "xxxxxxxx"   // Auth Key

WiFiClient client;

Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
Adafruit_MQTT_Subscribe Battery = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME"/feeds/Battery"); 

void MQTT_connect();
void setup() {
  Serial.begin(9600);
  pinMode(PIN_RELAY_1, OUTPUT);
  pinMode(PIN_RELAY_2, OUTPUT);
  pinMode(PIN_RELAY_3, OUTPUT);
  pinMode(PIN_RELAY_4, OUTPUT);

  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); 
  Serial.println(WiFi.localIP());
 
  mqtt.subscribe(&Battery);
  
}

// the loop function runs over and over again forever
void loop() {
  Serial.println("Turn on all");
  digitalWrite(PIN_RELAY_1, HIGH);

  Serial.println("Turn off all");
  digitalWrite(PIN_RELAY_1, LOW);

  MQTT_connect();
  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(20000))) {
    if (subscription == &Battery) {
      Serial.print(F("Got: "));
      Serial.println((char *)Battery.lastread);
      int Battery_State = atoi((char *)Battery.lastread);
      digitalWrite(PIN_RELAY_1, Battery_State);      
    }
  }
  
}

void MQTT_connect() {
  int8_t ret;

  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  
  while ((ret = mqtt.connect()) != 0) {
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000); 
    retries--;
    if (retries == 0) {
      while (1);
    }
  }
  Serial.println("MQTT Connected!");
  
}
