/**********************************************
 * Info: 	Die Typen/Klassen werden für die unterschiedlichen
 *    Klassen des IOT's benötigt und sind deswegen
 *    global definiert.
 * Typen:
 *
 **********************************************/

#ifndef _JCA_IOT_TYPES_H
#define _JCA_IOT_TYPES_H

#define JCA_IOT_TYPES_T_INT 1
#define JCA_IOT_TYPES_T_FLOAT 2

#include <ArduinoJson.h>
#inculde "JCA_IOT_QC.h"

/**********************************************
 * Class:	JCA_IOT_Data
 * Info: 	Die Klasse stellt einen Datenpunkt mit
 *          Name und Wert dar.
 * Version:
 * 	V1.0.0	Erstellt	23.04.2019	JCA
 *    -constructor
 *    -getValue
 *    -getname
 *    -addJson
 **********************************************/
namespace JCA{ namespace IOT{
class cData {
  public:
    cData(const char* strName, const unsigned char Type, const int32_t iValue);
    cData(const char* strName, const unsigned char Type, const float fValue);
    char* getName();
    bool chechName(const char* strName);
    bool getJson(JsonObject* jStatus);
    int32_t getInt();
    float getFloat();
    bool setInt(int32_t Value);
    bool setFloat(float Value);

  protected:
    int32_t intValue;
    float floatValue;
    char Name[30];
    unsigned char Type;
    unsigned char QC;
};
}}





 #endif
