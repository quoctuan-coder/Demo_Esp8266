#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <ESP8266HTTPUpdateServer.h>

ESP8266WebServer webServer(80);
DNSServer dnsServer;
ESP8266HTTPUpdateServer httpUpdater;

#define USE_AP_MODE
// #define USE_STA_MODE
#define USE_DNS

const char *ssid_station = "ICVN_Cashless";
const char *pass_station = "12345678";

const char *ssid_host = "458 L1";
const char *pass_host = "09766114991";

const byte DNS_PORT = 53;

#define LED_STATUS_PIN      D7
#define SIGNAL_PIN          D0
#define CONFIG_AP_URL                 "iconvina.local"

#define WIFI_AP_IP                    IPAddress(192, 168, 4, 1)
#define WIFI_AP_Subnet                IPAddress(255, 255, 255, 0)

void signal_blink();
void mainpage();
void led_init();


//=========Biến chứa mã HTLM Website==//
const char MainPage[] PROGMEM = R"=====(
  <!DOCTYPE html> 
  <html>
   <head> 
       <title>HOME PAGE</title> 
       <style> 
       body {
              background-color: #fcfcfc;
              box-sizing: border-box;
            }
            body, input {
              font-family: Roboto, sans-serif;
              font-weight: 400;
              font-size: 16px;
            }
            .centered {
              position: fixed;
              top: 50%;
              left: 50%;
              transform: translate(-50%, -50%);

              padding: 20px;
              background-color: #ccc;
              border-radius: 4px;
              }
              td { padding:0 0 0 5px; }
              label { white-space:nowrap; }
              input { width: 20em; }
              input[name="port"] { width: 5em; }
              input[type="submit"], img { margin: auto; display: block; width: 30%; }
       </style>
       <meta charset="UTF-8">
   </head>
   <body> 
      <h1>Web Local</h1> 
      <div class="centered">
        <form method="post" action="signal">
          <table>
          <tr><td><label for="money"> MONEY:</label></td>  <td><input type="text" name="money" placeholder="1,2,3..." length=64 required="required"></td></tr>
          </table><br/>
          <input type="submit" value="Send">
        </form>
      </div>

   </body> 
  </html>
)=====";

void led_init()
{
  digitalWrite(LED_STATUS_PIN,HIGH);
  delay(900);
  digitalWrite(LED_STATUS_PIN,LOW);
  delay(900);
}

//=========================================//
void setup() {
  Serial.begin(115200);
  Serial.print("Connecting");

  pinMode(LED_STATUS_PIN,OUTPUT);
  pinMode(SIGNAL_PIN,OUTPUT);

  digitalWrite(LED_STATUS_PIN,HIGH);
  digitalWrite(SIGNAL_PIN,HIGH);

  led_init();

  #ifdef USE_STA_MODE
  /* Station Mode*/
  WiFi.begin(ssid_host,pass_host); 
  delay(200);
  long timer_start = millis();
  
  while(WiFi.status()!=WL_CONNECTED ){
    void led_init();
    if (millis() - timer_start > 30000)
    {
      break;
    }
  }

  if (WiFi.status()==WL_CONNECTED)
  {
    digitalWrite(LED_STATUS_PIN, HIGH);
    IPAddress mySTAIP = WiFi.localIP();
    Serial.print("_Debug_ Successfully connected to ");
    Serial.println(ssid_host);
    Serial.print("_Debug_ Station IP:");
    Serial.println(mySTAIP);
    dnsServer.start(DNS_PORT, CONFIG_AP_URL, mySTAIP);
  }
  else
  {
    Serial.println("_Debug_ Connect to Station is FAILED");
  }
  #endif
  
  #ifdef USE_AP_MODE
  /* Access Point Mode */
  WiFi.mode(WIFI_OFF);
  delay(100);
  WiFi.mode(WIFI_AP_STA);
  digitalWrite(LED_STATUS_PIN, LOW);

  IPAddress myAPIP;
  WiFi.softAPConfig(WIFI_AP_IP, WIFI_AP_IP, WIFI_AP_Subnet);
  bool status = WiFi.softAP(ssid_station, pass_station);
  delay(100);

  if (status == true)
  {
    Serial.println("_Debug_ Access Point is Creadted");
    myAPIP = WiFi.softAPIP();
    Serial.print("_Debug_ Access point IP:");
    Serial.println(myAPIP);
  }
  else
  {
    Serial.println("_Debug_ Unable to Create Access Point");
  }
  #endif
  
  #ifdef USE_DNS
  /* Set up DNS Server */
  dnsServer.setTTL(300); // Time-to-live 300s
  dnsServer.setErrorReplyCode(DNSReplyCode::ServerFailure); // Return code for non-accessible domains
  dnsServer.start(DNS_PORT, CONFIG_AP_URL, myAPIP);
  #endif

  httpUpdater.setup(&webServer, "/update");
  webServer.on("/",mainpage);
  webServer.on("/signal",signal_blink);
  webServer.begin();
  
}
void loop() {
  #ifdef USE_DNS
  dnsServer.processNextRequest();
  #endif
  webServer.handleClient();
}
//==========Chương trình con=================//

void signal_blink()
{
  String money = webServer.arg("money");
  int signal = money.toInt();
  
  Serial.println("_Debug_ Signal is send");
  for (int i = 0; i < signal; i++)
  {
    digitalWrite(SIGNAL_PIN, LOW);
    digitalWrite(LED_STATUS_PIN, LOW);
    delay(50);
    digitalWrite(SIGNAL_PIN, HIGH);
    digitalWrite(LED_STATUS_PIN, HIGH);
    delay(100);
    
  } 
  String s = FPSTR(MainPage);
  webServer.send(200,"text/html",s);
}

void mainpage(){
  String s = FPSTR(MainPage);
  webServer.send(200,"text/html",s);
}
