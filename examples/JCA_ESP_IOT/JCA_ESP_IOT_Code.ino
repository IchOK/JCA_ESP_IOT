//--------------------------------------------------------------------------
// Initialisierung des IOT's
//--------------------------------------------------------------------------
void initCode(){
  File file = SPIFFS.open(CODE_FILE, "r");
  Runner.pharseFile(&file, &addCodeToMesh);
  file.close();
}

//--------------------------------------------------------------------------
// Task-Funktion zur Code-Ausführung
//--------------------------------------------------------------------------
void runCode(){
  int RetVal;
  // IOs einlesen / schreiben
  RetVal = Runner.doIOs();
  #ifdef _DEBUG_ON
    if(RetVal != 0){
      Serial.print("FEHLER IOs Zeile ");
      Serial.print(RetVal);
      Serial.print(" QC=");
      Serial.println(Runner.getIO(RetVal)->getQC());
    }
  #endif
  
  // Code ausführen
  RetVal = Runner.doCode(0);
  #ifdef _DEBUG_ON
    if(RetVal != 0){
      Serial.print("FEHLER Code Zeile ");
      Serial.println(RetVal);
    }
  #endif
}


