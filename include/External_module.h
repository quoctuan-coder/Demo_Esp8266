/**
 * Configure Led
*/
#define USE_LED
#ifdef USE_LED
  #define LED_1_PIN       D4 // Build-in Led
  #define LED_2_PIN       D7
#endif

/**
 * Configure Bluetooth
*/
// Blue HC_*
// HC-*   <->   Arduino (Host)
// Tx       <->   D2
// Rx       <->   D3

#define USE_BLUETOOTH
#ifdef USE_BLUETOOTH
  #define BLUE_TYPE_HC05 1
  #define BLUE_TYPE_HC06 0
#endif
#ifdef USE_BLUETOOTH
  #include <SoftwareSerial.h>
#endif
#if BLUE_TYPE_HC05
    SoftwareSerial Blue_Serial(D3, D2);
#endif
#if BLUE_TYPE_HC06
    SoftwareSerial Blue_Serial(D2, D3);
#endif

/**
 * Configure DTH
*/
#define USE_DTH
#ifdef USE_DTH
  #include <DHT.h>
#endif

#ifdef USE_DTH
  #define DTH_TYPE DHT11
  #define DHT_PIN   D5
#endif

/**
 * Configure Servo
*/
#define USE_SERVO
#ifdef USE_SERVO
  #include <Servo.h>
  #define SERVO_PIN D8
#endif


