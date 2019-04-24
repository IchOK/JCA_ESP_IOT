/**********************************************
 * Class:	JCA_IOT_ELEMENT
 * Info: 	Die Klasse ist der Ursprung für alle JCA_IOT_IO
 *          Elemente und enthält die definition der
 *          Datenstrukturen sowie Grundfunktionen für Ein-
 *          und Ausaben, sowie die Schrnittstelle zur
 *          Kommunikation.
 * Version:
 * 	V1.0.0	Erstellt	23.04.2019	JCA
 *		-tData
 *    -tInterface
 *    -tMesh
 *    -addParam
 *    -addValue
 *    -addMesh
 **********************************************/

#ifndef _JCA_IOT_ELEMENT_H
#define _JCA_IOT_ELEMENT_H

#include "JCA_IOT_QC.h"

#define JCA_IOT_ELEMENT_NAME_LEN 30

#define JCA_IOT_ELEMENT_DATA_BOOL 1
#define JCA_IOT_ELEMENT_DATA_INT 2
#define JCA_IOT_ELEMENT_DATA_FLOAT 3

#define JCA_IOT_ELEMENT_MESH_PUT 1
#define JCA_IOT_ELEMENT_MESH_GET 2

#define JCA_IOT_ELEMENT_DIR_IN 1
#define JCA_IOT_ELEMENT_DIR_OUT 2
#define JCA_IOT_ELEMENT_DIR_INOUT 3

#define JCA_IOT_ELEMENT_RANGE_IO 1
#define JCA_IOT_ELEMENT_RANGE_FUNC 2
#define JCA_IOT_ELEMENT_RANGE_MESH 3

#include <vector>

namespace JCA{ namespace IOT{ namespace ELEMENT{
  //Struktur zur internen Datenablage
  struct tData{
    float fValue;     //Wert als Gleitpunktzahl
    int32_t iValue;   //Wert als Ganzzahl
    char Name[JCA_IOT_ELEMENT_NAME_LEN];  //Name des Datenpunkts
    unsigned char Type; //Type -> DEFINE
    unsigned char QC;   //Quality-Code -> INCLUDE
  };

  //Struktur für die Beschreibung einer Schrnittstelle
  struct tInterface{
    char Name[JCA_IOT_ELEMENT_NAME_LEN];  //Name des Interfaces
    unsigned char Datatype; //Datentyp -> DEFINE
    unsigned char Dir;      //Richtung -> DEFINE
    unsigned char Range;    //Bereich -> Define
    unsigned char Offset;   //Positin des Werts im Element
    int16_t Index;          //Index des Elements im Bereich
  };

  struct tRemote{
    char Station[JCA_IOT_ELEMENT_NAME_LEN];
    char Element[JCA_IOT_ELEMENT_NAME_LEN];
    char Datapoint[JCA_IOT_ELEMENT_NAME_LEN];
    unsigned char Direction;  //Senden oder Anfragen -> DEFINE
    int32_t Watchdog;         //Laufzeitüberwachung
  };

  class cRoot {
  	public:
  		cSystem(const char* strName, const unsigned char iRange);

    protected:
  		bool addParam();
      bool addValue();
      bool addRemote();
      bool addInterface();
      bool setParam(const JsonObject* data);
      bool getParamAll(JsonArray* data);
      bool setValue(const JsonObject* data);
      bool getValueAll(JsonArray* data);
/*
  Zugriff aus dem Element erfolgt direkt über den Vector
  Zugriffe von aussen erfolgen immer über JsonArrays

      float getParamFloat(const int16_t index);
      int32_t getParamInt(const int16_t index);
      bool getParamBool(const int16_t index);
      bool setValueFloat(const int16_t index, const float value);
      bool setValueInt(const int16_t index, const int32_t value);
      bool setValueBool(const int16_t index, const bool value);
      float getValueFloat(const int16_t index);
      int32_t getValueInt(const int16_t index);
      bool getValueBool(const int16_t index);
*/

      std::vector<tData> Params;
      std::vector<tData> Values;
      std::vector<tInterface> Interfacess;
      std::vector<tRemote> Remotes;

  };
}}}
