/**********************************************
 * Beschreibung: siehe Header File
 **********************************************/

#include <JCA_IOT_CODELINE.h>
//#define _DEBUG_ON

using namespace JCA::IOT;
/**********************************************
 * Methode: Constructor()
 * Info:	Initialisierung der statischen Werte
 * Param:	
 * Version:
 *	V1.0.0	Erstellt	22.05.2018	JCA
 **********************************************/
cCodeLine::cCodeLine(int iFunction, int iType, int iPos){
	#ifdef _DEBUG_ON
		Serial.print("cCodeLine::cCodeLine");
	#endif
	this->function = iFunction;
	this->type = iType;
	this->pos = iPos;
	#ifdef _DEBUG_ON
		Serial.print(" Func:");
		Serial.print(iFunction);
		Serial.print(" Type:");
		Serial.print(iType);
		Serial.print(" Pos:");
		Serial.println(iPos);
	#endif
}

