/**********************************************
 * Beschreibung: siehe Header File
 **********************************************/

#include <JCA_IOT_ELEMENT.h>

using namespace JCA::IOT::ELEMENT;
/**********************************************
 * Methode: Constructor(const JsonObject* data)
 * Info:	Initialisierung des Elements, dies
 *          muss im Child-Element erfolgen da
 *          immer zuerst der Parent-Constructor
 *          aufgerufen wird und die Bestandteile
 *          des Elements zu diesem Zeitpunkt
 *          noch nicht existieren.
 *
 * Param:
 *		data: Im Json-Object ist die Konfiguration
 *            des Elements abgelegt
 * Version:
 *	V1.0.0	Erstellt	27.04.2019	JCA
 **********************************************/
cRoot::cRoot(const JsonObject* data){
}

/**********************************************
 * Methode: addParam(...)
 * Info:	Konstruktorfunktion, erzeugt einen
 *          Paramenter im Vector zur späteren
 *          Verwendung.
 * Param:
 *  name:     Name des Parameters
 *  dataType: Datentyp des Parameters
 * Return:	true = erfolgreich erstellt
 * Version:
 *	V1.0.0	Erstellt	27.04.2019	JCA
 **********************************************/
bool cRoot::addParam(const char* name, const unsigned char dataType){
	if(strlen(name) > JCA_IOT_ELEMENT_NAME_LEN){
    return false;
  }else{
    tData dummy;
    strcpy(dummy.Name, name);
    dummy.Type = dataType;
    dummy.QC = JCA_IOT_QC_CREAT;
    this->Params.push_back(dummy);
    return true;
  }
}

/**********************************************
 * Methode: addValue(...)
 * Info:	Konstruktorfunktion, erzeugt einen
 *          Wert im Vector zur späteren
 *          Verwendung.
 * Param:
 *  name:     Name des Werts
 *  dataType: Datentyp des Werts
 * Return:	true = erfolgreich erstellt
 * Version:
 *	V1.0.0	Erstellt	27.04.2019	JCA
 **********************************************/
bool cRoot::addValue(const char* name, const unsigned char dataType){
	if(strlen(name) > JCA_IOT_ELEMENT_NAME_LEN){
    return false;
  }else{
    tData dummy;
    strcpy(dummy.Name, name);
    dummy.Type = dataType;
    dummy.QC = JCA_IOT_QC_CREAT;
    this->Values.push_back(dummy);
    return true;
  }
}

/**********************************************
 * Methode: addInterface(...)
 * Info:	Konstruktorfunktion, erzeugt eine
 *          Schnittstelle im Vector zur späteren
 *          Verwendung.
 * Param:
 *  name:     Name der Schnittsteller
 *  dataType: Datentyp der Schnittstelle
 * Return:	true = erfolgreich erstellt
 * Version:
 *	V1.0.0	Erstellt	27.04.2019	JCA
 **********************************************/
bool cRoot::addInterface(const char* name, const unsigned char dataType, const unsigned char dir){
	if(strlen(name) > JCA_IOT_ELEMENT_NAME_LEN){
    return false;
  }else{
    tData dummy;
    strcpy(dummy.Name, name);
    dummy.Type = dataType;
    dummy.Dir = dir;
    this->Interfaces.push_back(dummy);
    return true;
  }
}

/**********************************************
 * Methode: writeData(...)
 * Info:	Standardfunktion, die Funktion wird sowohl
 *          vom Constructor als auch von der Runtime
 *          verwendet um die Vectore zu parametrieren.
 *        Zusätzlich wird der Remotevector erzeugt.
 * Param:
 *  data:     Json-Object das die Konfiguration enthält.
 *              Es kann nur ein Datenpunkt oder das
 *              komplette Element enthalten sein.
 *  caller:   Definiert den Ursprung der Daten, dies
 *              dient dazu die Schreibrechte sicher
 *              zu stellen
 * Return:	true = erfolgreich erstellt
 * Version:
 *	V1.0.0	Erstellt	27.04.2019	JCA
 **********************************************/
bool cRoot::writeData(const JsonObject* data, const unsigned char caller){
  //prüfen ob Parameter in den Daten enthalten sind
  if(data->containsKey("params")){
    for (JsonObject value : data["params"].as<JsonArray>()) {
      //Finde den Wert im Vector
      bool bFound = false;
      for(i=0; i<this->Params.size(); i++){
        if(strcmp(this->Params[i]->Name,value["name"])==0){
					bool enValue;
          if(caller == JCA_IOT_ELEMENT_WRITE_CREAT){
            if(value.containsKey("write")){
              this->Params[i]->Write = value["write"];
            }
						enValue = true;
          }else{
						enValue = (this->Params[i]->Write & caller) == caller;
          }
					if(value.containsKey("value") && enValue){
						switch(this->Params[i]->Type){
							case JCA_IOT_ELEMENT_DATA_BOOL :
								this->Params[i]->iValue = value["value"].as<bool>;
								break;
							case JCA_IOT_ELEMENT_DATA_INT :
								this->Params[i]->iValue = value["value"].as<int32_t>;
								break;
							case JCA_IOT_ELEMENT_DATA_FLOAT :
								this->Params[i]->fValue = value["value"].as<float>;
								break;
						}
					}
        }
      }
    }
  }
	//prüfen ob Istwerte in den Daten enthalten sind
  if(data->containsKey("values")){
    for (JsonObject value : data["values"].as<JsonArray>()) {
      //Finde den Wert im Vector
      bool bFound = false;
      for(i=0; i<this->Values.size(); i++){
        if(strcmp(this->Values[i]->Name,value["name"])==0){
					bool enValue;
          if(caller == JCA_IOT_ELEMENT_WRITE_CREAT){
            if(value.containsKey("write")){
              this->Values[i]->Write = value["write"];
            }
						enValue = true;
          }else{
						enValue = (this->Values[i]->Write & caller) == caller;
          }
					if(value.containsKey("value") && enValue){
						switch(this->Values[i]->Type){
							case JCA_IOT_ELEMENT_DATA_BOOL :
								this->Values[i]->iValue = value["value"].as<bool>;
								break;
							case JCA_IOT_ELEMENT_DATA_INT :
								this->Values[i]->iValue = value["value"].as<int32_t>;
								break;
							case JCA_IOT_ELEMENT_DATA_FLOAT :
								this->Values[i]->fValue = value["value"].as<float>;
								break;
						}
						if(caller == JCA_IOT_ELEMENT_WRITE_CREAT){
	          	this->Values[i]->QC = JCA_IOT_QC_INIT;
						}else{
							this->Values[i]->QC = JCA_IOT_QC_GOOD;
						}
					}
        }
      }
    }
  }
	//prüfen ob Interfacebeschreibungen in den Daten enthalten sind
  if(data->containsKey("interfaces")){
		if(caller == JCA_IOT_ELEMENT_WRITE_CREAT || caller == JCA_IOT_ELEMENT_WRITE_UPDATE){
	    for (JsonObject value : data["interfaces"].as<JsonArray>()) {
	      //Finde den Wert im Vector
	      bool bFound = false;
	      for(i=0; i<this->Interfaces.size(); i++){
	        if(strcmp(this->Interfaces[i]->Name,value["name"])==0){
						if(value.containsKey("area")){
              this->Interfaces[i]->Area = value["area"];
            }
						if(value.containsKey("ielement")){
              this->Interfaces[i]->Element = value["ielement"];
            }
						if(value.containsKey("ivalue")){
              this->Interfaces[i]->Value = value["ivalue"];
            }
					}
				}
			}
		}
	}
	//prüfen ob Kommunikationen in den Daten enthalten sind
  if(data->containsKey("remotes")){
		if(caller == JCA_IOT_ELEMENT_WRITE_CREAT || caller == JCA_IOT_ELEMENT_WRITE_UPDATE){
	    for (JsonObject value : data["remotes"].as<JsonArray>()) {
	      //Finde den Wert im Vector
	      bool bFound = false;
	      for(i=0; i<this->Remotes.size(); i++){
	        if(strcmp(this->Remotes[i]->Name,value["name"])==0){
						bFound = true;
						if(value.containsKey("xstation")){
							if(strlen(value["xstation"]) <= JCA_IOT_ELEMENT_NAME_LEN){
              	strcpy(this->Remotes[i]->Station, value["xstation"]);
							}
            }
						if(value.containsKey("xelement")){
							if(strlen(value["xelement"]) <= JCA_IOT_ELEMENT_NAME_LEN){
              	strcpy(this->Remotes[i]->Element, value["xelement"]);
							}
            }
						if(value.containsKey("xvalue")){
							if(strlen(value["xvalue"]) <= JCA_IOT_ELEMENT_NAME_LEN){
              	strcpy(this->Remotes[i]->Datapoint, value["xvalue"]);
							}
            }
						if(value.containsKey("dir")){
              this->Remotes[i]->Dir = value["dir"];
            }
						if(value.containsKey("ivalue")){
              this->Remotes[i]->Value = value["ivalue"];
            }
						if(value.containsKey("iinterface")){
              this->Remotes[i]->Interface = value["iinterface"];
            }
					}
				}
			}
			//nicht gefunden also anlegen
			if(!bFound){
				if(value.containsKey("xstation")
						&& value.containsKey("xelement")
						&& value.containsKey("xvalue")
						&& value.containsKey("dir")
						&& value.containsKey("ivalue")
						&& value.containsKey("iinterface")
					){
					if(strlen(value["xstation"]) <= JCA_IOT_ELEMENT_NAME_LEN
							&& strlen(value["xelement"]) <= JCA_IOT_ELEMENT_NAME_LEN
							&& strlen(value["xvalue"]) <= JCA_IOT_ELEMENT_NAME_LEN
						){
						tRemote dummy;
						strcpy(dummy.Station, value["xstation"]);
						strcpy(dummy.Element, value["xelement"]);
						strcpy(dummy.Datapoint, value["xvalue"]);
						dummy.Dir = value["dir"];
						dummy.Value = value["ivalue"];
						dummy.Interface = value["iinterface"];
						this->Remotes.push_back(dummy);
					}
				}
			}
		}
	}
}

/**********************************************
 * Methode: inInterface(...)
 * Info:	Runtimefunktion, wir vom Standard aufgerufen
 *					und liest die Interfaces des Elements ein.
 * Param:
 * Return:	true = erfolgreich erstellt
 * Version:
 *	V1.0.0	Erstellt	02.05.2019	JCA
 **********************************************/
bool cRoot::inInterface(std::vector<cRoot*> *IOs, std::vector<cRoot*> *Funcs, std::vector<cRoot*> *Meshs){
	std::vector<cRoot*> *Area;
	for(i=0; i<this->Interfaces.size(); i++){
		if(JCA_IOT_ELEMENT_DIR_IS_IN(this->Interfaces[i].Dir)){
			switch(tihs->Interfaces[i].Area){
				case JCA_IOT_ELEMENT_RANGE_IO:
						Area = IOs;
					break;
				case JCA_IOT_ELEMENT_RANGE_FUNC:
						Area = Funcs;
					break;
				case JCA_IOT_ELEMENT_RANGE_MESH:
						Area = Meshs;
					break;
				default:
					Area = NULL;
					continue;
			}
			if(Area->size() >= this->Interfaces[i].Element){
				switch(this->Interfaces[i].Type){
					case JCA_IOT_ELEMENT_DATA_BOOL:
						bool Value;
						if(Area[this->Interfaces[i].Element]->getValueBool(this->Interfaces[i].Value, &Value)){
							this->Interfaces[i].bValue = Value;
						}
						break;
					case JCA_IOT_ELEMENT_DATA_INT:
						int32_t Value;
						if(Area[this->Interfaces[i].Element]->getValueInt(this->Interfaces[i].Value, &Value)){
							this->Interfaces[i].iValue = Value;
						}
						break;
					case JCA_IOT_ELEMENT_DATA_FLOATL:
						float Value;
						if(Area[this->Interfaces[i].Element]->getValueFloat(this->Interfaces[i].Value, &Value)){
							this->Interfaces[i].fValue = Value;
						}
						break;
				}
			}
		}
	}
}

/**********************************************
 * Methode: outInterface(...)
 * Info:	Runtimefunktion, wir vom Standard aufgerufen
 *					und liest die Interfaces des Elements ein.
 * Param:
 * Return:	true = erfolgreich erstellt
 * Version:
 *	V1.0.0	Erstellt	02.05.2019	JCA
 **********************************************/
bool cRoot::outInterface(std::vector<cRoot*> *IOs, std::vector<cRoot*> *Funcs, std::vector<cRoot*> *Meshs){
	std::vector<cRoot*> *Area;
	for(i=0; i<this->Interfaces.size(); i++){
		if(JCA_IOT_ELEMENT_DIR_IS_OUT(this->Interfaces[i].Dir)){
			switch(tihs->Interfaces[i].Area){
				case JCA_IOT_ELEMENT_RANGE_IO:
						Area = IOs;
					break;
				case JCA_IOT_ELEMENT_RANGE_FUNC:
						Area = Funcs;
					break;
				case JCA_IOT_ELEMENT_RANGE_MESH:
						Area = Meshs;
					break;
				default:
					Area = NULL;
					continue;
			}
			if(Area->size() >= this->Interfaces[i].Element){
				switch(this->Interfaces[i].Type){
					case JCA_IOT_ELEMENT_DATA_BOOL:
						Area[this->Interfaces[i].Element]->setValueBool(this->Interfaces[i].Value, this->Interfaces[i].bValue));
						break;
					case JCA_IOT_ELEMENT_DATA_INT:
						Area[this->Interfaces[i].Element]->setValueInt(this->Interfaces[i].Value, this->Interfaces[i].iValue));
						break;
					case JCA_IOT_ELEMENT_DATA_FLOATL:
						Area[this->Interfaces[i].Element]->setValueFloat(this->Interfaces[i].Value, this->Interfaces[i].fValue));
						break;
				}
			}
		}
	}
}
