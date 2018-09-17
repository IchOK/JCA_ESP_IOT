/**********************************************
 * Beschreibung: siehe Header File
 **********************************************/

#include <JCA_IOT_MESH_TAG.h>
//#define _DEBUG_ON

using namespace JCA::IOT::MESH;
/**********************************************
 * Methode: Constructor()
 * Info:	Initialisierung der statischen Werte
 * Param:
 * Version:
 *	V1.0.0	Erstellt	14.09.2018	JCA
 **********************************************/
cTag::cTag(char* strTag, int iSetTime, int iIndex, bool bRecv){
	#ifdef _DEBUG_ON
		Serial.print("cTag::cTag");
	#endif
	strncpy(this->name, strTag, 30);
	this->ioIndex = iIndex;
	this->setTime = 1000000 * (uint32_t)iSetTime;
	this->recv = bRecv;
	this->count = 0;
	this->found = false;
	this->linked = false;
	#ifdef _DEBUG_ON
		Serial.print(" Name:");
		Serial.print(strName);
		Serial.print(" Index:");
		Serial.print(iIndex);
		Serial.print(" Time:");
		Serial.print(iSetTime);
		Serial.print(" Recv:");
		Serial.println(bRecv);
	#endif
}
