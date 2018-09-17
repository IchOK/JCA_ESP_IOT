#define _DEBUG_ON

#include <painlessMesh.h>
#include <FS.h>
#define CODE_FILE "/Code.txt"

#include <JCA_IOT_IO_All.h>
#include <JCA_IOT_CODE.h>
//#include <JCA_IOT_IO_Float.h>
//#include <JCA_IOT_IO_Long.h>
//#include <JCA_IOT_IO_DO.h>

using namespace JCA::IOT;

cCode Runner;// = new cCode();

void InitIOT(){
  File file = SPIFFS.open(CODE_FILE, "r");
  Runner.pharseFile(&file);
  file.close();
}

void setup() {
  #ifdef _DEBUG_ON
    unsigned long mTime = micros();
    Serial.begin(74880);
  #endif
  SPIFFS.begin();
  InitIOT();
  #ifdef _DEBUG_ON
    Serial.print("Init Done in ");
    Serial.print(micros() - mTime);
    Serial.println("µs");
  #endif
}

bool GetLoop(unsigned long *tDif){
  static unsigned long tOld;
  static unsigned long cLoop;
  unsigned long t;

  cLoop++;
  if(tOld == 0){
    tOld = micros();
  }
  t = micros() - tOld;
  
  if(t > 1000000){
    *tDif = t / cLoop;
    cLoop = 0;
    tOld = micros();
    return true;
  }else{
    return false;
  }
}

void loop() {
  unsigned long tLoop;
  int RetVal;
  if(GetLoop(&tLoop)){
    #ifdef _DEBUG_ON
      Serial.print("Loop Done in ");
      Serial.print(tLoop);
      Serial.print("µs Button:");
    #endif
  }
  RetVal = Runner.doIOs();
  #ifdef _DEBUG_ON
    if(RetVal != 0){
      Serial.print("FEHLER IOs Zeile ");
      Serial.println(RetVal);
    }
  #endif

  RetVal = Runner.doCode(0);
  #ifdef _DEBUG_ON
    if(RetVal != 0){
      Serial.print("FEHLER Code Zeile ");
      Serial.println(RetVal);
    }
  #endif
}
