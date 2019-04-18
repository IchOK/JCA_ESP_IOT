/**********************************************
 * Beschreibung: siehe Header File
 **********************************************/

#include <JCA_IOT_FUNC_ClockSwitch.h>
using namespace JCA::IOT::FUNC;

/**********************************************
 * Methode: Constructor(const char* strName)
 * Info:	Initialisierung der Parameter und
 *				Schnittstellen und übergibt den Namen.
 * Param:
 *		strName	Name der Funktion
 *		vIOs		Zeiger auf den IO-Vector
 *		iConfig Modus der Funktion
 *		iTime1	Einschaltzeit / Startzeit
 *		iTime2	Ausschaltzeit / Dauer
 *		iIndexOutput	Zeiger auf das mit dem
 *			Ausgang verknüpften IO
 * Version:
 *	V1.0.0	Erstellt	17.04.2019	JCA
 **********************************************/
cClockSwitch::cClockSwitch(const char* strName, std::vector<JCA::IOT::IO::cRoot*>& vIOs, char* iConfig, char* iTime1, char* iTime2, int iIndexOutput) : cRoot(strName, JCA_IOT_FUNC_T_CLOCKSWITCH, vIOs){
	//Parameter anlegen
	this->Params.push_back(new cParam("Config", JCA_IOT_FUNC_PARAM_T_INT));
	if (!this->Params[0].setValue(iConfig)){
		this->Error = JCA_IOT_FUNC_F_STEP_RUNTIME | JCA_IOT_FUNC_F_SOURCE_PARAM | 0;
	}else{
		if (this->Params[0].getInt() == JCA_IOT_FUNC_CLOCKSWITCH_T_ONOFF){
			this->Params.push_back(new cParam("TimeOn", JCA_IOT_FUNC_PARAM_T_INT));
			this->Params.push_back(new cParam("TimeOff", JCA_IOT_FUNC_PARAM_T_INT));
		}
		if (this->Params[0].getInt() == JCA_IOT_FUNC_CLOCKSWITCH_T_PULSE){
			this->Params.push_back(new cParam("TimeStart", JCA_IOT_FUNC_PARAM_T_INT));
			this->Params.push_back(new cParam("Duration", JCA_IOT_FUNC_PARAM_T_INT));
		}
		//Parameter initialisieren
		if (!this->Params[1].setValue(iTime1)){
			this->Error = JCA_IOT_FUNC_F_STEP_RUNTIME | JCA_IOT_FUNC_F_SOURCE_PARAM | 1;
		}
		if (!this->Params[2].setValue(iTime2)){
			this->Error = JCA_IOT_FUNC_F_STEP_RUNTIME | JCA_IOT_FUNC_F_SOURCE_PARAM | 2;
		}
	}

	//Interface anlegen
}
cClockSwitch::cClockSwitch(const char* strName, int iPort, float fMin, float fMax, float fValue) : cRoot(strName, JCA_IOT_IO_T_AI){
	this->Port = iPort;
	this->Min = fMin;
	this->Max = fMax;
	this->Value = fValue;
	pinMode(this->Port, INPUT);
	this->QC = JCA_IOT_QC_GOOD;
	this->update();
}

/**********************************************
 * Methode: update()
 * Info:	Aktuallisiert den IO-Wert
 *				der aktuelle Wert wird auf den
 *				Ausgang geschrieben.
 * Param:
 * Version:
 *	V1.0.0	Erstellt	22.05.2018	JCA
 **********************************************/
void cClockSwitch::update(){
	int iIn = analogRead(this->Port);
	this->Value = (float)iIn / 1023.0 * (this->Max - this->Min) + this->Min;
	this->QC = JCA_IOT_QC_GOOD;
}

/**********************************************
 * Methode: Get/Set...()
 * Info:	Wert schreiben oder auslesen
 * Param:	void / Wert
 * Return:	Wert / void
 * Version:
 *	V1.0.0	Erstellt	22.05.2018	JCA
 **********************************************/
void cClockSwitch::setString(char* strValue){
	this->Value = atof(strValue);
	this->QC = JCA_IOT_QC_GOOD;
}
char* cClockSwitch::getString(char* strValue){
	String(this->Value).toCharArray(strValue, sizeof(strValue));
	return strValue;
	}
void  cClockSwitch::setFloat(float Value){this->Value = Value;this->QC = JCA_IOT_QC_GOOD;}
float cClockSwitch::getFloat(){return (float)this->Value;}
void  cClockSwitch::setTime(int32_t Value){this->Value = (float)Value;this->QC = JCA_IOT_QC_GOOD;}
int32_t  cClockSwitch::getTime(){return (int32_t)this->Value;}
void  cClockSwitch::setLong(int32_t Value){this->Value = (float)Value;this->QC = JCA_IOT_QC_GOOD;}
int32_t  cClockSwitch::getLong(){return (int32_t)this->Value;}
void  cClockSwitch::setBool(bool Value){this->Value = (float)Value;this->QC = JCA_IOT_QC_GOOD;}
bool  cClockSwitch::getBool(){return this->Value;}
void  cClockSwitch::setDate(int32_t Value){this->Value = (float)Value;this->QC = JCA_IOT_QC_GOOD;}
int32_t  cClockSwitch::getDate(){return (int32_t)this->Value;}
