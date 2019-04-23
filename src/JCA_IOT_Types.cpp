/**********************************************
 * Beschreibung: siehe Header File
 **********************************************/

#include <JCA_IOT_Types.h>

using namespace JCA::IOT;
/**********************************************
 * Methode: Constructor(...)
 * Info:	Initialisierung der statischen Werte
 *				und übergibt den Namen
 * Param:
 *		strName Name des Datenpunkts
 *		iType	  Type des Datenpunkts
 *    Value   Initialwert des Datenpunkts
 * Version:
 *	V1.0.0	Erstellt	19.05.2018	JCA
 **********************************************/
cData::cData(const char* strName, const unsigned char iType, const int32_t iValue){
  unsigned char tQC = JCA_IOT_QC_INIT;
  int iLen = strlen(strName);
  //Namen überprüfen
  if(iLen <= 0 || iLen > 30){
    tQC = JCA_IOT_QC_CONFNAME;
  }else{
    strcpy(this->Name, strName);
  }

  //Wenn Namen ok Type überprüfen
  if(tQC == JCA_IOT_QC_INIT){
    switch(iType){
      case JCA_IOT_TYPES_T_INT :
        this->intValue = iValue;
        this->Type = iType;
        break;
      case JCA_IOT_TYPES_T_FLOAT :
        this->floatValue = (float)iValue;
        this->Type = iType;
        break;
      default:
        this->Type = 0;
        tQC = JCA_IOT_QC_CONFTYPE;
        break;
    }
  }

  //Quality-Code des Datenpunkts setzen
  this->QC = tQC;
}
cData::cData(const char* strName, const unsigned char iType, const float fValue){
  unsigned char tQC = JCA_IOT_QC_INIT;
  int iLen = strlen(strName);
  //Namen überprüfen
  if(iLen <= 0 || iLen > 30){
    tQC = JCA_IOT_QC_CONFNAME;
  }else{
    strcpy(this->Name, strName);
  }

  //Wenn Namen ok Type überprüfen
  if(tQC == JCA_IOT_QC_INIT){
    switch(iType){
      case JCA_IOT_TYPES_T_INT :
        this->intValue = (int32_t)fValue;
        this->Type = iType;
        break;
      case JCA_IOT_TYPES_T_FLOAT :
        this->floatValue = fValue;
        this->Type = iType;
        break;
      default:
        this->Type = 0;
        tQC = JCA_IOT_QC_CONFTYPE;
        break;
    }
  }

  //Quality-Code des Datenpunkts setzen
  this->QC = tQC;
}

/**********************************************
 * Methode: getName()
 * Info:	Gibt den Namen des Datenpunkts zurück
 * Param:
 * Return:	Name als Zeichenkette
 * Version:
 *	V1.0.0	Erstellt	23.04.2019	JCA
 **********************************************/
char* cData::getName(){
  if(this->QC != JCA_IOT_QC_CONFNAME){
    return this->Name;
  }else{
    return NULL;
  }
}

/**********************************************
 * Methode: checkName(char* strName)
 * Info:	Prüft ob der Name dem Datenpunkt entspricht
 * Param:
 * Return:	1=Name gleich und gültig
 * Version:
 *	V1.0.0	Erstellt	23.04.2019	JCA
 **********************************************/
char* cData::checkName(const char* strName){
  if(this->QC != JCA_IOT_QC_CONFNAME){
    return (0 = strcpm(this->Name,strName));
  }else{
    return false;
  }
}

/**********************************************
 * Methode: getJson(JsonObject* jStatus)
 * Info: gibt den Status des Datenpunkts als Json-Objekt zurück
 * Param:
 * Return:	false bei Fehler
 * Version:
 *	V1.0.0	Erstellt	23.04.2019	JCA
 **********************************************/
bool cData::addJson(JsonObject* jStatus){
  if(this->QC == JCA_IOT_QC_CONFNAME){
    jStatus["name"] = "Undefined";
  }else{
    jStatus["name"] = this->Namen;
  }
  jStatus["type"] = this->Type;
  switch(this->Type){
    case JCA_IOT_TYPES_T_INT :
      jStatus["wert"] = this->intValue;
      break;
    case JCA_IOT_TYPES_T_FLOAT :
      jStatus["wert"] = this->floatValue;
      break;
    default:
      jStatus["wert"] = 0;
      break;
  }
  jStatus["qc"] = this->QC;

  //TODO später
  return true;
}

/**********************************************
 * Methode: Get/Set...()
 * Info:	Wert schreiben oder auslesen
 * Param:	void / Wert
 * Return:	Wert / void
 * Version:
 *	V1.0.0	Erstellt	23.04.2019	JCA
 **********************************************/
bool  cData::setFloat(float Value){
  if(JCA_IOT_QC_IO(this->QC)){
    switch(this->Type){
      case JCA_IOT_TYPES_T_INT :
        this->intValue = (int32_t)Value;
        this->QC = JCA_IOT_QC_GOOD;
        break;
      case JCA_IOT_TYPES_T_FLOAT :
        this->floatValue = Value;
        this->QC = JCA_IOT_QC_GOOD;
        break;
    }
    return true;
  }else{
    return false;
  }
}
float cData::getFloat(){
  if(JCA_IOT_QC_IO(this->QC)){
    switch(this->Type){
      case JCA_IOT_TYPES_T_INT :
        return (float)this->intValue;
        break;
      case JCA_IOT_TYPES_T_FLOAT :
        return this->floatValue;
        break;
    }
  }else{
    return 0.0;
  }
}
bool  cData::setInt(int32_t Value){
  if(JCA_IOT_QC_IO(this->QC)){
    switch(this->Type){
      case JCA_IOT_TYPES_T_INT :
        this->intValue = Value;
        this->QC = JCA_IOT_QC_GOOD;
        break;
      case JCA_IOT_TYPES_T_FLOAT :
        this->floatValue = (float)Value;
        this->QC = JCA_IOT_QC_GOOD;
        break;
    }
    return true;
  }else{
    return false;
  }
}
float cData::getint(){
  if(JCA_IOT_QC_IO(this->QC)){
    switch(this->Type){
      case JCA_IOT_TYPES_T_INT :
        return this->intValue;
        break;
      case JCA_IOT_TYPES_T_FLOAT :
        return (int32_t)this->floatValue;
        break;
    }
  }else{
    return 0;
  }
}
