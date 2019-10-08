// ESP DMX Lib
#include <ESPDMX.h>

// ESP Alexa Libs
#include <ESP8266WiFi.h>
//#define ESPALEXA_ASYNC
//#define ESPALEXA_DEBUG            //activate debug serial logging
#include <Espalexa.h>

// Max DMX Channels Being Used
#define maxDmx 300

// Wifi Settings
const char* ssid = "DMeXa";
const char* password = "FirstAlexaDMX!";


// Prototypes
boolean connectWifi();
boolean wifiConnected = false;

// Devices
EspalexaDevice* houseLights;
EspalexaDevice* workLights;
EspalexaDevice* z1;
EspalexaDevice* z2;
EspalexaDevice* z3;
EspalexaDevice* z4;
EspalexaDevice* z5;
EspalexaDevice* z6;

// ESPAlexa Object
Espalexa espalexa;

// DMX Object
DMXESPSerial dmx;

/******************SETUP**********************/
void setup() {
  // Begin Serial (we can do this because the MAX485 is on Serial1)
  Serial.begin(9600);
  
  // Initialise wifi connection
  wifiConnected = connectWifi();
  
  if(wifiConnected){
    //ESP Alexa Device Config
    houseLights = new EspalexaDevice("House Lights", changeHouseLights);
    workLights = new EspalexaDevice("Work Lights", changeWorkLights);
    z1 = new EspalexaDevice("Zone 1", changeWorkLights);
    z2 = new EspalexaDevice("Zone 2", changeWorkLights);
    z3 = new EspalexaDevice("Zone 3", changeWorkLights);
    z4 = new EspalexaDevice("Zone 4", changeWorkLights);
    z5 = new EspalexaDevice("Zone 5", changeWorkLights);
    z6 = new EspalexaDevice("Zone 6", changeWorkLights);

    espalexa.addDevice(houseLights);
    espalexa.addDevice(workLights);
    espalexa.addDevice(z1);
    espalexa.addDevice(z2);
    espalexa.addDevice(z3);
    espalexa.addDevice(z4);
    espalexa.addDevice(z5);
    espalexa.addDevice(z6);
    
    espalexa.begin();
    
  } else {
    while (1) {
      // Serial.println("Cannot connect to WiFi. Please check data and reset the ESP.");
      delay(2500);
    }
  }

  // Setup DMX Universe!
  dmx.init(maxDmx);
  Serial.println("Initialized DMX!");
}

/**************** MAIN LOOP ******************/
void loop() {
   espalexa.loop();
   delay(1);
   dmxHouseLights();
   dmxWorkLights();
   zone1();
   zone2();
   zone3();
   zone4();
   zone5();
   zone6();

  //Update DMX
   dmx.update();
}
/*********** LIGHTING FUNCTIONS****************/

void dmxHouseLights() {
  uint8_t val = houseLights->getValue(); // Brightness out of 255, convienent for DMX!
  Serial.print("Setting DMX House To: ");
  Serial.println(val);
  // Start Writing to house lights
  dmx.write(195, val);
  dmx.write(196, val);
  dmx.write(197, val);
  dmx.write(198, val);
  dmx.write(199, val);
  dmx.write(200, val);
  dmx.write(201, val);
  dmx.write(202, val);
  dmx.write(203, val);
  dmx.write(204, val);
  dmx.write(205, val);
  dmx.write(206, val);
  dmx.write(207, val);
  dmx.write(208, val);
  dmx.write(209, val);
  dmx.write(210, val);
  dmx.write(213, val);
  dmx.write(214, val);
}

void dmxWorkLights() {
  uint8_t valW = workLights->getValue(); // Brightness out of 255, convienent for DMX!
  Serial.print("Setting DMX Works To: ");
  Serial.println(valW);
  // Start Writing to work lights
  dmx.write(166, valW);
  dmx.write(167, valW);
  dmx.write(168, valW);
  dmx.write(170, valW);
  dmx.write(171, valW);
  dmx.write(172, valW);
  dmx.write(181, valW);
  dmx.write(282, valW);
}

void zone1() {
  uint8_t val = z1->getValue(); // Brightness out of 255, convienent for DMX!
  Serial.print("Setting DMX Z1 To: ");
  Serial.println(val);
  // Start Writing to work lights
  dmx.write(29, val);
  dmx.write(36, val);
  dmx.write(39, val);
  dmx.write(22, val);
}

void zone2() {
  uint8_t val = z2->getValue(); // Brightness out of 255, convienent for DMX!
  Serial.print("Setting DMX Z2 To: ");
  Serial.println(val);
  // Start Writing to work lights
  dmx.write(31, val);
  dmx.write(41, val);
  dmx.write(42, val);
  dmx.write(24, val);
}

void zone3() {
  uint8_t val = z3->getValue(); // Brightness out of 255, convienent for DMX!
  Serial.print("Setting DMX Z3 To: ");
  Serial.println(val);
  // Start Writing to work lights
  dmx.write(17, val);
  dmx.write(19, val);
  dmx.write(26, val);
  dmx.write(33, val);
}

void zone4() {
  uint8_t val = z4->getValue(); // Brightness out of 255, convienent for DMX!
  Serial.print("Setting DMX Z4 To: ");
  Serial.println(val);
  // Start Writing to work lights
  dmx.write(23, val);
  dmx.write(30, val);
  dmx.write(37, val);
  dmx.write(40, val);
}

void zone5() {
  uint8_t val = z5->getValue(); // Brightness out of 255, convienent for DMX!
  Serial.print("Setting DMX Z5 To: ");
  Serial.println(val);
  // Start Writing to work lights
  dmx.write(32, val);
  dmx.write(13, val);
  dmx.write(16, val);
  dmx.write(25, val);
}

void zone6() {
  uint8_t val = z6->getValue(); // Brightness out of 255, convienent for DMX!
  Serial.print("Setting DMX Z6 To: ");
  Serial.println(val);
  // Start Writing to work lights
  dmx.write(34, val);
  dmx.write(18, val);
  dmx.write(20, val);
  dmx.write(27, val);
}

/*********** CALBACK FUNCTIONS****************/
void changeHouseLights(uint8_t br, uint32_t rgb) {}

void changeWorkLights(uint8_t b, uint32_t rgb) {}

/*************** WIFI STUFF **********************/
// connect to wifi – returns true if successful or false if not
boolean connectWifi(){
  boolean state = true;
  int i = 0;
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.println("Connecting to WiFi");

  // Wait for connection
  Serial.print("Connecting...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (i > 40){
      state = false; break;
    }
    i++;
  }
  Serial.println("");
  if (state){
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }
  else {
    Serial.println("Connection failed.");
  }
  return state;
}
