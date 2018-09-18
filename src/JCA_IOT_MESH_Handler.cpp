/**********************************************
 * Beschreibung: siehe Header File
 **********************************************/

#include <JCA_IOT_MESH_Handler.h>
//#define _DEBUG_ON

using namespace JCA::IOT::MESH;
/**********************************************
 * Methode: Constructor()
 * Info:	Initialisierung der statischen Werte
 * Param:
 * Version:
 *	V1.0.0	Erstellt	14.09.2018	JCA
 **********************************************/
cHandler::cHandler(painlessMesh* xMesh, JCA::IOT::cCode* xCode){
	#ifdef _DEBUG_ON
		Serial.print("JCA::IOT::MESH::cHandler::cHandler");
	#endif
	this->mesh = xMesh;
	this->code = xCode;
	this->checkMode = 0;
	this->checkStation = 0;
}

/**********************************************
 * Methode: update()
 * Info:	Zyklisch aufgerufene Funktion die die
 *				Watchdogs und Counter aktuallisiert
 *				und Daten zu den angemeldeten Stationen sendet
 * Param:
 *		ulMicros : Zeit seit letzten Aufruf in µSekunden
 * Return:
 * Version:
 *	V1.0.0	Erstellt	14.09.2018	JCA
 **********************************************/
void cHandler::update(uint32_t ulMicros){
	uint16_t x;
	uint16_t y;
	bool bSend;
	char buffer[30];
	for(x=0;x<this->stations.size();x++){
		if(this->stations[x]->id != this->mesh->getNodeId()){
			//stations Watchdog
			if(this->stations[x]->watchDog > JCA_IOT_MESH_STATION_WD){
				this->stations[x]->id = 0; //Station nicht mehr vorhanden
			}else{
				this->stations[x]->watchDog += ulMicros;
			}

			bSend = false;
			strcpy(this->msgOut, "DATA$");
			//tags jeder Station durchlaufen
			for(y=0;this->stations[x]->tags.size();y++){
				if(this->stations[x]->tags[y]->count > this->stations[x]->tags[y]->setTime){
					if(this->stations[x]->tags[y]->recv){
						this->code->getIO(this->stations[x]->tags[y]->ioIndex)->setQC(JCA_IOT_QC_COMMLAST);
					}else{
						if(bSend){strcat(this->msgOut, "$");}
						strcat(this->msgOut, this->stations[x]->tags[y]->name);
						strcat(this->msgOut, ";");
						strcat(this->msgOut, this->code->getIO(this->stations[x]->tags[y]->ioIndex)->getString(buffer));
						bSend = true;
						this->stations[x]->tags[y]->count = 0;
					}
				}else{
					this->stations[x]->tags[y]->count += ulMicros;
				}
			}
			if(bSend && this->mesh->isConnected(this->stations[x]->id)){
				String dummy = String(this->msgOut);
				this->mesh->sendSingle(this->stations[x]->id, dummy);
			}
		}
	}
}

/**********************************************
 * Methode: helloAll()
 * Info:	Broadcast mit stationsname und allen tags
 * Param:
 * Return:
 * Version:
 *	V1.0.0	Erstellt	17.09.2018	JCA
 **********************************************/
void cHandler::helloAll(){
	if(this->genTagList()){
		String dummy = String(this->msgOut);
		this->mesh->sendBroadcast(dummy);
	}
}

/**********************************************
 * Methode: reciveData()
 * Info:	Daten Empfangen und auswerten
 * Param:
 *		ID       : NodeID des Absenders
 *		strData	 : Empfange Zeichenkette
 * Return:
 * Version:
 *	V1.0.0	Erstellt	17.09.2018	JCA
 **********************************************/
void cHandler::receiveData(uint32_t ID, String* strData){
	char* ptr;
	int iPos;
	char strType[20];

	iPos = strData->indexOf("$");
	if(iPos > 0 && iPos < strData->length()){
		strData->substring(0,iPos-1).toCharArray(strType,20);
		strData->substring(iPos+1).toCharArray(this->msgIn,JCA_IOT_MESH_MSG_LEN);

		//Tag-Auflistung empfangen
		if(strcmp(ptr,"DATALIST") == 0){
			this->receiveDatalist(ID);

		//Daten empfangen
		}else if(strcmp(ptr,"DATA") == 0){
			this->receiveDatatags(ID);

		//Tag-Anforderung empfangen
		}else if(strcmp(ptr,"REQUEST") == 0){
			this->receiveRequest(ID);

		//Tag-Requestbestätigung empfangen
		}else if(strcmp(ptr,"LINKED") == 0){
			this->receiveLinked(ID);

		//Tag-Requestbestätigung empfangen
		}else if(strcmp(ptr,"HOWIS") == 0){
			this->receiveHowIs(ID);


		}
	}
}

/**********************************************
 * Methode: checkstations()
 * Info:	Die Fuktion prüft den Zustand der
 *				Stationen und fragt falls nötig
 *				die Nodes ab.
 *				Die Prüfung erfolgt in drei Schritten.
 *				Vorgesehen für zyklische Abarbeitung (10s)
 * Param:
 * Return:
 * Version:
 *	V1.0.0	Erstellt	17.09.2018	JCA
 **********************************************/
void cHandler::checkStations(){
	if(this->checkStation >= this->stations.size()){
		this->checkStation = 0;
	}
	switch(this->checkMode){
		case 0:	//nicht verlinkte Stationen prüfen
			this->checkMissingStations(this->checkStation);
			this->checkMode = 1;
			break;

		case 1:	//nicht gefundene tags prüfen
			this->checkMissingTags(this->checkStation);
			this->checkMode = 0;
			this->checkStation++;
			break;

		default:
			this->checkMode = 0;
			break;
	}
}

/**********************************************
 * Methode: addToList
 * Info:		Fügt einen Datenpunkt in die Matchliste
 *						ein.
 * Param:
 *		strStation : stations Name
 *		strTag	: Name des tags
 *		iSetTime: Sendeintervall/Watchdog des tags
 *		iIndex	: index im lokalen IO-Vektor
 *		cRecv	: R=Empfangen
 * Return:
 * Version:
 *	V1.0.0	Erstellt	18.09.2018	JCA
 **********************************************/
void cHandler::addToList(char* strStation, char* strTag, int iSetTime, int iIndex, char cRecv){
	this->updateList(strStation, 0, strTag, iSetTime, iIndex, cRecv, false);
}

/**********************************************
 * Methode: checkXXX()
 * Info:	Unterfuncktionen des STationscheck
 * Param:
 * Return:
 * Version:
 *	V1.0.0	Erstellt	17.09.2018	JCA
 **********************************************/
void cHandler::checkMissingStations(uint16_t iStation){
	//String msg;
	if(iStation<this->stations.size()){
		if(this->stations[iStation]->id == 0){
			strcpy(this->msgLoop, "HOWIS$");
			strcat(this->msgLoop, this->stations[iStation]->name);
			String msg = this->msgLoop;
			this->mesh->sendBroadcast(msg);
		}
	}
}
void cHandler::checkMissingTags(uint16_t iStation){
	uint16_t y;
	char strZyklus[10];
	bool bSend;
	if(iStation<this->stations.size()){
		if(this->stations[iStation]->id > 0){
			strcpy(this->msgLoop, "REQUEST$");
			strcat(this->msgLoop, this->stations[iStation]->name);
			bSend = false;
			for(y=0;y<this->stations[iStation]->tags.size();y++){
				if(!this->stations[iStation]->tags[y]->found ||!this->stations[iStation]->tags[y]->linked){
					strcat(this->msgLoop, "$");
					strcat(this->msgLoop, this->stations[iStation]->tags[y]->name);
					itoa(this->stations[iStation]->tags[y]->setTime, strZyklus, 10);
					strcat(this->msgLoop, strZyklus);
					if(this->stations[iStation]->tags[y]->recv){
						strcat(this->msgLoop, "S");
					}else{
						strcat(this->msgLoop, "R");
					}
				}
			}
			if(bSend){
				String msg = this->msgLoop;
				this->mesh->sendSingle(this->stations[iStation]->id, msg);
			}
		}
	}
}

/**********************************************
 * Methode: reciveXXX()
 * Info:	Daten Empfangen und auswerten
 * Param:
 *		ID       : NodeID des Absenders
 * Return:
 * Version:
 *	V1.0.0	Erstellt	17.09.2018	JCA
 **********************************************/
void cHandler::receiveDatalist(uint32_t ID){
	char* ptr;
	uint16_t x;
	uint16_t y;
	//Station ermitteln und in Liste suchen
	ptr = strtok(this->msgIn, ";");
	if(ptr != NULL){
		for(x=0;x<this->stations.size();x++){
			if(strcmp(ptr,this->stations[x]->name) == 0){
				this->stations[x]->id = ID;

				//Station gefunden, IO's suchen
				ptr = strtok(NULL, ";");
				while(ptr != NULL){
					for(y=0;y<this->stations[x]->tags.size();y++){
						if(strcmp(ptr,this->stations[x]->tags[y]->name) == 0){
							this->stations[x]->tags[y]->found = true;
							break;
						}
					}
					ptr = strtok(NULL, ";");
				}
				this->checkMissingTags(x);
				break;
			}
		}
	}
}
void cHandler::receiveDatatags(uint32_t ID){
	char* ptr;
	char strName[60];
	char strValue[30];
	uint16_t x;
	uint16_t y;
	//Station in Liste suchen
	for(x=0;x<this->stations.size();x++){
		if(this->stations[x]->id == ID){
			//Station gefunden, IO's suchen
			ptr = strtok(this->msgIn, "$");
			while(ptr != NULL){
				strcpy(strName, ptr);
				ptr = strchr(strName,';');
				if(ptr != NULL){
					*ptr = 0;
					ptr++;
					strcpy(strValue, ptr);
					for(y=0;y<this->stations[x]->tags.size();y++){
						if(strcmp(strName,this->stations[x]->tags[y]->name) == 0 && this->stations[x]->tags[y]->recv){
							this->code->getIO(this->stations[x]->tags[y]->ioIndex)->setString(strValue);
							this->stations[x]->tags[y]->count = 0;
							break;
						}
					}
				}
				ptr = strtok(NULL, "$");
			}
			break;
		}
	}
}
void cHandler::receiveRequest(uint32_t ID){
	char* ptr;
	char strStation[30];
	char strName[60];
	char strValue[30];
	uint16_t x;
	uint16_t y;
	bool bAdded= false;
	strcpy(this->msgOut,"LINKED$");
	//Datensätze druchlaufen
	ptr = strtok(this->msgIn, "$");
	if(ptr != NULL){
		strcpy(strStation, ptr);
		ptr = strtok(NULL, "$");
	}
	while(ptr != NULL){
		strcpy(strName, ptr);
		ptr = strchr(strName,';');
		if(ptr != NULL){
			*ptr = 0;
			ptr++;
			strcpy(strValue, ptr);
			ptr = strchr(strValue,';');
			if(ptr != NULL){
				*ptr = 0;
				ptr++;
				if(this->updateList(strStation, ID, strName, atoi(strValue), this->code->findIO(strName), *ptr, true)){
					if(bAdded){strcat(this->msgOut,";");}
					strcat(this->msgOut, strName);
					bAdded = true;
				}
			}
		}
		ptr = strtok(NULL, "$");
	}
	if(bAdded){
		String dummy = String(this->msgOut);
		this->mesh->sendSingle(ID, dummy);
	}
}
void cHandler::receiveLinked(uint32_t ID){
	char* ptr;
	uint16_t x;
	uint16_t y;
	//Station ermitteln und in Liste suchen
	for(x=0;x<this->stations.size();x++){
		if(this->stations[x]->id == ID){

			//Station gefunden, IO's suchen
			ptr = strtok(this->msgIn, ";");
			while(ptr != NULL){
				for(y=0;y<this->stations[x]->tags.size();y++){
					if(strcmp(ptr,this->stations[x]->tags[y]->name) == 0){
						this->stations[x]->tags[y]->found = true;
						this->stations[x]->tags[y]->linked = true;
						break;
					}
				}
				ptr = strtok(NULL, ";");
			}

			break;
		}
	}
}
void cHandler::receiveHowIs(uint32_t ID){
	uint16_t x;
	for(x=0;x<this->stations.size();x++){
		if(this->stations[x]->id == this->mesh->getNodeId()){
			if(strcmp(this->msgIn, this->stations[x]->name) == 0){
				if(this->genTagList()){
					String dummy = String(this->msgOut);
					this->mesh->sendSingle(ID,dummy);
				}
			}
			break;
		}
	}
}

/**********************************************
 * Methode: updateList()
 * Info:	aktuallisiert die Matchliste der Node
 *				und fügt den Datenpunkt hin zu falls
 *				nicht vorhanden
 * Param:
 *		strStation : stations Name
 *		ID		: stations-ID der gegenstelle
 *		strTag	: Name des tags
 *		iSetTime: Sendeintervall/Watchdog des tags
 *		iIndex	: index im lokalen IO-Vektor
 *		cRecv	:
 * Return:
 * Version:
 *	V1.0.0	Erstellt	17.09.2018	JCA
 **********************************************/
bool cHandler::updateList(char* strStation, uint32_t ID, char* strTag, int iSetTime, int iIndex, char cRecv, bool bRequest){
	if(iIndex >= 0){
		uint16_t x;
		uint16_t y;
		bool sFound = false;
		bool tFound = false;
		bool bRecv;
		bRecv = cRecv == 'R';
		// Station suchen
		for(x=0;x<this->stations.size();x++){
			if(strcmp(strStation,this->stations[x]->name) == 0){
				this->stations[x]->id = ID;

				//tagsuchen
				for(y=0;y<this->stations[x]->tags.size();y++){
					if(strcmp(strTag,this->stations[x]->tags[y]->name) == 0 && this->stations[x]->tags[y]->recv == bRecv){
						this->stations[x]->tags[y]->ioIndex = iIndex;
						this->stations[x]->tags[y]->setTime = iSetTime;
						if(bRequest){
							this->stations[x]->tags[y]->found = true;
							this->stations[x]->tags[y]->linked = true;
						}
						tFound = true;
						break;
					}
				}

				// neuen Tag anlegen falls nicht gefunden
				if(!tFound){
					this->stations[x]->tags.push_back(new cTag(strTag, iSetTime, iIndex, bRecv, bRequest));
				}

				sFound = true;
				break;
			}
		}

		//neue Station anlegen falls nicht gefunden (+Tag)
		if(!sFound){
			this->stations.push_back(new cStation(strStation));
			x=this->stations.size() - 1;
			this->stations[x]->id = ID;
			this->stations[x]->tags.push_back(new cTag(strTag, iSetTime, iIndex, bRecv, bRequest));
		}
		return true;
	}else{
		return false;
	}
}

/**********************************************
 * Methode: getTagList()
 * Info:	Erzeugt die komplette Datenlist der Node
 * Param:
 * Return:
 * Version:
 *	V1.0.0	Erstellt	17.09.2018	JCA
 **********************************************/
bool cHandler::genTagList(){
	uint16_t x;
	bool found = false;
	strcpy(this->msgOut, "DATALIST$");
	for(x=0;x<this->stations.size();x++){
		if(this->stations[x]->id == this->mesh->getNodeId()){
			strcat(this->msgOut, this->stations[x]->name);
			found = true;
			break;
		}
	}
	if(found){
		for(x=0;x<this->code->getIoSize();x++){
			strcat(this->msgOut, ";");
			strcat(this->msgOut, this->code->getIO(x)->getName());
		}
	}
	return found;
}
