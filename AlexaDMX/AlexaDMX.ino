// ESP DMX Lib
#include <ESPDMX.h>

// ESP Alexa Libs
#include <ESP8266WiFi.h>
#define ESPALEXA_ASYNC
#define ESPALEXA_DEBUG            //activate debug serial logging
#include <Espalexa.h>

// Max DMX Channels Being Used
#define maxDmx 255

// Wifi Settings
const char* ssid = "DMeXa";
const char* password = "FirstAlexaDMX!";


// Prototypes
boolean connectWifi();
boolean wifiConnected = false;

// Devices
EspalexaDevice* houseLights;
EspalexaDevice* workLights;

// ESPAlexa Object
Espalexa espalexa;

// DMX Object
DMXESPSerial dmx;

// Global Booleans
boolean updateHouse = false;
boolean updateWork = false;

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

    espalexa.addDevice(houseLights);
    espalexa.addDevice(workLights);
    
    espalexa.begin();
    
  } else {
    while (1) {
      // Serial.println("Cannot connect to WiFi. Please check data and reset the ESP.");
      delay(2500);
    }
  }

  // Setup DMX Universe!
  dmx.init(maxDmx);
}

/**************** MAIN LOOP ******************/
void loop() {
   espalexa.loop();
   delay(1);

  if(updateHouse) {
    uint8_t val = houseLights->getValue(); // Brightness out of 255, convienent for DMX!
    // Start Writing to house lights
    dmx.write(1, val);
  }
  if(updateWork) {
    uint8_t val = workLights->getValue(); // Brightness out of 255, convienent for DMX!
    // Start Writing to work lights
    dmx.write(201, val);
  }

  // Update the DMX Bus Once per loop
  if(updateHouse || updateWork) {
    dmx.update(); 
  }
  updateHouse = false;
  updateWork = false;
}

/*********** CALBACK FUNCTIONS****************/
void changeHouseLights(uint8_t br, uint32_t rgb) {
  updateHouse = true;
}

void changeWorkLights(uint8_t b, uint32_t rgb) {
  updateWork = false;
}

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
