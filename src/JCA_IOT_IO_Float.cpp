/**********************************************
 * Beschreibung: siehe Header File
 **********************************************/

#include <JCA_IOT_IO_Float.h>
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
cFloat::cFloat(const char* strName) : cRoot(strName, JCA_IOT_IO_T_FLOAT){
	this->QC = JCA_IOT_QC_INIT;
}
cFloat::cFloat(const char* strName, float fValue) : cRoot(strName, JCA_IOT_IO_T_FLOAT){
	this->Value = fValue;
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
void cFloat::setString(char* strValue){
	this->Value = atof(strValue);
	this->QC = JCA_IOT_QC_GOOD;
}
char* cFloat::getString(char* strValue){
	String(this->Value).toCharArray(strValue, sizeof(strValue));
	return strValue;
	}
void  cFloat::setFloat(float Value){this->Value = Value;}
float cFloat::getFloat(){return this->Value;}
void  cFloat::setTime(int32_t Value){this->Value = (float)Value;}
int32_t  cFloat::getTime(){return (int32_t)this->Value;}
void  cFloat::setLong(int32_t Value){this->Value = (float)Value;}
int32_t  cFloat::getLong(){return (int32_t)this->Value;}
void  cFloat::setBool(bool Value){}
bool  cFloat::getBool(){
	if(this->Value > 0){
		return true;
	} else {
		return false;
	}
}
void  cFloat::setDate(int32_t Value){this->Value = (float)Value;}
int32_t  cFloat::getDate(){return (int32_t)this->Value;}
