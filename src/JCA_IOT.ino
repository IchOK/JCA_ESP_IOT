#define _DEBUG_ON

#include "IPAddress.h"
#include "painlessMesh.h"

#ifdef ESP8266
#include "Hash.h"
#include <ESPAsyncTCP.h>
#else
#include <AsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>
//#include <SPIFFSEditor.h>

#include <FS.h>
#define CODE_FILE "/Code.txt"
#define CONFIG_FILE "/Config.json"

#include <JCA_IOT_IO_All.h>
#include <JCA_IOT_CODE.h>

using namespace JCA::IOT;

//--------------------------------------------------------------------------
// Code-Interpreter
//--------------------------------------------------------------------------
//  Prototypes
void initCode();
void runCode();
//  Handler
cCode Runner;

//--------------------------------------------------------------------------
// Mesh
//--------------------------------------------------------------------------
//  Prototypes
void initMesh();
void runMesh();
void receivedCallback( const uint32_t &from, const String &msg );
IPAddress getLocalIP();
//  Handler
painlessMesh  mesh;
#ifdef _DEBUG_ON
  IPAddress myIP(0,0,0,0);
  IPAddress myAPIP(0,0,0,0);
#endif
#define   DEF_MESH_PREFIX     "IchBinNeuHier"
#define   DEF_MESH_PASSWORD   "IchBinNeuHier"
#define   DEF_MESH_PORT       5555
#define   DEF_MESH_CHANNEL    6
#define   DEF_HOSTNAME        "DerNeue"

//--------------------------------------------------------------------------
// Scheduler
//--------------------------------------------------------------------------
//  Prototypes

//  Handler
Scheduler oScheduler;
Task taskCode(TASK_MILLISECOND * 100, TASK_FOREVER, &runCode );
Task taskNetwork(TASK_MILLISECOND * 1000, TASK_FOREVER, &runMesh );

//--------------------------------------------------------------------------
// Webserver
//--------------------------------------------------------------------------
//  Prototypes
void initWebserver();
String getTemplates(const String& var);
//  Handler
AsyncWebServer server(80);
File fsUploadFile;

//--------------------------------------------------------------------------
// ArduinoOTA
//--------------------------------------------------------------------------
//  Prototypes
//void InitOTA();

//--------------------------------------------------------------------------
// Start Up
//--------------------------------------------------------------------------
void setup() {
  #ifdef _DEBUG_ON
    unsigned long mTime = micros();
    Serial.begin(74880);
  #endif
  
  //File-System starten
  SPIFFS.begin();

  //Tasks initialisieren
  oScheduler.addTask(taskCode);
  taskCode.enable();
  oScheduler.addTask(taskNetwork);
  taskNetwork.enable();
  
  //Mesh init
  initMesh();

  //IOT Initialisieren
  initCode();

  //Werbserver initialisieren
  initWebserver();
  
  #ifdef _DEBUG_ON
    Serial.print("Init Done in ");
    Serial.print(micros() - mTime);
    Serial.println("µs");
  #endif
}

//--------------------------------------------------------------------------
// Loop
//--------------------------------------------------------------------------
void loop() {
  oScheduler.execute();
}


