#include <Arduino.h>
#include "External_module.h"

#define USE_BLYNK
#ifdef USE_BLYNK
  #define BLYNK_TEMPLATE_ID "TMPLKb6uRi3a"
  #define BLYNK_DEVICE_NAME "HOME"
  // #define BLYNK_AUTH_TOKEN "nVpMFO1QFKz1im4a5v-OgDxIBr13PrEo"
  #define BLYNK_FIRMWARE_VERSION        "0.1.0"
  #define BLYNK_PRINT Serial
  #define APP_DEBUG
  #include "BlynkEdgent.h"
#endif

#ifdef USE_BLYNK
BLYNK_WRITE(V1)
{
  int p = param.asInt();
  digitalWrite(LED_1_PIN, p);
  Serial.println("[INFO] Virtual Pin V1 is called");
}

BLYNK_WRITE(V2)
{
  int p = param.asInt();
  digitalWrite(LED_2_PIN, p);
  Serial.println("[INFO] Virtual Pin V2 is called");
}
#endif

#ifdef USE_DTH
  DHT dht(DHT_PIN, DTH_TYPE);
#endif

#ifdef USE_SERVO
  Servo servo;
#endif


void setup()
{
  Serial.begin(115200);

  #ifdef USE_BLUETOOTH
    Blue_Serial.begin(9600);
  #endif
  
  #ifdef USE_DTH
    dht.begin();
  #endif

  #ifdef USE_SERVO
    servo.attach(SERVO_PIN);
  #endif

  #ifdef USE_BLYNK
    BlynkEdgent.begin();
  #endif

  #ifdef USE_LED
    pinMode(LED_1_PIN,OUTPUT);
    pinMode(LED_2_PIN,OUTPUT);
  #endif

}

void loop() {
  #ifdef USE_BLYNK
  BlynkEdgent.run();
  #endif

  #ifdef USE_BLUETOOTH
  // Check bluetooth
  if (Blue_Serial.available() > 0)
  {
    char data = Blue_Serial.read();
    Serial.println(data);
  }
  #endif
  
}
