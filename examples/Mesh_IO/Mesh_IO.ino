/*---------------------------------------------------------------
 * Testprojekt: io
 * Funktionen:
 * Verbindet das Mesh-Netzwerk mit einem allgemeinen WLAN.
 * Das Gateway kommuniziert mit dem WLAN über UDP und leitet
 * die Daten an das Mesh-Netzwerk weiter.
 * Der Datenaustausch über UDP erfolgt über JSON-Objekte,
 * Jeder Datensatz vom WLAN zur Bridge muss den key "type" enthalten,
 * dieser definiert die weitere Verwendung
 *---------------------------------------------------------------*/

#define NAMEDMESH 1
#define DEBUGLEVEL 0
// Debuglevels:
// 0 : keine Debugmeldungen
// 1 : Diagnose Meldungen
// 2 : 
// 3 : Startup Information
// ...
// 8 : Loop Prints
// 9 : alle Debugmeldungen

//######################################################################
//-- Mesh-Netzwerk --
//######################################################################
#if (NAMEDMESH == 1)
  #include  "namedMesh.h"
  namedMesh mesh;
#else
  #include  "painlessMesh.h"
  painlessMesh  mesh;
#endif
#define   MESH_PREFIX     "JCAmesh"
#define   MESH_PASSWORD   "T3f]xGX*J=QWDaZbv.v+M@3="
#define   MESH_PORT       5555
//        Der Mesh-Channel muss dem WLAN-Channel entsprechen
#define   MESH_CHANNEL    6
void receivedCallbackMeshID( uint32_t from, String &msg );
#if (NAMEDMESH == 1)
  void receivedCallbackMeshName( String &from, String &msg );
#endif
/*Task taskSendMessage( TASK_SECOND*30, TASK_FOREVER, []() {
    String msg = String("This is a message from: ") + nodeName + String(" for logNode");
    String to = "logNode";
    mesh.sendSingle(to, msg); 
}); // start with a one second interval*/

//######################################################################
//-- JSON --
//######################################################################
#include <ArduinoJson.h>
#define JDOC_SIZE 1024
//StaticJsonDocument<JDOC_SIZE> jDoc;

//######################################################################
//-- SETUP -- 
//######################################################################
void setup() {
  #if (DEBUGLEVEL >= 1)
    Serial.begin(115200);
    mesh.setDebugMsgTypes( ERROR | STARTUP | CONNECTION );
  #endif

  //Mesh ---------------------------------------------------------------
  mesh.init( MESH_PREFIX, MESH_PASSWORD, MESH_PORT, WIFI_AP_STA, MESH_CHANNEL );
  // Verbindung zum allgemein WLAN herstellen
  #if (NAMEDMESH == 1)
    String nodeName = String(ESP.getChipId());
    mesh.setName(nodeName);
  #endif
  // Root im Netzwerk vorhanden
  mesh.setContainsRoot(true);
  // Callback für Daten aus dem Mesh-Netzwerk (aktuell: UDP Broadcast)
  #if (NAMEDMESH == 1)
    mesh.onReceive(&receivedCallbackMeshID);
  #endif
  mesh.onReceive(&receivedCallbackMeshName);
  #if (DEBUGLEVEL >= 1)
    Serial.print("My AP IP is ");
    Serial.println(mesh.getAPIP());
  #endif

}

//######################################################################
//-- LOOP --
//######################################################################
void loop() {
  mesh.update();
}

//######################################################################
//-- Callback MESH --
//######################################################################
bool handleRequest(String &msgIn, String &msgOut){
    DynamicJsonDocument jDoc(JDOC_SIZE);
    JsonObject joRoot;
    char reply[JDOC_SIZE];
    #if (DEBUGLEVEL >= 8)
      Serial.println(msg);
      Serial.println(" Json-Doc erzeugt");
    #endif
  
    //Json decodieren
    DeserializationError error = deserializeJson(jDoc, msgIn);
    if (error){
      #if (DEBUGLEVEL >= 8)
        Serial.println(" Json-Doc FAILED");
      #endif
      return false;
    }

    //Json Objekt erstellen
    #if (DEBUGLEVEL >= 8)
      Serial.println(" Json-Doc DONE");
    #endif
    joRoot = jDoc.as<JsonObject>();

    if (joRoot.containsKey("Ident")){
      //Anforderung auswerten
      #if (DEBUGLEVEL >= 8)
        Serial.println("  Ident gefunden");
      #endif
      int i = joRoot["Ident"];
      switch(i){
        case 1://aktiviere OTA
          #if (DEBUGLEVEL >= 8)
            Serial.println("    OTA");
          #endif
          msgOut = "OTA was ist das";
          return true;
          break;
        default:
          #if (DEBUGLEVEL >= 8)
            Serial.println("    UNBEKANNT");
          #endif
          return false;
      }
    }else{
      #if (DEBUGLEVEL >= 8)
        Serial.println("  Ident NICHT gefunden");
      #endif
      return false;
    }
    
    
    //Antwort auswerten
  
}
void receivedCallbackMeshID( uint32_t from, String &msg ) {
  String msgOut;
  #if (DEBUGLEVEL >= 8)
    Serial.print("Mesh Recv: Received from ");
    Serial.print(from);
    Serial.println(" :");
  #endif
  if(handleRequest(msg, msgOut)){
    mesh.sendSingle(from , msgOut);
  }
}
#if (NAMEDMESH == 1)
  String msgOut;
  void receivedCallbackMeshName( String &from, String &msg ) {
    #if (DEBUGLEVEL >= 8)
      Serial.print("Mesh Recv: Received from ");
      Serial.print(from);
      Serial.println(" :");
    #endif
  if(handleRequest(msg, msgOut)){
    mesh.sendSingle(from , msgOut);
  }
  }
#endif
