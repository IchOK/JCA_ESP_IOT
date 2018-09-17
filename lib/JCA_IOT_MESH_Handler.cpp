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
	this->Mesh = xMesh;
	this->Code = xCode;
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
	int x;
	int y;
	bool bSend;
	char buffer[30];
	for(x=0;x<this->Stations.size();x++){
		if(this->Stations[x]->ID != this->Mesh->getNodeId()){
			//Stations Watchdog
			if(this->Stations[x]->WatchDog > JCA_IOT_MESH_STATION_WD){
				this->Stations[x]->ID = 0; //Station nicht mehr vorhanden
			}else{
				this->Stations[x]->WatchDog += ulMicros;
			}
			
			bSend = false;
			strcpy(this->msgOut, "DATA$");
			//Tags jeder Station durchlaufen
			for(y=0;this->Stations[x]->Tags.size();y++){
				if(this->Stations[x]->Tags[y]->Count > this->Stations[x]->Tags[y]->SetTime){
					if(this->Stations[x]->Tags[y]->Recv){
						this->Code->getIO(this->Stations[x]->Tags[y]->ioIndex)->setQC(JCA_IOT_QC_COMMLAST);
					}else{
						if(bSend){strcat(this->msgOut, "$");}
						strcat(this->msgOut, this->Stations[x]->Tags[y]->Name);
						strcat(this->msgOut, ";");
						strcat(this->msgOut, this->Code->getIO(this->Stations[x]->Tags[y]->ioIndex)->getString(buffer));
						bSend = true;
						this->Stations[x]->Tags[y]->Count = 0;
					}
				}else{
					this->Stations[x]->Tags[y]->Count += ulMicros;
				}
			}
			if(bSend && this->Mesh->isConnected(this->Stations[x]->ID)){
				String dummy = String(this->msgOut);
				this->Mesh->sendSingle(this->Stations[x]->ID, dummy);
			}
		}
	}
}

/**********************************************
 * Methode: helloAll()
 * Info:	Broadcast mit Stationsname und allen Tags
 * Param:
 * Return:	
 * Version:
 *	V1.0.0	Erstellt	17.09.2018	JCA
 **********************************************/
void cHandler::helloAll(){
	if(this->genTagList()){
		String dummy = String(this->msgOut);
		this->Mesh->sendBroadcast(dummy);
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
 * Methode: checkStations()
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
	switch(this->checkMode){
		case 0:	//nicht verlinkte Stationen prüfen
			checkMissingStations();
			this->checkMode = 1
			break;

		case 1:	//nicht gefundene Tags prüfen
			checkMissingTags();
			this->checkMode = 2
			break;
			
		case 2:	//nicht verknüpfte Tags prüfen
			checkUnlinkedTags();
			this->checkMode = 0;
			break;
		default:
			this->checkMode = 0;
			break;
	}
}


//void cHandler::AddToList(char* strStation, char* strTag, int iSetTime, int iIndex, bool bRecv);//Datenpunkt in Liste eintragen


/**********************************************
 * Methode: checkXXX()
 * Info:	Unterfuncktionen des STationscheck
 * Param:
 * Return:	
 * Version:
 *	V1.0.0	Erstellt	17.09.2018	JCA
 **********************************************/




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
	int x;
	int y;
	//Station ermitteln und in Liste suchen
	ptr = strtok(this->msgIn, ";");
	if(ptr != NULL){
		for(x=0;x<this->Stations.size();x++){
			if(strcmp(ptr,this->Stations[x]->Name) == 0){
				this->Stations[x]->ID = ID;
				
				//Station gefunden, IO's suchen
				ptr = strtok(NULL, ";");
				while(ptr != NULL){
					for(y=0;y<this->Stations[x]->Tags.size();y++){
						if(strcmp(ptr,this->Stations[x]->Tags[y]->Name) == 0){
							this->Stations[x]->Tags[y]->Found = true;
							break;
						}
					}
					ptr = strtok(NULL, ";");
				}
				
				break;
			}
		}
	}
}

void cHandler::receiveDatatags(uint32_t ID){
	char* ptr;
	char strName[60];
	char strValue[30];
	int x;
	int y;
	//Station in Liste suchen
	for(x=0;x<this->Stations.size();x++){
		if(this->Stations[x]->ID == ID){
			//Station gefunden, IO's suchen
			ptr = strtok(thid->msgIn, "$");
			while(ptr != NULL){
				strcpy(strName, ptr);
				ptr = strchr(strName,';');
				if(ptr != NULL){
					*ptr = 0;
					ptr++;
					strcpy(strValue, ptr);
					for(y=0;y<this->Stations[x]->Tags.size();y++){
						if(strcmp(strName,this->Stations[x]->Tags[y]->Name) == 0 && this->Stations[x]->Tags[y]->Recv){
							this->Code->getIO(this->Stations[x]->Tags[y]->ioIndex)->setString(strValue);
							this->Stations[x]->Tags[y]->Count = 0;
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
	int x;
	int y;
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
				if(this->updateList(strStation, ID, strName, atoi(strValue), this->Code->findIO(strName), *ptr)){
					if(bAdded){strcat(this->msgOut,";");}
					strcat(this->msgOut, strName);
				}
			}
		}
		ptr = strtok(NULL, "$");
	}
	if(bAdded){
		String dummy = String(this->msgOut);
		this->Mesh->sendSingle(ID, dummy);
	}
}
	
void cHandler::receiveLinked(uint32_t ID){
	char* ptr;
	int x;
	int y;
	//Station ermitteln und in Liste suchen
	for(x=0;x<this->Stations.size();x++){
		if(this->Stations[x]->ID) == ID){
		
			//Station gefunden, IO's suchen
			ptr = strtok(this->msgIn, ";");
			while(ptr != NULL){
				for(y=0;y<this->Stations[x]->Tags.size();y++){
					if(strcmp(ptr,this->Stations[x]->Tags[y]->Name) == 0){
						this->Stations[x]->Tags[y]->Linked = true;
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
	int x;
	for(x=0;x<this->Stations.size();x++){
		if(this->Stations[x]->ID == this->Mesh->getNodeId()){
			if(strcmp(this->msgIn, this->Stations[x]->Name) == 0){
				if(this->genTagList()){
					String dummy = String(this->msgOut);
					this->Mesh->sendSingle(ID,dummy);
				}
			}
			break;
		}
	}
}
	


//void cHandler::anyThere(char* strStation);
//void cHandler::iAm(char* strDataList);				
//void cHandler::giveMe(int iStationId);

/**********************************************
 * Methode: updateList()
 * Info:	aktuallisiert die Matchliste der Node
 *				und fügt den Datenpunkt hin zu falls
 *				nicht vorhanden
 * Param:
 *		strStation : Stations Name
 *		ID		: Stations-ID der gegenstelle
 *		strTag	: Name des Tags
 *		iSetTime: Sendeintervall/Watchdog des Tags
 *		iIndex	: index im lokalen IO-Vektor
 *		cRecv	: 
 * Return:	
 * Version:
 *	V1.0.0	Erstellt	17.09.2018	JCA
 **********************************************/
bool cHandler::updateList(char* strStation, uint32_t ID, char* strTag, int iSetTime, int iIndex, char cRecv){
	if(iIndex >= 0){
		int x;
		int y;
		bool sFound = false;
		bool tFound = false;
		bool bRecv;
		bRecv = cRecv == 'R';
		// Station suchen
		for(x=0;x<this->Stations.size();x++){
			if(strcmp(strStation,this->Stations[x]->Name) == 0){
				this->Stations[x]->ID = ID;
				
				//Tagsuchen
				for(y=0;y<this->Stations[x]->Tags.size();y++){
					if(strcmp(strTag,this->Stations[x]->Tags[y]->Name) == 0 && this->Stations[x]->Tags[y]->Recv == bRecv){
						this->Stations[x]->Tags[y]->ioIndex = iIndex;
						this->Stations[x]->Tags[y]->SetTime = iSetTime;
						this->Stations[x]->Tags[y]->Found = true;
						
						tFound = true;
						break;
					}
				}
				
				// neuen Tag anlegen falls nicht gefunden
				if(!tFound){
					this->Stations[x]->Tags.push_back(new cTag(strTag, iSetTime, iIndex, bRecv));
				}
				
				sFound = true;
				break;
			}
		}
		
		//neue Station anlegen falls nicht gefunden (+Tag)
		if(!sFound){
			this->Stations.push_back(new cStation(strStation));
			x=this->Stations.size() - 1;
			this->Stations[x]->ID = ID;
			this->Stations[x]->Tags.push_back(new cTag(strTag, iSetTime, iIndex, bRecv));
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
	int x;
	bool found = false;
	strcpy(this->msgOut, "DATALIST$");
	for(x=0;x<this->Stations.size();x++){
		if(this->Stations[x]->ID == this->Mesh->getNodeId()){
			strcat(this->msgOut, this->Stations[x]->Name);
			found = true;
			break;
		}
	}
	if(found){
		for(x=0;x<this->Code->getIoSize();x++){
			strcat(this->msgOut, ";");
			strcat(this->msgOut, this->Code->getIO(x)->getName());
		}
	}
	return found;
}
	

