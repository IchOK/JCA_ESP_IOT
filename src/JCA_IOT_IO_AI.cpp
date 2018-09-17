/**********************************************
 * Beschreibung: siehe Header File
 **********************************************/

#include <JCA_IOT_IO_AI.h>
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
cAI::cAI(const char* strName, int iPort, float fMin, float fMax) : cRoot(strName, JCA_IOT_IO_T_AI){
	this->Port = iPort;
	this->Min = fMin;
	this->Max = fMax;
	pinMode(this->Port, INPUT);
	this->QC = JCA_IOT_QC_INIT;
}
cAI::cAI(const char* strName, int iPort, float fMin, float fMax, float fValue) : cRoot(strName, JCA_IOT_IO_T_AI){
	this->Port = iPort;
	this->Min = fMin;
	this->Max = fMax;
	this->Value = fValue;
	pinMode(this->Port, INPUT);
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
void cAI::update(){
	int iIn = analogRead(this->Port);
	this->Value = (float)iIn / 1023.0 * (this->Max - this->Min) + this->Min;
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
void cAI::setString(char* strValue){
	this->Value = atof(strValue);
	this->QC = JCA_IOT_QC_GOOD;
}
char* cAI::getString(char* strValue){
	String(this->Value).toCharArray(strValue, sizeof(strValue));
	return strValue;
	}
void  cAI::setFloat(float Value){this->Value = Value;this->QC = JCA_IOT_QC_GOOD;}
float cAI::getFloat(){return (float)this->Value;}
void  cAI::setTime(int32_t Value){this->Value = (float)Value;this->QC = JCA_IOT_QC_GOOD;}
int32_t  cAI::getTime(){return (int32_t)this->Value;}
void  cAI::setLong(int32_t Value){this->Value = (float)Value;this->QC = JCA_IOT_QC_GOOD;}
int32_t  cAI::getLong(){return (int32_t)this->Value;}
void  cAI::setBool(bool Value){this->Value = (float)Value;this->QC = JCA_IOT_QC_GOOD;}
bool  cAI::getBool(){return this->Value;}
void  cAI::setDate(int32_t Value){this->Value = (float)Value;this->QC = JCA_IOT_QC_GOOD;}
int32_t  cAI::getDate(){return (int32_t)this->Value;}
