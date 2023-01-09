#include <Arduino.h>
#include <Servo.h>
#include <SoftwareSerial.h>

// pass wifi: 09766114991

// Fill-in information from your Blynk Template here
#define BLYNK_TEMPLATE_ID "TMPLKb6uRi3a"
#define BLYNK_DEVICE_NAME "HOME"
// #define BLYNK_AUTH_TOKEN "nVpMFO1QFKz1im4a5v-OgDxIBr13PrEo"

#define BLYNK_FIRMWARE_VERSION        "0.1.0"

#define BLYNK_PRINT Serial

#define APP_DEBUG
// #include "DHT.h"

#define DHTPIN 1
#define DHTTYPE DHT22

// DHT dht(DHTPIN, DHTTYPE);
#define led_1 D7
#define led_2 D4
#define led_3 D8

#define SERVO       D2
#define BUTTON_SERVO D1

// HC-06    <->   Arduino (Host)
// Tx       <->   Rx
// Rx       <->   Tx

//  HC-05   <->   Arduino (Host)
// Tx       <->     Tx
// Rx       <->     Rx

SoftwareSerial btSerial(D2, D3); // (Rx-Pin,Tx-Pin) 

Servo my_servo;

bool trangthai_servo = HIGH;
boolean bt_state = HIGH;

#include "BlynkEdgent.h"


void setup()
{
  Serial.begin(115200);
  btSerial.begin(9600);

  delay(100);

  pinMode(led_1, OUTPUT);
  pinMode(led_2, OUTPUT);

  // Init servo
  // my_servo.attach(SERVO);

  // dht.begin();
  // BlynkEdgent.begin();

  Serial.print("Setup");

}

BLYNK_WRITE(V5){
  int p = param.asInt();
  if (p == 1)
  {
    // trangthai_servo = 1;
    my_servo.write(90);
    delay(200);

  }
  else if ( p == 0 )
  {
    // trangthai_servo = 0;
    my_servo.write(0);
    delay(200);

  }
  else{}

}

BLYNK_WRITE(V2)
{
  int pinLED_1 = param.asInt();
  digitalWrite(led_1, pinLED_1);
  Serial.println("Test");
}

BLYNK_WRITE(V3)
{
  int pinLED_2 = param.asInt();
  digitalWrite(led_2, pinLED_2);
}

void loop() {
  // BlynkEdgent.run();

  // Check bluetooth
  if (btSerial.available() > 0)
  {
    char data = btSerial.read();
    Serial.println(data);
  }
  
}
