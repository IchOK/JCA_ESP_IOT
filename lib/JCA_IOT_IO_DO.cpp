/**********************************************
 * Beschreibung: siehe Header File
 **********************************************/

#include <JCA_IOT_IO_DO.h>
using namespace JCA::IOT::IO;

/**********************************************
 * Methode: Constructor(const char* strName)
 * Info:	Initialisierung der statischen Werte
 *				und übergibt den Namen
 * Param:	
 *		strName		Name des IO's
 *		iType		Type des IO's als String
 * Version:
 *	V1.0.0	Erstellt	22.05.2018	JCA
 **********************************************/
cDO::cDO(const char* strName, int iPort) : cRoot(strName, JCA_IOT_IO_T_DO){
	this->Port = iPort;
	pinMode(this->Port, OUTPUT);
	this->QC = JCA_IOT_QC_INIT;
}
cDO::cDO(const char* strName, int iPort, bool bValue) : cRoot(strName, JCA_IOT_IO_T_DO){
	this->Port = iPort;
	this->Value = bValue;
	pinMode(this->Port, OUTPUT);
	this->QC = JCA_IOT_QC_GOOD;
	this->update();
}

/**********************************************
 * Methode: update()
 * Info:	Aktuallisiert den IO-Wert
 *				der aktuelle Wert wird auf den
 *				Ausgang geschrieben.
 * Param:	
 * Version:
 *	V1.0.0	Erstellt	22.05.2018	JCA
 **********************************************/
void cDO::update(){
	if(this->QC){
		digitalWrite(this->Port, this->Value);
	}
}

/**********************************************
 * Methode: Get/Set...()
 * Info:	Wert schreiben oder auslesen
 * Param:	void / Wert
 * Return:	Wert / void
 * Version:
 *	V1.0.0	Erstellt	22.05.2018	JCA
 **********************************************/
void cDO::setString(char* strValue){
	this->Value = (bool)atoi(strValue);
	this->QC = JCA_IOT_QC_GOOD;
}
char* cDO::getString(char* strValue){
	String(this->Value).toCharArray(strValue, sizeof(strValue));
	return strValue;
	}
void  cDO::setFloat(float Value){this->Value = (bool)Value;this->QC = JCA_IOT_QC_GOOD;}
float cDO::getFloat(){return (float)this->Value;}
void  cDO::setTime(int32_t Value){this->Value = (bool)Value;this->QC = JCA_IOT_QC_GOOD;}
int32_t  cDO::getTime(){return (int32_t)this->Value;}
void  cDO::setLong(int32_t Value){this->Value = (bool)Value;this->QC = JCA_IOT_QC_GOOD;}
int32_t  cDO::getLong(){return (int32_t)this->Value;}
void  cDO::setBool(bool Value){this->Value = Value;this->QC = JCA_IOT_QC_GOOD;}
bool  cDO::getBool(){return this->Value;}
void  cDO::setDate(int32_t Value){this->Value = (bool)Value;this->QC = JCA_IOT_QC_GOOD;}
int32_t  cDO::getDate(){return (int32_t)this->Value;}
