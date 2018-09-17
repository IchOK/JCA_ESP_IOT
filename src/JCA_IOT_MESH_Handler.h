/**********************************************
 * Class:	JMZ_IOT_MESH_Handler
 * Info: 	Die Klasse beinhaltet die Handling-Funktionen
 *				der Mesh-Kommunikation.
 * Version:
 * 	V1.0.0	Erstellt	14.09.2018	JCA
 *		-HelloAll (Broadcast mit allen verfügbaren Datenpunkten)
 *		-AnyThere (Broadcase Abfrage nach Datenpunkten)
 *		-Iam (Antwort auf AnyThere)
 *		-GiveMe (Anforderunge zum zyklischen Senden an eine Station)
 *		-SendData (Daten an verfügbare Stationen senden)
 *		-AddToList (Befüllt die Matchlist mit Initialwerten)
 *		-Update
 *
 *		+MatchList (Matrix mit Zuordnungen zwischen Mesh und Variablen)
 **********************************************/

#ifndef _JCA_IOT_MESH_HANDLER_H
#define _JCA_IOT_MESH_HANDLER_H

#include "JCA_IOT_QC.h"
#include "JCA_IOT_MESH_STAT.h"
#include "JCA_IOT_CODE.h"

#define JCA_IOT_MESH_STATION_WD 10000000
#define JCA_IOT_MESH_MSG_LEN 1400

#include <painlessMesh.h>
#include <Arduino.h>
#include <string.h>
namespace JCA{ namespace IOT{ namespace MESH{
class cHandler {
	public:
		cHandler(painlessMesh* xMesh, JCA::IOT::cCode* xCode);
		void update(uint32_t ulMicros);	//loop Funktion Zähler erhöhen und Daten senden
		void helloAll();		//Wenn connected mit dem Netzwerk
		void receiveData(uint32_t ID, String* strData);		//Wenn Daten empfangen wurden
		void checkStations();						//alle 10s prüfen ob alle Stationen verbunden sind sonst fehlende anfragen
		void addToList(char* strStation, char* strTag, int iSetTime, int iIndex, char cRecv);//Datenpunkt in Liste eintragen

	protected:

		void checkMissingStations();
		void checkMissingTags();
		void checkUnlinkedTags();

		void receiveDatalist(uint32_t ID);
		void receiveDatatags(uint32_t ID);
		void receiveRequest(uint32_t ID);
		void receiveLinked(uint32_t ID);
		void receiveHowIs(uint32_t ID);


		void iAm(char* strDataList);
		void giveMe(int iStationId);
		bool genTagList();
		bool updateList(char* strStation, uint32_t ID, char* strTag, int iSetTime, int iIndex, char cRecv);

		char msgIn[JCA_IOT_MESH_MSG_LEN];
		char msgOut[JCA_IOT_MESH_MSG_LEN];
		char msgLoop[JCA_IOT_MESH_MSG_LEN];
		int checkMode;

		painlessMesh* mesh;
		JCA::IOT::cCode* code;
		std::vector<cStation*> stations;
		//Stations[]
		//	char*	Name
		//	uLong 	ID 	(0 = Nicht bekannt
		//	uLong	WatchDog 	(Kopplungsüberwachung
		//	Tags[]
		//		char*	Name
		//		int		ioIndex
		//		uLong	SetTime	(Konfigurierte Zeit)
		//		uLong	Count	(Micros-Zähler, Read=WD / Write=Zyklus)
		//		bool	Recv	(1 = Mesh -> IO, 0 = IO -> Mesh)
		//		bool	Found
};
}}}

#endif
