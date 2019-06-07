/*---------------------------------------------------------------
 * Testprojekt: Gateway
 * Funktionen:
 * Verbindet das Mesh-Netzwerk mit einem allgemeinen WLAN.
 * Das Gateway kommuniziert mit dem WLAN über UDP und leitet
 * die Daten an das Mesh-Netzwerk weiter.
 * Der Datenaustausch über UDP erfolgt über JSON-Objekte,
 * Jeder Datensatz vom WLAN zur Bridge muss den key "type" enthalten,
 * dieser definiert die weitere Verwendung
 *---------------------------------------------------------------*/
// ONLY ESP32

#define NAMEDMESH 1
#define USE_TCPSERVER 0
#define USE_SCHEDULER 1
#define USE_NODERED 1
#define DEBUGLEVEL 7
// Debuglevels:
// 0 : keine Debugmeldungen
// 1 : Diagnose Meldungen
// 2 : 
// 3 : Startup Information
// 7 : Scheduler Prints
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

//######################################################################
//-- Scheduler --
//######################################################################
#if (USE_SCHEDULER == 1)
  Scheduler     userScheduler;
  Task taskSendAlive( TASK_SECOND*10, TASK_FOREVER, []() {
    DynamicJsonDocument jsonBuffer(1024);
    JsonObject msg = jsonBuffer.to<JsonObject>();
    String strTemp;
    IPAddress ipTemp;
    ipTemp = mesh.getStationIP();
    strTemp = ipTemp[0];
    strTemp += ".";
    strTemp += ipTemp[1];
    strTemp += ".";
    strTemp += ipTemp[2];
    strTemp += ".";
    strTemp += ipTemp[3];
    msg["StationIP"] = strTemp;
    ipTemp = mesh.getAPIP();
    strTemp = ipTemp[0];
    strTemp += ".";
    strTemp += ipTemp[1];
    strTemp += ".";
    strTemp += ipTemp[2];
    strTemp += ".";
    strTemp += ipTemp[3];
    msg["APIP"] = strTemp;
    strTemp = "";
    serializeJson(msg, strTemp);

    String to = "NodeRED";
    mesh.sendSingle(to, strTemp);
//    mesh.sendBroadcast(strTemp);
    
    #if (DEBUGLEVEL >= 7)
      serializeJson(msg, Serial);
      Serial.printf("\n");
    #endif
  });
#endif

//######################################################################
//-- WLAN --
//######################################################################
//#define   STATION_SSID      "JCAtest"
//#define   STATION_PASSWORD  "JCAtest2019"
#define   STATION_SSID      "Home Sweet Home 1"
#define   STATION_PASSWORD  "1591409202662413"
#define   STATION_PORT      5555
uint8_t   station_ip[4] =   {192,168,23,14}; // Node-RED
#define   HOSTNAME          "MeshBridge"

//######################################################################
//-- JSON --
//######################################################################
#include <ArduinoJson.h>
#define JDOC_SIZE 1024
//StaticJsonDocument<JDOC_SIZE> jDoc;

//######################################################################
//-- Async TCP --
//######################################################################
#if (USE_TCPSERVER == 1)
  // Server --------------------------------------------------------------
  AsyncServer tcpServer(10080);
  static void tcpSrvHandleNewClient( void *s, AsyncClient* c );
  // Client --------------------------------------------------------------
  static std::vector<AsyncClient*> clients;
  static void tcpCltHandleError(void* arg, AsyncClient* client, int8_t error);
  static void tcpCltHandleData(void* arg, AsyncClient* client, void *data, size_t len);
  static void tcpCltHandleDisconnect(void* arg, AsyncClient* client);
  static void tcpCltHandleTimeOut(void* arg, AsyncClient* client, uint32_t time);
#endif

//######################################################################
//-- SETUP --
//######################################################################
void setup() {
  uint16_t uiMeshDebug = 0;
  #if (DEBUGLEVEL >= 1)
    uiMeshDebug += ERROR;
  #endif
  #if (DEBUGLEVEL >= 3)
    uiMeshDebug += STARTUP;
  #endif
  #if (DEBUGLEVEL >= 7)
    uiMeshDebug += COMMUNICATION;
  #endif
  #if (DEBUGLEVEL >= 8)
    uiMeshDebug += CONNECTION;
  #endif
  
  #if (DEBUGLEVEL >= 1)
    Serial.begin(115200);
    mesh.setDebugMsgTypes( uiMeshDebug );
  #endif

  //Mesh ---------------------------------------------------------------
  #if (USE_SCHEDULER == 1)
    mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT, WIFI_AP_STA, MESH_CHANNEL );
  #else
    mesh.init( MESH_PREFIX, MESH_PASSWORD, MESH_PORT, WIFI_AP_STA, MESH_CHANNEL );
  #endif
  // Verbindung zum allgemein WLAN herstellen
  #if (USE_NODERED == 1)
    mesh.stationManual(STATION_SSID, STATION_PASSWORD, STATION_PORT, station_ip); //Mit Node-RED direkt verbinden
  #else
    mesh.stationManual(STATION_SSID, STATION_PASSWORD); //Nur mit WLAN verbinden
  #endif
  mesh.setHostname(HOSTNAME);
  #if (NAMEDMESH == 1)
    String nodeName = HOSTNAME;
    mesh.setName(nodeName);
  #endif
  // Mesh als Root definieren und im Netzwerk bekannt machen
  mesh.setRoot(true);
  mesh.setContainsRoot(true);
  // Callback für Daten aus dem Mesh-Netzwerk (aktuell: UDP Broadcast)
  #if (NAMEDMESH == 1)
    mesh.onReceive(&receivedCallbackMeshID);
  #endif
  mesh.onReceive(&receivedCallbackMeshName);
  #if (DEBUGLEVEL >= 1)
    Serial.print("My ST IP is ");
    Serial.println(mesh.getStationIP());
    Serial.print("My AP IP is ");
    Serial.println(mesh.getAPIP());
  #endif

  //TCP ----------------------------------------------------------------
  #if (USE_TCPSERVER == 1)
    tcpServer.onClient(&tcpSrvHandleNewClient, &tcpServer);
    tcpServer.setNoDelay(true);
    tcpServer.begin();
  #endif

  //SCHEDULER ----------------------------------------------------------
  #if (USE_SCHEDULER == 1)
    userScheduler.addTask(taskSendAlive);
    taskSendAlive.enable();
  #endif
}

//######################################################################
//-- LOOP --
//######################################################################
void loop() {
  #if (USE_SCHEDULER == 1)
    userScheduler.execute();
  #endif
  mesh.update();
}

//######################################################################
//-- Callback MESH --
//######################################################################
void receivedCallbackMeshID( uint32_t from, String &msg ) {
  #if (DEBUGLEVEL >= 1)
    Serial.print("Mesh Recv: Received from ");
    Serial.print(from);
    Serial.println(" :");
    Serial.println(msg);
  #endif
}
#if (NAMEDMESH == 1)
  void receivedCallbackMeshName( String &from, String &msg ) {
    #if (DEBUGLEVEL >= 8)
      Serial.print("Mesh Recv: Received from ");
      Serial.print(from);
      Serial.println(" :");
      Serial.println(msg);
    #endif
  }
  
  static void JCA_MeshNodeAddName(JsonObject &joMeshNode){
    //Namen ermitteln
    String szNodeName;
    uint32_t uiNodeID = joMeshNode["nodeId"].as<uint32_t>();
    if(mesh.getNodeId() == uiNodeID){
      #if (DEBUGLEVEL >= 8)
        Serial.print("  NodeName: ");
        Serial.println(mesh.getName());
      #endif
      joMeshNode["name"] = mesh.getName();
    }else if(mesh.getNameById(szNodeName, uiNodeID)){
      #if (DEBUGLEVEL >= 8)
        Serial.print("  NodeName: ");
        Serial.println(szNodeName);
      #endif
      joMeshNode["name"] = szNodeName;
    }
    //Prüfen ob subs vorhanden sind
    if(joMeshNode.containsKey("subs")){
      JsonArray jaMeshSubs = joMeshNode["subs"].as<JsonArray>();
      for (JsonObject joMeshSub : jaMeshSubs){
        JCA_MeshNodeAddName(joMeshSub);
      }
    }
  }
#endif

//######################################################################
//-- Callback TCP --
//######################################################################
#if (USE_TCPSERVER == 1)
  void tcpSrvHandleNewClient( void* arg, AsyncClient* client ){
    #if (DEBUGLEVEL >= 8)
      Serial.printf("\n new client has been connected to server, ip: %s", client->remoteIP().toString().c_str());
    #endif
    // register events
    client->onData(&tcpCltHandleData, NULL);
    client->onError(&tcpCltHandleError, NULL);
    client->onDisconnect(&tcpCltHandleDisconnect, NULL);
    client->onTimeout(&tcpCltHandleTimeOut, NULL);  
  }
  
  static void tcpCltHandleError(void* arg, AsyncClient* client, int8_t error) {
    #if (DEBUGLEVEL >= 8)
      Serial.printf("\n connection error %s from client %s \n", client->errorToString(error), client->remoteIP().toString().c_str());
    #endif
    ;
  }
  
  static void tcpCltHandleData(void* arg, AsyncClient* client, void *data, size_t len) {
    #if (DEBUGLEVEL >= 8)
      Serial.println("TCP Client HandleData");
      Serial.printf (" Heap %07i\r\n", ESP.getFreeHeap());
    #endif
    DynamicJsonDocument jDoc(JDOC_SIZE);
    JsonObject joRoot;
    char reply[JDOC_SIZE];
    #if (DEBUGLEVEL >= 8)
      Serial.println(" Json-Doc und char-Buffer erzeugt");
      Serial.printf ("  Heap %07i\r\n", ESP.getFreeHeap());
    #endif
    IPAddress StaIP = mesh.getStationIP();
    
    #if (DEBUGLEVEL >= 8)
      Serial.printf("\n data received from client %s [%i]\n", client->remoteIP().toString().c_str(), len);
      Serial.write((uint8_t*)data, len);
    #endif
  
    //Json decodieren
    DeserializationError error = deserializeJson(jDoc, (char*)data, len);
    if (!error){
      //Json Objekt erstellen
      joRoot = jDoc.as<JsonObject>();
    }
    
    //Antwort erstellen
    JsonObject joMsg = jDoc.createNestedObject("response");
    joMsg["ack"] = "NONE";
    // Hostinformationen hinzufügen
    JsonObject joHost = joMsg.createNestedObject("host");
    sprintf(reply, "%i.%i.%i.%i", StaIP[0], StaIP[1], StaIP[2], StaIP[3]);
    joHost["ip"] = reply;
    strcpy(reply, HOSTNAME);
    joHost["name"] = reply;
  
    if (error) {
      #if (DEBUGLEVEL >= 1)
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.c_str());
      #endif
      //Fehlerantwort erstellen
      joMsg["ack"] = "FAIL";
      joMsg["text"] = error.c_str();
      // ursprüngliche Nachricht anhängen zu Fehleranalyse
      joMsg["echo"] = data;
      
    }else{
      //Befehl auswerten
      if (joRoot.containsKey("type")){
        switch (joRoot["type"].as<uint16_t>()){
          case 1:{
              //Mesh-Topologie einlesen
              strcpy(reply, mesh.subConnectionJson(false).c_str());
              size_t replyLen = strlen(reply);
              #if (DEBUGLEVEL >= 8)
                Serial.println("MeshLayout");
                Serial.println(reply);
              #endif
              DynamicJsonDocument jDocMesh(1024 + replyLen);
              DeserializationError errorMesh = deserializeJson(jDocMesh, reply, replyLen);
              if (errorMesh) {
                //Fehlerantwort erstellen
                joMsg["ack"] = "FAIL";
                joMsg["text"] = "MeshTopology parse failed";
                // ursprüngliche Nachricht anhängen zu Fehleranalyse
                joMsg["echo"] = reply;
              }else{
                JsonObject joMeshNode = jDocMesh.as<JsonObject>();
                JCA_MeshNodeAddName(joMeshNode);
                joMsg["ack"] = "OK";
                joMsg["text"] = "MeshTopology";
                joMsg["data"] = joMeshNode;
              }
            }
            break;
          default:{
              //Fehlerantwort erstellen
              joMsg["ack"] = "FAIL";
              joMsg["text"] = "Type Unbekannt";
              // ursprüngliche Nachricht anhängen zu Fehleranalyse
              joMsg["echo"] = joRoot;
            }
            break;
        }
      }else{
        //Fehlerantwort erstellen
        joMsg["ack"] = "FAIL";
        joMsg["text"] = "Type nicht enthalten";
        // Hostinformationen hinzufügen
        JsonObject joHost = joMsg.createNestedObject("host");
        sprintf(reply, "%i.%i.%i.%i", StaIP[0], StaIP[1], StaIP[2], StaIP[3]);
        joHost["ip"] = reply;
        strcpy(reply, HOSTNAME);
        joHost["name"] = reply;
        // ursprüngliche Nachricht anhängen zu Fehleranalyse
        joMsg["echo"] = joRoot;
      }
    }
    if (client->canSend()){
      serializeJson(joMsg, reply, JDOC_SIZE);
      client->add(reply, strlen(reply));
      client->send();
    }
    jDoc.clear();
  }
  
  static void tcpCltHandleDisconnect(void* arg, AsyncClient* client) {
    #if (DEBUGLEVEL >= 8)
      Serial.printf("\n client %s disconnected \n", client->remoteIP().toString().c_str());
      Serial.printf(" Free Heap %07i", ESP.getFreeHeap());
    #endif
    client->close(true);
    client->free();
    delete client;
    #if (DEBUGLEVEL >= 8)
      Serial.println(" client deleted");
      Serial.printf(" Free Heap %07i", ESP.getFreeHeap());
    #endif
  }
  
  static void tcpCltHandleTimeOut(void* arg, AsyncClient* client, uint32_t time) {
    #if (DEBUGLEVEL >= 8)
      Serial.printf("\n client ACK timeout ip: %s \n", client->remoteIP().toString().c_str());
    #endif
    client->close(true);
    client->free();
    delete client;
  }
#endif
