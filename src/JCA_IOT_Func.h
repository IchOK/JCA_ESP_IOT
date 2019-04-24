/**********************************************
 * Class:	JMZ_IOT_Func
 * Info: 	Die Klasse beinhaltet alle Grundfunktionen
 *				der IOT-Funktionen und dient als Ursprung
 *				aller weiternen Klassen
 * Error: Der Datenpunkt gibt an ob bei der Configuration
 *        oder der Laufzeit ein Fehler aufgetreten ist.
 *        XYZZ
 *        ||+--Index an dem der Fehler aufgetreten ist
 *        |+---1=Parameter, 2=Interface, 3=Code
 *        +----1=Konfiguration, 2=Laufzeit
 * Version:
 * 	V1.0.0	Erstellt	17.04.2019	JCA
 *		-init
 *		-update dummy
 *		-getName
 *		-getState
 *		-get/set Float/Time/int32_t/Bool/Date
 **********************************************/

#ifndef _JCA_IOT_FUNC_H
#define _JCA_IOT_FUNC_H

#define JCA_IOT_FUNC_T_CLOCKSWITCH	1
#define JCA_IOT_FUNC_T_COMPARE	2
#define JCA_IOT_FUNC_T_PID	3
#define JCA_IOT_FUNC_T_AND	4
#define JCA_IOT_FUNC_T_OR	5
#define JCA_IOT_FUNC_T_NOT	6

#define JCA_IOT_FUNC_F_STEP_CONFIG 0x1000
#define JCA_IOT_FUNC_F_STEP_RUNTIME 0x2000
#define JCA_IOT_FUNC_F_SOURCE_PARAM 0x0100
#define JCA_IOT_FUNC_F_SOURCE_INTERFACE 0x0200
#define JCA_IOT_FUNC_F_SOURCE_CODE 0x0300

#include "JCA_IOT_IO.h"
#include "JCA_IOT_PARAM.h"

#include <Arduino.h>
#include <ArduinoJson.h> //v6
#include <string.h>
namespace JCA{ namespace IOT{ namespace FUNC{
class cRoot {
	public:
		cRoot(const char* strName, int iType, std::vector<JCA::IOT::IO::cRoot*>& vIOs);
		bool getConfig(JsonObject* ptrConfig);
    bool getState(JsonObject* ptrState);

		virtual void  update(uint32_t tTime);

		virtual bool  setParam(char* strName, char* strValue);

	protected:
    uint32_t Error;
		char Name[30];
		int Type;
    <JCA::IOT::IO::cRoot*>& IOs;
    std::vector<JCA::IOT::FUNC::cParam*> Params;
    std::vector<JCA::IOT::FUNC::cParam*> Interface;
};
}}}

#endif
