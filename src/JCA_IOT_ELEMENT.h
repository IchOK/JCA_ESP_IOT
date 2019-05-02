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
#define JCA_IOT_ELEMENT_MESH_LOG 3

#define JCA_IOT_ELEMENT_DIR_IN 0x01
#define JCA_IOT_ELEMENT_DIR_OUT 0x02
#define JCA_IOT_ELEMENT_DIR_INOUT 0x03

#define JCA_IOT_ELEMENT_RANGE_IO 1
#define JCA_IOT_ELEMENT_RANGE_FUNC 2
#define JCA_IOT_ELEMENT_RANGE_MESH 3

#define JCA_IOT_ELEMENT_WRITE_CREAT 0x00
#define JCA_IOT_ELEMENT_WRITE_UPDATE 0x01
#define JCA_IOT_ELEMENT_WRITE_PUT 0x02

//Code MAKRO
#define JCA_IOT_ELEMENT_DIR_IS_IN(wert) ((wert & JCA_IOT_ELEMENT_DIR_IN) == JCA_IOT_ELEMENT_DIR_IN)
#define JCA_IOT_ELEMENT_DIR_IS_OUT(wert) ((wert & JCA_IOT_ELEMENT_DIR_OUT) == JCA_IOT_ELEMENT_DIR_OUT)

//Inkludes
#include <vector>

namespace JCA{ namespace IOT{ namespace ELEMENT{
  //Struktur zur internen Datenablage
  struct tData{
    float fValue;     //Wert als Gleitpunktzahl
    int32_t iValue;   //Wert als Ganzzahl
    char Name[JCA_IOT_ELEMENT_NAME_LEN];  //Name des Datenpunkts
    unsigned char Type; //Type -> DEFINE
    unsigned char Write;
    unsigned char QC;   //Quality-Code -> INCLUDE
  };

  //Struktur für die Beschreibung einer Schrnittstelle
  struct tInterface{
    char Name[JCA_IOT_ELEMENT_NAME_LEN];  //Name des Interfaces
    unsigned char Type;     //Datentyp -> DEFINE
    unsigned char Dir;      //Richtung -> DEFINE
    unsigned char Area;     //Bereich -> Define
    unsigned char Value;    //Positin des Werts im Element
    bool bValue;
    float fValue;
    int32_t iValue;
    int16_t Element;        //Index des Elements im Bereich
  };

  struct tRemote{
    char Station[JCA_IOT_ELEMENT_NAME_LEN];
    char Element[JCA_IOT_ELEMENT_NAME_LEN];
    char Datapoint[JCA_IOT_ELEMENT_NAME_LEN];
    unsigned char Dir;        //Senden oder Anfragen -> DEFINE
    unsigned char Value;      //Wert im Element der gesendet/empfangen Wird
    unsigned char Interface;  //Interface des Elements das als Trigger dient
    bool TriggerOld;          //Speicher zur Flankenerkennung
    int32_t Watchdog;         //Laufzeitüberwachung
  };

  class cRoot {
  	public:
  		cSystem(const JsonObject *data);
      //liest einen Wert aus anhand des Index im Values-Vector
      bool getValueBool(const int32_t index, bool *Value);
      bool getValueInt(const int32_t index, int32_t *Value);
      bool getValueFloat(const int32_t index, float *Value);
      bool setValueBool(const int32_t index, const bool Value);
      bool setValueInt(const int32_t index, const int32_t Value);
      bool setValueFloat(const int32_t index, const float Value);
      //Aktuallisiert einen Wert anhand des JsonObects
      bool setValue(const JsonObject *data);
      //Gibt alle Werte zurück für die externe Verarbeitung
      bool getValueAll(JsonArray *data);
      //Aktuallisiert die Runtimedaten eines Elements, diese gehen
      //  nach einem Neustart oder neu einlesen der Konfig verlohren
      bool updateElement(const JsonObject *data);
      //Gibt den Zustand des Kompletten Elements zurück
      //  mit allen Vectoren als einzelnes Array im JsonObject
      bool getElement(JsonArray *data);
      //Aktuallisiert das komplette Element In-Code-Out-Mesh
      bool update(std::vector<cRoot*> *IOs,
        std::vector<cRoot*> *Funcs,
        std::vector<cRoot*> *Meshs,
        JsonObject *dataToMesh,
        int32_t loopTime);

    protected:
      //Konstruktor Funktionen zum erstellen der Elemente
  		bool addParam(const char* name, const unsigned char dataType);
      bool addValue(const char* name, const unsigned char dataType);
      bool addInterface(const char* name, const unsigned char dataType);
      //Standardfunktion vom verändern der internen Werte,
      //  wird sowohl vom Konstruktor als auch von der
      //  Runtime verwendet
      bool writeData(const JsonObject* data, const unsigned char caller);
      //Standardfuntkionen zur Runtimebearbeitung
      bool inInterface(std::vector<cRoot*> *IOs,
        std::vector<cRoot*> *Funcs,
        std::vector<cRoot*> *Meshs,);
      bool outInterface(std::vector<cRoot*> *IOs,
        std::vector<cRoot*> *Funcs,
        std::vector<cRoot*> *Meshs,);
      bool checkRemote(JsonObject* dataToMesh, int32_t loopTime);
      //Userfunktion, enthält den eigentlichen Code des Elements
      bool codeElement();

      std::vector<tData> Params;
      std::vector<tData> Values;
      std::vector<tInterface> Interfaces;
      std::vector<tRemote> Remotes;

  };
}}}
