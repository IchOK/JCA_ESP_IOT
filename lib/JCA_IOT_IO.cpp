/**********************************************
 * Beschreibung: siehe Header File
 **********************************************/

#include <JCA_IOT_IO.h>

using namespace JCA::IOT::IO;
/**********************************************
 * Methode: Constructor(const char* strName)
 * Info:	Initialisierung der statischen Werte
 *				und übergibt den Namen
 * Param:	
 *		strName		Name des IO's
 *		iType		Type des IO's als String
 * Version:
 *	V1.0.0	Erstellt	19.05.2018	JCA
 **********************************************/
cRoot::cRoot(const char* strName, int iType){
	strcpy(this->Name,strName);
	this->Type = iType;
}

/**********************************************
 * Methode: getType()
 * Info:	Gibt den Type des IO' zurück
 * Param:	
 * Return:	Typekennung als [int]
 * Version:
 *	V1.0.0	Erstellt	19.05.2018	JCA
 **********************************************/
int cRoot::getType(){
	return this->Type;
}

/**********************************************
 * Methode: getName()
 * Info:	gibt den Name des IO's zurück
 * Param:	
 * Return:	IO-Name als [String]
 * Version:
 *	V1.0.0	Erstellt	19.05.2018	JCA
 **********************************************/
char* cRoot::getName(){
	return this->Name;
	this->QC = JCA_IOT_QC_GOOD;
}

/**********************************************
 * Methode: getQC()
 * Info:	gibt den Zustand des IO's zurück
 * Param:	
 * Return:	
 *		bool = QC==0x80
 * Version:
 *	V1.0.0	Erstellt	19.05.2018	JCA
 **********************************************/
unsigned char cRoot::getQC(){
	return this->QC;
}
/**********************************************
 * Methode: setQC(unsigned char uQC)
 * Info:	setzt den QualityCode des Datenpunkts extern
 * Param:	
 * Version:
 *	V1.0.0	Erstellt	14.09.2018	JCA
 **********************************************/
void cRoot::setQC(unsigned char uQC){
	this->QC = uQC;
}
/**********************************************
 * Methode: Good()
 * Info:	Prüft ob der Datenpunkt ok ist (QC)
 * Param:	
 * Return:	
 *		bool = QC==0x80
 * Version:
 *	V1.0.0	Erstellt	14.09.2018	JCA
 **********************************************/
bool cRoot::Good(){
	return (this->QC & JCA_IOT_QC_GOOD) == JCA_IOT_QC_GOOD;
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
