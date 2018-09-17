/**********************************************
 * Beschreibung: siehe Header File
 **********************************************/

#include <JCA_IOT_IO_Bool.h>
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
cBool::cBool(const char* strName) : cRoot(strName, JCA_IOT_IO_T_FLOAT){
	this->QC = JCA_IOT_QC_INIT;
}
cBool::cBool(const char* strName, bool bValue) : cRoot(strName, JCA_IOT_IO_T_FLOAT){
	this->Value = bValue;
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
void cBool::setString(char* strValue){
	this->Value = (bool)atoi(strValue);
	this->QC = JCA_IOT_QC_GOOD;
}
char* cBool::getString(char* strValue){
	String(this->Value).toCharArray(strValue, sizeof(strValue));
	return strValue;
	}
void  cBool::setFloat(float Value){this->Value = (bool)Value;}
float cBool::getFloat(){return (float)this->Value;}
void  cBool::setTime(int32_t Value){this->Value = (bool)Value;}
int32_t  cBool::getTime(){return (int32_t)this->Value;}
void  cBool::setLong(int32_t Value){this->Value = (bool)Value;}
int32_t  cBool::getLong(){return (int32_t)this->Value;}
void  cBool::setBool(bool Value){this->Value = Value;}
bool  cBool::getBool(){return this->Value > 0;}
void  cBool::setDate(int32_t Value){this->Value = (bool)Value;}
int32_t  cBool::getDate(){return (int32_t)this->Value;}
