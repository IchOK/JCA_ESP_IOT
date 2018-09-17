/**********************************************
 * Beschreibung: siehe Header File
 **********************************************/

#include <JCA_IOT_IO_DI.h>
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
cDI::cDI(const char* strName, int iPort, bool bPullup) : cRoot(strName, JCA_IOT_IO_T_DI){
	this->Port = iPort;
	if(bPullup){
		pinMode(this->Port, INPUT_PULLUP);
	}else{
		pinMode(this->Port, INPUT);
	}
	this->QC = JCA_IOT_QC_INIT;
}
cDI::cDI(const char* strName, int iPort, bool bPullup, bool bValue) : cRoot(strName, JCA_IOT_IO_T_DI){
	this->Port = iPort;
	this->Value = bValue;
	if(bPullup){
		pinMode(this->Port, INPUT_PULLUP);
	}else{
		pinMode(this->Port, INPUT);
	}
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
void cDI::update(){
	this->Value = digitalRead(this->Port);
	this->QC = JCA_IOT_QC_GOOD;
}

/**********************************************
 * Methode: Get/Set...()
 * Info:	Wert schreiben oder auslesen
 * Param:	void / Wert
 * Return:	Wert / void
 * Version:
 *	V1.0.0	Erstellt	22.05.2018	JCA
 **********************************************/
void cDI::setString(char* strValue){
	this->Value = (bool)atoi(strValue);
	this->QC = JCA_IOT_QC_GOOD;
}
char* cDI::getString(char* strValue){
	String(this->Value).toCharArray(strValue, sizeof(strValue));
	return strValue;
	}
void  cDI::setFloat(float Value){this->Value = (bool)Value;this->QC = JCA_IOT_QC_GOOD;}
float cDI::getFloat(){return (float)this->Value;}
void  cDI::setTime(int32_t Value){this->Value = (bool)Value;this->QC = JCA_IOT_QC_GOOD;}
int32_t  cDI::getTime(){return (int32_t)this->Value;}
void  cDI::setLong(int32_t Value){this->Value = (bool)Value;this->QC = JCA_IOT_QC_GOOD;}
int32_t  cDI::getLong(){return (int32_t)this->Value;}
void  cDI::setBool(bool Value){this->Value = Value;this->QC = JCA_IOT_QC_GOOD;}
bool  cDI::getBool(){return this->Value;}
void  cDI::setDate(int32_t Value){this->Value = (bool)Value;this->QC = JCA_IOT_QC_GOOD;}
int32_t  cDI::getDate(){return (int32_t)this->Value;}
