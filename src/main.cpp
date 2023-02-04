#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <ESP8266HTTPUpdateServer.h>

ESP8266WebServer webServer(80);
DNSServer dnsServer;
ESP8266HTTPUpdateServer httpUpdater;

const char *ssid_station = "ICVN_Cashless";
const char *pass_station = "12345678";

const char *ssid_host = "458L1";
const char *pass_host = "09766114991";

const byte DNS_PORT = 53;

#define LED_STATUS_PIN      D7
#define SIGNAL_PIN          D0
#define CONFIG_AP_URL                 "iconvina.local"

#define WIFI_AP_IP                    IPAddress(192, 168, 4, 1)
#define WIFI_AP_Subnet                IPAddress(255, 255, 255, 0)

void signal_blink();
void mainpage();
void on_D1();
void off_D1();
void on_D2();
void off_D2();
void led_init();

//=========Biến chứa mã HTLM Website==//
const char MainPage[] PROGMEM = R"=====(
  <!DOCTYPE html> 
  <html>
   <head> 
       <title>HOME PAGE</title> 
       <style> 
          <!--body {text-align:center;}
          h1 {color:#003399;}
          a {text-decoration: none;color:#FFFFFF;}
          .bt_on {height:50px; width:100px; margin:10px 0;background-color:#FF6600;border-radius:5px;}
          .bt_off {height:50px; width:100px; margin:10px 0;background-color:#00FF00;border-radius:5px;}-->
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
      <h1>ESP8266 Web Server</h1> 
      <!-- <div>Trạng thái trên chân D1: <b>OFF</b></div>
      <div>
        <button class="bt_on"><a href="/onD1">ON</a></button>
        <button class="bt_off"><a href="/offD1">OFF</a></button>
      </div>
      <div>Trạng thái trên chân D2: <b>OFF</b></div>
      <div>
        <button class="bt_on"><a href="/onD2">ON</a></button>
        <button class="bt_off"><a href="/offD2">OFF</a></button>
      </div> -->
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
  delay(100);
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
  digitalWrite(SIGNAL_PIN,HIGH); // Start is HIGH

  

  //Setup connect to wifi
  WiFi.begin(ssid_host,pass_host); 

  // long timer_start = millis();
  // while(WiFi.status()!=WL_CONNECTED ){
  //   void led_init();
  //   // Serial.print("...");
  //   if (millis() - timer_start > 30000)
  //   {
  //     Serial.println("Connect to STA is failed");
  //     break;
  //   }
  // }

  // Set up DNS Server
  

  // if (WiFi.status()==WL_CONNECTED)
  // {
  //   digitalWrite(LED_STATUS_PIN, HIGH);
  //   IPAddress mySTAIP = WiFi.localIP();
  //   Serial.println("Connected: Mod STA");
  //   Serial.println(mySTAIP);
  //   dnsServer.start(DNS_PORT, CONFIG_AP_URL, mySTAIP);
  // }
  // else
  // {
    WiFi.mode(WIFI_OFF);
    delay(100);
    WiFi.mode(WIFI_AP_STA);
    digitalWrite(LED_STATUS_PIN, LOW);
    // Setup host wifi
    WiFi.softAPConfig(WIFI_AP_IP, WIFI_AP_IP, WIFI_AP_Subnet);
    WiFi.softAP(ssid_station, pass_station); // Setup wifi as hostpost
    delay(500);
    IPAddress myAPIP = WiFi.softAPIP();
    Serial.println("Connected: Mod AP");
    Serial.println(myAPIP);

    dnsServer.setTTL(300); // Time-to-live 300s
    dnsServer.setErrorReplyCode(DNSReplyCode::ServerFailure); // Return code for non-accessible domains
    dnsServer.start(DNS_PORT, CONFIG_AP_URL, myAPIP);
    
  // }

  Serial.println("AP URL:  " + String(CONFIG_AP_URL));

  httpUpdater.setup(&webServer, "/update");

  webServer.on("/",mainpage);
  webServer.on("/onD1",on_D1);
  webServer.on("/offD1",off_D1);
  webServer.on("/onD2",on_D2);
  webServer.on("/offD2",off_D2);
  webServer.on("/signal",signal_blink);

  webServer.begin();
}
void loop() {
  dnsServer.processNextRequest();
  webServer.handleClient();
}
//==========Chương trình con=================//

void signal_blink()
{
  String money = webServer.arg("money");
  int signal = money.toInt();
  
  Serial.println("Signal is called");
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
void on_D1(){
  digitalWrite(D1,LOW);
  String s = FPSTR(MainPage);
  webServer.send(200,"text/html",s);
}
void off_D1(){
  digitalWrite(D1,HIGH);
  String s = FPSTR(MainPage);
  webServer.send(200,"text/html",s);
}
void on_D2(){
  digitalWrite(D2,LOW);
  String s = FPSTR(MainPage);
  webServer.send(200,"text/html",s);
}
void off_D2(){
  digitalWrite(D2,HIGH);
  String s = FPSTR(MainPage);
  webServer.send(200,"text/html",s);
}

