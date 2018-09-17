/**********************************************
 * Beschreibung: siehe Header File
 **********************************************/

#include <JCA_IOT_IO_Long.h>
using namespace JCA::IOT::IO;

/**********************************************
 * Methode: Constructor(const char* strName)
 * Info:	Initialisierung der statischen Werte
 *				und übergibt den Namen
 * Param:	
 *		strName		Name des IO's
 *		iType		Type des IO's als String
 * Version:
 *	V1.0.0	Erstellt	19.05.2018	JCA
 **********************************************/
cLong::cLong(const char* strName) : cRoot(strName, JCA_IOT_IO_T_LONG){
	this->QC = JCA_IOT_QC_INIT;
}
cLong::cLong(const char* strName, int32_t lValue) : cRoot(strName, JCA_IOT_IO_T_LONG){
	this->Value = lValue;
	this->QC = JCA_IOT_QC_GOOD;
}

/**********************************************
 * Methode: Get/Set...()
 * Info:	Wert schreiben oder auslesen
 * Param:	void / Wert
 * Return:	Wert / void
 * Version:
 *	V1.0.0	Erstellt	19.05.2018	JCA
 **********************************************/
void cLong::setString(char* strValue){
	this->Value = atoi(strValue);
	this->QC = JCA_IOT_QC_GOOD;
}
char* cLong::getString(char* strValue){
	String(this->Value).toCharArray(strValue, sizeof(strValue));
	return strValue;
	}
void  cLong::setFloat(float Value){this->Value = (int32_t)Value;}
float cLong::getFloat(){return this->Value;}
void  cLong::setTime(int32_t Value){this->Value = Value;}
int32_t  cLong::getTime(){return this->Value;}
void  cLong::setLong(int32_t Value){this->Value = Value;}
int32_t  cLong::getLong(){return this->Value;}
void  cLong::setBool(bool Value){}
bool  cLong::getBool(){
	if(this->Value > 0){
		return true;
	} else {
		return false;
	}
}
void  cLong::setDate(int32_t Value){this->Value = Value;}
int32_t  cLong::getDate(){return this->Value;}
