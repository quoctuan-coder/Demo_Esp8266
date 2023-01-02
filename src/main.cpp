#include <Arduino.h>

// pass wifi: 09766114991

// Fill-in information from your Blynk Template here
#define BLYNK_TEMPLATE_ID "TMPLKb6uRi3a"
#define BLYNK_DEVICE_NAME "HOME"
#define BLYNK_AUTH_TOKEN "nVpMFO1QFKz1im4a5v-OgDxIBr13PrEo"

#define BLYNK_FIRMWARE_VERSION        "0.1.1"

#define BLYNK_PRINT Serial
//#define BLYNK_DEBUG

#define APP_DEBUG
// #include "DHT.h"
#define DHTPIN 1
#define DHTTYPE DHT22
// DHT dht(DHTPIN, DHTTYPE);
#define led_1 D7
#define led_2 D4
#define led_3 D8


boolean bt_state=HIGH;
#include "BlynkEdgent.h"

void setup()
{
  Serial.begin(115200);
  delay(100);

  pinMode(led_1, OUTPUT);
  pinMode(led_2, OUTPUT);
  pinMode(led_3, OUTPUT);


  // dht.begin();
  BlynkEdgent.begin();
}

BLYNK_WRITE(V2)
{
  int pinLED_1 = param.asInt();
  digitalWrite(led_1, pinLED_1);
}

BLYNK_WRITE(V3)
{
  int pinLED_2 = param.asInt();
  digitalWrite(led_2, pinLED_2);
}

BLYNK_WRITE(V5)
{
  int pinLED_2 = param.asInt();
  digitalWrite(led_3, pinLED_2);
}

void loop() {
  BlynkEdgent.run();

    // float t = dht.readTemperature();
    // float h = dht.readHumidity();
    // if (isnan(h) || isnan(t)) {
    //   Serial.println(F("Failed to read from DHT sensor!"));
    //   return;
    // }
    // Blynk.virtualWrite(V1,t);
    // Blynk.virtualWrite(V0,h);
    Serial.print(F("Temperature: "));
    // Serial.print(t);
    Serial.print(F("°C "));
    Serial.print(F("Humidity: "));
    // Serial.print(h);
    Serial.println(F("%"));
    delay(1000);
    
}
