/**********************************************
 * Beschreibung: siehe Header File
 **********************************************/

#include <JCA_IOT_JUMP.h>
//#define _DEBUG_ON

using namespace JCA::IOT;
/**********************************************
 * Methode: Constructor()
 * Info:	Initialisierung der statischen Werte
 * Param:
 * Version:
 *	V1.0.0	Erstellt	24.05.2018	JCA
 **********************************************/
cJump::cJump(int iPos, char* strName){
	#ifdef _DEBUG_ON
		Serial.print("cJump::cJump");
	#endif
	this->pos = iPos;
	strcpy(this->Name, strName);
}
