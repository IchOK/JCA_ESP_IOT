//--------------------------------------------------------------------------
// Initialisierung des Mesh
//--------------------------------------------------------------------------
void initMesh(){
  File file = SPIFFS.open(CONFIG_FILE, "r");
  DynamicJsonBuffer jsonBuffer;
  String meshSSID = DEF_MESH_PREFIX;
  String meshPass = DEF_MESH_PASSWORD;
  uint16_t meshPort = DEF_MESH_PORT;
  uint8_t meshChannel = DEF_MESH_CHANNEL;
  char strHostname[30] = DEF_HOSTNAME;

  String wlanSSID;
  String wlanPass;
  bool wlanIO = false;

  if(file){
    #ifdef _DEBUG_ON
      Serial.println("File open erfolgreich");
    #endif
    JsonObject &root = jsonBuffer.parseObject(file);
    file.close();
    
    if(root.success()){
      #ifdef _DEBUG_ON
        Serial.println("JSON-Parse erfolgreich");
      #endif
      if(root["Mesh"]["SSID"].is<const char*>()){
        meshSSID = root["Mesh"]["SSID"].as<String>();
      }
      if(root["Mesh"]["Pass"].is<const char*>()){
        meshPass = root["Mesh"]["Pass"].as<String>();
      }
      if(root["Mesh"]["Port"].is<int>()){
        meshPort = root["Mesh"]["Port"].as<uint16_t>();
      }
      if(root["Mesh"]["Channel"].is<int>()){
        meshChannel = root["Mesh"]["Channel"].as<uint8_t>();
      }
  
      if(root["Hostname"].is<const char*>()){
        strlcpy(strHostname, root["Hostname"], sizeof(strHostname));
      }
  
      if(root["Station"]["SSID"].is<const char*>() && root["Station"]["Pass"].is<const char*>()){
        wlanIO = true;
        wlanSSID = root["Station"]["SSID"].as<String>();
        wlanPass = root["Station"]["Pass"].as<String>();
      }
    }
    #ifdef _DEBUG_ON
    else{
      Serial.println("JSON-Parse fehlgeschlagen");
    }
    #endif
  }
  #ifdef _DEBUG_ON
  else{
    Serial.print("File open fehlgeschlagen" );
    Serial.println(CONFIG_FILE);
  }
  #endif

  
  #ifdef _DEBUG_ON
    mesh.setDebugMsgTypes( ERROR | STARTUP | CONNECTION );  // set before init() so that you can see startup messages
  #else  
    mesh.setDebugMsgTypes(0);
  #endif
  mesh.init( meshSSID, meshPass, &oScheduler, meshPort, WIFI_AP_STA, meshChannel);
  #ifdef _DEBUG_ON
    Serial.print("Mesh Init: ");
    Serial.print(meshSSID);
    Serial.print(" | ");
    Serial.print(meshPass);
    Serial.print(" | ");
    Serial.print(meshPort);
    Serial.print(" | ");
    Serial.println(meshChannel);
  #endif
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnection);
  mesh.onDroppedConnection(&lostConnection);
  
  if(wlanIO){
    #ifdef _DEBUG_ON
      Serial.print("Mesh manStation: ");
      Serial.print(wlanSSID);
      Serial.print(" | ");
      Serial.println(wlanPass);
    #endif
    mesh.stationManual(wlanSSID, wlanPass);
  }
  mesh.setHostname(strHostname);
  
  meshHandler.helloAll();
  
  #ifdef _DEBUG_ON
    myAPIP = IPAddress(mesh.getAPIP());
    Serial.println("My AP IP is " + myAPIP.toString());
  #endif
}

void receivedCallback( const uint32_t from, const String &msg ) {
  #ifdef _DEBUG_ON
    Serial.printf("bridge: Received from %u msg=%s\n", from, msg.c_str());
  #endif
  meshHandler.receiveData(from, msg);
}

void newConnection(uint32_t nodeID){
  meshHandler.hello(nodeID);
}

void lostConnection(uint32_t nodeID){
  ;
}


IPAddress getLocalIP() {
  return IPAddress(mesh.getStationIP());
}

//--------------------------------------------------------------------------
// Task-Funktion für Mesh-IOs
//--------------------------------------------------------------------------
void runMesh(){
  static uint16_t loopCount;
  #ifdef _DEBUG_ON
    if(myIP != getLocalIP()){
      myIP = getLocalIP();
      Serial.println("My IP is " + myIP.toString());
    }
    if(myAPIP != IPAddress(mesh.getAPIP())){
      myAPIP = IPAddress(mesh.getAPIP());
      Serial.println("My AP IP is " + myAPIP.toString());
    }
  #endif

  meshHandler.update(micros());
  if(loopCount >= 10){
    meshHandler.checkStations();
    loopCount = 0;
  }
  loopCount++;
}


void addCodeToMesh(char* strStation, char* strTag, int iSetTime, int iIndex, char cRecv){
  meshHandler.addToList(strStation, strTag, iSetTime, iIndex, cRecv);
}

