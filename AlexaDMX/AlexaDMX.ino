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

/************ Channel Definitions *************/
int houseFixtures[] = {195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 213, 214};
int workFixtures[] = {166, 167, 168, 170, 171, 172, 181, 282};
int zone1Lights[] = {29, 36, 39, 22};
int zone2Lights[] = {31, 41, 42, 24};
int zone3Lights[] = {17, 19, 26, 33};
int zone4Lights[] = {23, 30, 37, 40};
int zone5Lights[] = {32, 13, 16, 25};
int zone6Lights[] = {34, 18, 20, 27};

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
   // Run Fixtures
   runAllZones();

  //Update DMX
   dmx.update();
}
/*********** LIGHTING FUNCTIONS****************/

void runAllZones(){
  runZone(houseLights, houseFixtures);
  runZone(workLights, workFixtures);
  runZone(z1, zone1Lights);
  runZone(z2, zone2Lights);
  runZone(z3, zone3Lights);
  runZone(z4, zone4Lights);
  runZone(z5, zone5Lights);
  runZone(z6, zone6Lights);
}

void runZone(EspalexaDevice* zoneDevice, int lights[]) {
  uint8_t val = zoneDevice->getValue(); // Brightness out of 255
  for(int i = 0; i < sizeof(lights); i++) {
    dmx.write(lights[i], val);
  }
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
