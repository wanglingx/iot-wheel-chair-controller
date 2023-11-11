#include <TridentTD_LineNotify.h>
#include <ESP8266WiFi.h>
// #define BLYNK_TEMPLATE_ID "TMPL6gboKWqF"
// #define BLYNK_DEVICE_NAME "wheelchair"
// #define BLYNK_FIRMWARE_VERSION        "0.1.0"
// #define BLYNK_PRINT Serial
// #define APP_DEBUG
// #define USE_NODE_MCU_BOARD
// #include "BlynkEdgent.h"
// #include "Adafruit_MQTT.h"
// #include "Adafruit_MQTT_Client.h"

#define SSID "xxxxxxxxxxx"
#define PASSWORD "xxxxxxxxxxx"
#define LINE_TOKEN "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"

// #define AIO_SERVER      "io.adafruit.com" 
// #define AIO_SERVERPORT  1883                   
// #define AIO_USERNAME    "xxxxxxx"          
// #define AIO_KEY         "xxxxxxxxxxxxxx"

int analogInPin  = A0;    // Analog input pin
int sensorValue;          // Analog Output of Sensor
float calibration = 1.4; // Check Battery voltage using multimeter & add/subtract the value
int battery_percentage;
float battery_maximum = 12.5;
float battery_cutoff = 1.0;
int max_value = 100;
int min_value = 0;
int before_value;

// WiFiClient client;

// Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
// Adafruit_MQTT_Subscribe Battery = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME"/feeds/Battery"); 

// void MQTT_connect();

//IpZHaGRVkLbBvl0Mf81c1Gmh72u58YFMSpXaAbCTpGc
//WiFiClient client;
 
void setup()
{
  Serial.begin(115200);
  Serial.println(LINE.getVersion());
  WiFi.begin(SSID, PASSWORD);
  Serial.printf("WiFi connecting to %s\n",  SSID);
    while(WiFi.status() != WL_CONNECTED) { 
        Serial.println("Connecting...  ");  
        Serial.printf("Connection Status: %d\n", WiFi.status()); 
        delay(1000);
    }
      Serial.printf("\nWiFi connected\nIP : ");
      Serial.println(WiFi.localIP());  
      // mqtt.subscribe(&Battery);
      //BlynkEdgent.begin();
      // กำหนด Line Token
      LINE.setToken(LINE_TOKEN);

      // ตัวอย่างส่งข้อความ
      LINE.notify("สวัสดีน้องๆ");

      // เลือก Line Sticker ได้จาก https://devdocs.line.me/files/sticker_list.pdf
      LINE.notifySticker(3,240);        // ส่ง Line Sticker ด้วย PackageID 3 , StickerID 240
      //LINE.notifySticker("Hello",1,2);  // ส่ง Line Sticker ด้วย PackageID 1 , StickerID 2  พร้อมข้อความ
}
 
void loop()
{
  //  MQTT_connect();
  //   Adafruit_MQTT_Subscribe *subscription;
  //   while ((subscription = mqtt.readSubscription(20000))) 
  //   {
  //     if (subscription == &Battery) {
  //       Serial.print(F("Got: "));
  //       Serial.println((char *)Battery.lastread);
  //       int Battery_State = atoi((char *)Battery.lastread);
  //       digitalWrite(battery_percentage, Battery_State);      
  //     }
  // }

  //BlynkEdgent.run();
  sensorValue = analogRead(analogInPin);
  float voltage = (((sensorValue * 3.3) / 1024) * 2 + calibration); 
  battery_percentage = mapfloat(voltage , battery_cutoff, battery_maximum, min_value , max_value); 
    if (battery_percentage >= 100){
        battery_percentage = 100;
    }

    if (battery_percentage <= 0){
        battery_percentage = 1;
    }

    if(battery_percentage == 100 && battery_percentage!=before_value && battery_percentage>before_value ){
       LINE.notify("Fully charging.... 100%");
    }
    else if(battery_percentage == 80 && battery_percentage!=before_value && battery_percentage>before_value){
      //count80++;
      LINE.notify("Already...battery 80%");
      //send notify
      //client.print(battery_percentage);
    }
    else if(battery_percentage == 20 && battery_percentage!=before_value && battery_percentage<before_value){
      //send minimum charging
      LINE.notify("แบตเหลือ 20% แล้วนะจ๊ะ ชาร์จด้วยจ้า");
      //client.print(battery_percentage);
    }
    else if(battery_percentage == 5 && battery_percentage!=before_value && battery_percentage<before_value){
      // send lowest battery
      LINE.notify("แบตเหลือ 5% จะดับแล้วนะ");
      //client.print(battery_percentage);
    }
    //Blynk.virtualWrite(A0, battery_percentage);
 
  before_value = battery_percentage;
  Serial.print("Analog Value = ");
  Serial.print(sensorValue);
  Serial.print("  Output Voltage = ");
  Serial.print(voltage);
  Serial.print("  Battery Percentage = ");
  Serial.println(battery_percentage);
  delay(1000);

}
 
float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
