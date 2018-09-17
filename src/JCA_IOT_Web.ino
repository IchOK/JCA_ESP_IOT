//--------------------------------------------------------------------------
// Initialisierung des Webservers
//--------------------------------------------------------------------------
void initWebserver(){
  server.onNotFound([](AsyncWebServerRequest *request){
    if(request->method() == HTTP_GET){
      #ifdef _DEBUG_ON
        Serial.print("GET ");
        Serial.print("[");
        Serial.print(request->host());
        Serial.print(" ");
        Serial.print(request->url());
        Serial.println("] ");
      #endif
      request->send(SPIFFS, request->url(), String(), false, getTemplates);
    }
    else if(request->method() == HTTP_POST){
      #ifdef _DEBUG_ON
        Serial.println("POST");
      #endif
      int params = request->params();
      int i;
      for(i=0;i<params;i++){
        AsyncWebParameter* p = request->getParam(i);
        if(p->isPost()){
          if(p->name() == "Reboot"){
            ESP.restart();
          }
/*          else if(p->name() == "InitIOT"){
            InitIOT();
          }
*/        }
      }
    }
  });

  server.onFileUpload([](AsyncWebServerRequest *request, const String& filename, size_t index, uint8_t *data, size_t len, bool final){
    //Start File-Upload
    if(!index){
      #ifdef _DEBUG_ON
        Serial.printf("UploadStart: %s\n", filename.c_str());
      #endif
      // open file, overwrite
      String strName = filename;
      if(!strName.startsWith("/")){
        strName = "/" + strName + ".tmp";
      }else{
        strName = strName + ".tmp";
      }
      fsUploadFile = SPIFFS.open(strName, "w");
    }
    
    //Daten in File schreiben
    if(fsUploadFile){
      fsUploadFile.write(data, len);
    }

    //Ende File-Upload
    if(final){
      #ifdef _DEBUG_ON
        Serial.printf("UploadEnd: %s (%u)\n", filename.c_str(), index+len);
      #endif
      if(fsUploadFile){
        fsUploadFile.close();
        String strName = filename;
        if(!strName.startsWith("/")){
          strName = "/" + strName;
        }else{
          strName = strName;
        }
        SPIFFS.remove(strName);
        SPIFFS.rename(strName + ".tmp", strName);
        request->send(303);
      }else{
        request->send(500, "text/plain", "500: couldn't create file");
      }
    }
  });

  server.begin();
}

//--------------------------------------------------------------------------
// Webserver
//--------------------------------------------------------------------------
String getTemplates(const String& var){
  int ioIndex;
  char strBuffer[30];
  if(var == "FreeHeap"){
    return String(ESP.getFreeHeap());
  }else{
    var.toCharArray(strBuffer, 30);
    ioIndex = Runner.findIO(strBuffer);
    if(ioIndex >= 0){
      return String(Runner.getIO(ioIndex)->getString(strBuffer));
    }else{
      return String();
    }
  }
}
