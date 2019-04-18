/**********************************************
 * Beschreibung: siehe Header File
 **********************************************/

#include <JCA_IOT_FUNC.h>

using namespace JCA::IOT::FUNC;
/**********************************************
 * Methode: Constructor(const char* strName)
 * Info:	Initialisierung der statischen Werte
 *				und übergibt den Namen
 * Param:
 *		strName	Name der Funktion
 *		iType		Typekennung der Funktion
 *		vIOs		Zeiger auf den IO-Vector
 * Version:
 *	V1.0.0	Erstellt	17.04.2019	JCA
 **********************************************/
cRoot::cRoot(const char* strName, int iType, std::vector<JCA::IOT::IO::cRoot*>& vIOs) : IOs(vIOs){
	strcpy(this->Name,strName);
	this->Type = iType;
	this->Error = 0;
}

/**********************************************
 * Methode: getConfig()
 * Info:	gibt die Konfiguration der Funktion zurück
 * Param:		ptrConfig [JsonObject] Konfiguration wird angehängt
 * Return:	Bearbeitung ok [bool]
 * Version:
 *	V1.0.0	Erstellt	17.04.2019	JCA
 **********************************************/
bool cRoot::getConfig(JsonObject* ptrConfig){
	ptrConfig["name"] = this->Name;
	ptrConfig["type"] = this->Type;
	return true;
}

/**********************************************
 * Methode: getState()
 * Info:	gibt den Status aller Parameter
 * 						der Funktion zurück.
 * Param:		ptrConfig [JsonObject] Konfiguration wird angehängt
 * Return:	Bearbeitung ok [bool]
 * Version:
 *	V1.0.0	Erstellt	17.04.2019	JCA
 **********************************************/
bool cRoot::getState(JsonObject* ptrState){
	ptrState["name"] = this->Name;
	ptrState["type"] = this->Type;
	JsonArray jParams = ptrState->createNestedArray("param");
	for (int i = 0; i < this->Params.size(); i++){
		JsonObject jParam = jParams.createNestedObject();
		jParam["name"] = this->Params[i].getName();
		int iType = this->Params[i].getType();
		jParam["type"] = iType;
		switch(iType) {
			case JCA_IOT_FUNC_PARAM_T_FLOAT :
				jParam["value"] = this->Params[i].getFloat();
				break;
			case JCA_IOT_FUNC_PARAM_T_INT :
				jParam["value"] = this->Params[i].getInt();
				break;
			case JCA_IOT_FUNC_PARAM_T_BOOL :
				jParam["value"] = this->Params[i].getBool();
				break;
		}
	}
	JsonArray jInterfaces = ptrState->createNestedArray("interface");
	for (int i = 0; i < this->Interface.size(); i++){
		JsonObject jInterface = jInterfaces.createNestedObject();
		jParam["name"] = this->Interface[i].getName();
		int iType = this->Interface[i].getType();
		jParam["type"] = iType;
		switch(iType) {
			case JCA_IOT_FUNC_PARAM_T_FLOAT :
				jParam["value"] = this->IOs[this->Interface[i].getInt()].getFloat();
				break;
			case JCA_IOT_FUNC_PARAM_T_INT :
				jParam["value"] = this->IOs[this->Interface[i].getInt()].getInt();
				break;
			case JCA_IOT_FUNC_PARAM_T_BOOL :
				jParam["value"] = this->IOs[this->Interface[i].getInt()].getBool();
				break;
		}
	}
	return true;
}

/**********************************************
 * Methode: update()
 * Info:	Vorlage zum verarbeiten der IO-Daten
 *				Standardmethode für Child-Klassen
 * Param:
 * Return:
 * Version:
 *	V1.0.0	Erstellt	19.05.2018	JCA
 **********************************************/
void cRoot::update(){
	;
}

/**********************************************
 * Methode: Get/Set...()
 * Info:	Wert schreiben oder auslesen
 * Param:	void / Wert
 * Return:	Wert / void
 * Version:
 *	V1.0.0	Erstellt	19.05.2018	JCA
 **********************************************/
void cRoot::setString(char* strValue){}
char* cRoot::getString(char* strValue){
	strcpy(strValue,"");
	return strValue;
	}
void  cRoot::setFloat(float Value){}
float cRoot::getFloat(){return 0.0;}
void  cRoot::setTime(int32_t Value){}
int32_t  cRoot::getTime(){return 0;}
void  cRoot::setLong(int32_t Value){}
int32_t  cRoot::getLong(){return 0;}
void  cRoot::setBool(bool Value){}
bool  cRoot::getBool(){return false;}
void  cRoot::setDate(int32_t Value){}
int32_t  cRoot::getDate(){return 0;}
