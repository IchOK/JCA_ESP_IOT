/**********************************************
 * Beschreibung: siehe Header File
 **********************************************/

#include <JCA_IOT_IO_AO.h>
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
cAO::cAO(const char* strName, int iPort, float fMin, float fMax) : cRoot(strName, JCA_IOT_IO_T_AO){
	this->Port = iPort;
	this->Min = fMin;
	this->Max = fMax;
	pinMode(this->Port, OUTPUT);
	this->QC = JCA_IOT_QC_INIT;
}
cAO::cAO(const char* strName, int iPort, float fMin, float fMax, float fValue) : cRoot(strName, JCA_IOT_IO_T_AO){
	this->Port = iPort;
	this->Min = fMin;
	this->Max = fMax;
	this->Value = fValue;
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
void cAO::update(){
	if(this->QC){
		int iOut = (int)((this->Value - this->Min)/(this->Max - this->Min) * 255.0);
		analogWrite(this->Port, iOut);
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
void cAO::setString(char* strValue){
	this->Value = atof(strValue);
	this->QC = JCA_IOT_QC_GOOD;
}
char* cAO::getString(char* strValue){
	String(this->Value).toCharArray(strValue, sizeof(strValue));
	return strValue;
	}
void  cAO::setFloat(float Value){this->Value = Value;this->QC = JCA_IOT_QC_GOOD;}
float cAO::getFloat(){return (float)this->Value;}
void  cAO::setTime(int32_t Value){this->Value = (float)Value;this->QC = JCA_IOT_QC_GOOD;}
int32_t  cAO::getTime(){return (int32_t)this->Value;}
void  cAO::setLong(int32_t Value){this->Value = (float)Value;this->QC = JCA_IOT_QC_GOOD;}
int32_t  cAO::getLong(){return (int32_t)this->Value;}
void  cAO::setBool(bool Value){this->Value = (float)Value;this->QC = JCA_IOT_QC_GOOD;}
bool  cAO::getBool(){return this->Value;}
void  cAO::setDate(int32_t Value){this->Value = (float)Value;this->QC = JCA_IOT_QC_GOOD;}
int32_t  cAO::getDate(){return (int32_t)this->Value;}
