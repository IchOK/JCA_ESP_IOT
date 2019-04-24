/**********************************************
 * Beschreibung: siehe Header File
 **********************************************/

#include <JCA_IOT_PARAM.h>

using namespace JCA::IOT;
/**********************************************
 * Methode: Constructor(const char* strName)
 * Info:	Initialisierung der statischen Werte
 *				und übergibt den Namen
 * Param:
 *		strName		Name des Parameters
 *		iType		Type des Parameters als String
 * Version:
 *	V1.0.0	Erstellt	17.04.2019	JCA
 **********************************************/
cParam::cParam(const char* strName, int iType){
	strcpy(this->Name,strName);
	this->Type = iType;
}

/**********************************************
 * Methode: getType()
 * Info:	Gibt den Type des Parameters zurück
 * Param:
 * Return:	Typekennung als [int]
 * Version:
 *	V1.0.0	Erstellt	17.04.2019	JCA
 **********************************************/
int cParam::getType(){
	return this->Type;
}

/**********************************************
 * Methode: getName()
 * Info:	gibt den Name des Parameters zurück
 * Param:
 * Return:	Parameter-Name als [String]
 * Version:
 *	V1.0.0	Erstellt	17.04.2019	JCA
 **********************************************/
char* cParam::getName(){
	return this->Name;
}

/**********************************************
 * Methode: Get/Set...()
 * Info:	Wert schreiben oder auslesen
 * Param:	void / Wert
 * Return:	Wert / void
 * Version:
 *	V1.0.0	Erstellt	19.05.2018	JCA
 **********************************************/
float cParam::getFloat(){return this->fValue;}
int32_t  cParam::getInt(){return this->iValue;}
bool  cParam::getBool(){return (bool)this->iValue;}
bool	cParam::setValue(char* strValue){
	bool retValue = false;
	switch(this->Type){
		case JCA_IOT_PARAM_T_FLOAT :
			if(isdigit(strValue)){
				this->fValue = atof(strValue);
				retValue = true;
			}
			break;
		case JCA_IOT_PARAM_T_INT :
			if(isdigit(strValue)){
				this->iVlaue = atoi(strValue);
				retValue = true;
			}
			break;
		case JCA_IOT_PARAM_T_BOOL :
			if(isdigit(strValue)){
				if(atoi(strValue) == 0){
					this->iValue = 0;
				}else{
					this->iValue = 1;
				}
				retValue = true;
			}else{
				for (int i = 0; i<strlen(strValue); i++){
					strValue[i] = tolower(strValue[i]);
				}
				if (strcmp("true",strValue) == 0){
					this->iValue = 1;
					retValue = true;
				}
				if (strcmp("false",strValue) == 0){
					this->iValue = 0;
					retValue = true;
				}
			}
			break;
	}
	return retValue;
}
