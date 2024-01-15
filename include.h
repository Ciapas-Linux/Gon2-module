//#include <Hash.h>
#include "PCF8574.h"
#include <ESP8266WiFi.h>
#include <Pinger.h>
extern "C"
{
  #include <lwip/icmp.h> // needed for icmp packet definitions
}
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <WebSocketsServer.h>
#include <Base64.h>

// ASYNC UDP
#include "ESPAsyncUDP.h"
AsyncUDP udp;
unsigned int localPort = 18765;


// UDP SOCKETS:
//#include <WiFiUdp.h>
//char packetBuffer[UDP_TX_PACKET_MAX_SIZE]; 
//WiFiUDP Udp;
//bool udp_connected = false;

//#define TIMER_INTERRUPT_DEBUG         0
//#define _TIMERINTERRUPT_LOGLEVEL_     0

//// Select a Timer Clock
//#define USING_TIM_DIV1                true           // for shortest and most accurate timer
//#define USING_TIM_DIV16              false          // for medium time and medium accurate timer
//#define USING_TIM_DIV256            false          // for longest timer but least accurate. Default


//// hardware timers
//#include "ESP8266TimerInterrupt.h"


// ##########################################################
// FIREBASE:
// #include <ESP8266Firebase.h>
// #include <FirebaseArduino.h>
// #include <Firebase_ESP_Client.h>
// #define API_KEY "AIzaSyDPUDywgdhxX0xgx0pQ3OiqrOk-sgB-bA4"
// Insert RTDB URLefine the RTDB URL */
// #define DATABASE_URL "https://sterowanie-22-default-rtdb.europe-west1.firebasedatabase.app" 
// ###########################################################


//#include <FS.h>
#include "LittleFS.h"

// internet time
//#include "NTPClient.h"

//mdns:
//#include <ESP8266mDNS.h>

//cloud:
//#include <ArduinoIoTCloud.h>
//#include <Arduino_ConnectionHandler.h>

#include <ArduinoJson.h>

#include "OneWire.h" 
#include "Wire.h"            
#include "DallasTemperature.h"  
#include <LiquidCrystal_I2C.h>
#include <ESP_EEPROM.h>
#include <AceButton.h>
#include <time.h>

#include <vector>
#include <list>
#include <string>

//#include "StringSplitter.h"


// AUTOMATA Classes
#include "termostat.h"
#include "Automata.h"


#include "static/index.html.gz.h"
#include "static/upload.html.gz.h"
#include "static/graph.html.gz.h"
#include "static/react.html.gz.h"


//#define debug true

// Note: I’m using sizeof(buf) quite often to get the array length at compile-time. In this case it 
// works as expected due to the fact that buf is always of char type. A more proper solution is to use 
// sizeof(buf)/sizeof(buf[0]) - this way we get the correct result regardless of the data type.

//const long utcOffsetInSeconds = 3600;
//WiFiUDP ntpUDP;
//NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

FS *fileSystem = &LittleFS;
static bool fsOK;

const size_t MAX_FILESIZE = 1024 * 1024 * 10; //15728640   --> 540672
static const char* MIMETYPE_HTML{"text/html"};


// Set i2c address
PCF8574 pcf8574;
byte pcf8574_addr = 0;


// interrupt timer
//ESP8266Timer ITimer;


Timex gTimex;

Pinger pinger;

RBD::Timer timer_buzzer;
RBD::Timer timer_ZGON;


//Websocket Server
#define NODEBUG_WEBSOCKETS
WebSocketsServer webSocket = WebSocketsServer(81);


AutomataData gParametryPracy;
Automata gAutomat(fileSystem,&webSocket ,&pcf8574,&gParametryPracy,&gTimex, &timer_buzzer, &timer_ZGON);


using namespace ace_button;

// ------------------------------|.
// buzzer:     expander pin P4   |..
// ds18b20 1:  gpio2  D5		 |...
// ds18b20 2:  gpio2  D6		 |.... 	
// ds18b20 3:  gpio2  D7         |.....
// lcd scl:    gpio5  D1         |......
// lcd sda:    gpio4  D2         |.....
// button1:    gpio14 D0         |....
// button2:    gpio14 D4         |...
// zawor -     dioda: expander P5|..
// grzałka1 -  dioda: expander P6|.
// ------------------------------| 


//AceButton button;
void handleEvent1(AceButton*, uint8_t, uint8_t);
AdjustableButtonConfig adjustableButtonConfig1;
AceButton button1(&adjustableButtonConfig1);

void handleEvent2(AceButton*, uint8_t, uint8_t);
AdjustableButtonConfig adjustableButtonConfig2;
AceButton button2(&adjustableButtonConfig2);

extern "C"
{ 
	#include "user_interface.h"
    //#include "ets_sys.h"
    //#include "osapi.h"
    //#include "os_type.h"
}

OneWire ds_1(DS_ZBIORNIK); 
OneWire ds_2(DS_KOLUMNA);  
OneWire ds_3(DS_GLOWICA);  
DallasTemperature sensor_1(&ds_1);
DallasTemperature sensor_2(&ds_2);
DallasTemperature sensor_3(&ds_3);


//########################-------LCD
LiquidCrystal_I2C lcd(20, 4);   //0x3F
byte lcd_addr = 0;
uint8_t heart[8] = {0x0,0xa,0x1f,0x1f,0xe,0x4,0x0};
byte smiley[8] = {B00000,B10001,B00000,B00000,B10001,B01110,B00000};
byte char_ą[8] = {B01110,B00001,B00001,B01111,B10001,B10001,B01111,B01000};
byte invert[8] = {B11111,B11111,B11111,B11111,B11111,B11111,B11111,B11111};
byte char_ó[8] = {B00000,  B00010,  B00100,  B01110,  B10001,  B10001,  B10001,  B01110};
byte smile1[8]={B00000,B01010,B00000,B00000,B10001,B01110,B00000,}; //Smile-1
byte smile2[8]={B00000,B01010,B00100,B00100,B00000,B01110,B10001,}; //Smile-2
byte hglass[8]={B01010,B00100,B00100,B01010,B10001,B00100,B10001,}; //Hour-glass



String msg = "";
String str_msg = "";
AsyncWebServer server(80);

char TempCharBuffer[12] = "0000000000\0"; // "-2147483648\0"
bool gAP_CONNECTED = false;

// UDP Timer
bool TimerUDP_occured = false;
os_timer_t  myTimer_UDP;

// check wifi
bool TimerCheckWifi_occured = false;
os_timer_t myTimer_CheckWifi;

// update temperatures timer
bool TimerUpdTemp_occured = false;
os_timer_t  myTimer_UpdTemp;

// update LCD timer
bool TimerLCD_occured = false;
os_timer_t  myTimer_LCD;

// update Alarm zalania timer
bool TimerALARM_ZAL_occured = false;
os_timer_t  myTimer_ALARM_ZAL;


String XML;

uint8_t ip_blink_cnt = 0;
uint8_t temp_blink_cnt = 0;

// termostat
bool termo_run_lcd_once = false;

//LCD
byte lcd_wait = 0;


bool blinkTMP    = false;
bool blinkTDN    = false;
bool blinkTermostat    = false;
bool StartblinkTMP    = false;
bool StartblinkTDN    = false;
bool DS18b20_1 = false;
bool DS18b20_2 = false;
bool DS18b20_3 = false;
bool AWARIA_DS = false;
unsigned int AwariaDS_number = 0;
bool  Alarm_temperaturyB = false;
bool  Alarm_temperaturyG = false;
bool  disable_lcd_update = false;
bool  Display_AWARIA_DS = false;
uint8_t Display_AWARIA_DS_cntr = 0;
bool next_scan = false; 
//bool Do_power_off = false;



// MODULE Cntr:
long   module_cntr = 0;
bool module_live = false;

// router conection error counter:
int router_error_cntr = 0;
int ping_error_cntr = 0;


// FAKE:
byte *fake_ptr;



/* format bytes as KB, MB or GB string */
String humanReadableSize(const size_t bytes) {
    if (bytes < 1024) return String(bytes) + " B";
    else if (bytes < (1024 * 1024)) return String(bytes / 1024.0) + " KB";
    else if (bytes < (1024 * 1024 * 1024)) return String(bytes / 1024.0 / 1024.0) + " MB";
    else return String(bytes / 1024.0 / 1024.0 / 1024.0) + " GB";
}



