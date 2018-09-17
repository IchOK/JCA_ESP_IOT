/**********************************************
 * Beschreibung: siehe Header File
 **********************************************/

#include <JCA_IOT_MESH_STAT.h>
//#define _DEBUG_ON

using namespace JCA::IOT::MESH;
/**********************************************
 * Methode: Constructor()
 * Info:	Initialisierung der statischen Werte
 * Param:	
 * Version:
 *	V1.0.0	Erstellt	14.09.2018	JCA
 **********************************************/
cStation::cStation(char* strStation){
	#ifdef _DEBUG_ON
		Serial.print("cStation::cStation");
	#endif
	strncpy(this->Name, strStation, 30);
	this->ID = 0;
	this->WatchDog = 0;
	#ifdef _DEBUG_ON
		Serial.print(" Name:");
		Serial.print(strStation);
		Serial.print(" ID:");
		Serial.println(this->ID);
	#endif
}

