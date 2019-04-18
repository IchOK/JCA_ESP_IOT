/**********************************************
 * Class:	JMZ_IOT_SYSTEM
 * Info: 	Die Klasse managt das Verhalten des IOT.
 * Version:
 * 	V1.0.0	Erstellt	18.04.2019	JCA
 *		-analyseIOs
 *		-analyseCode
 *		-doCode
 **********************************************/

#ifndef _JCA_IOT_SYSTEM_H
#define _JCA_IOT_SYSTEM_H

//#include "JCA_IOT_QC.h"
//#include "JCA_IOT_IO.h"
//#include "JCA_IOT_IO_DO.h"
//#include "JCA_IOT_IO_DI.h"
//#include "JCA_IOT_IO_AO.h"
//#include "JCA_IOT_IO_AI.h"
//#include "JCA_IOT_IO_Bool.h"
//#include "JCA_IOT_IO_Long.h"
//#include "JCA_IOT_IO_Float.h"


#include "JCA_IOT_FUNC.h"
#include "JCA_IOT_FUNC_Param.h"
#include "JCA_IOT_FUNC_ClockSwitch.h"

#include <FS.h>
#include <string.h>
#include <vector>
namespace JCA{ namespace IOT{
class cCode {
	public:
		cCode();
		void setStart();
		bool pharseFile(File* file, void (*xAddToList)(char*, char*, int, int, char));
		char* analyseIOs(char* strLine);
		char* analyseCode(char* strLine);
		char* analyseMesh(char* strLine, void (*xAddToList)(char*, char*, int, int, char));
		int doIOs();
		int doCode(uint32_t ulMicros);
		int insertIO(char* strLine);
		bool deleteIO(char* strName);
		IO::cRoot* getIO(char* strName);
		IO::cRoot* getIO(int iIndex);
		int getCodeSize();
		int getIoSize();
		int getFaultIO();
		int getFaultCode();
		int findIO(char* strName);


	protected:
		void addIO(std::vector<char*> vPar, int iType);
		void updateIO(std::vector<char*> vPar, int iType, int iIndex);
		bool getTypeIO(char* strType, int* iType, int* iMinLen);
		bool deleteIO(int iIndex);

		int insertCode(char* strLine);
		bool getFunctionCode(char* strFunc, int* iFunc, int* iMinLen);
		bool getValueCode(char* strValue, int iFunc, int* iType, int* iPos);

		std::vector<IO::cRoot*> IOs;	//Vector alles im IO-Bereich definierten IOs

		std::vector<bool> boolConst;			//Vector der Konstanten aus dem Code-Bereich Bool
		std::vector<int32_t> longConst;			//Vector der Konstanten aus dem Code-Bereich int32_t
		std::vector<float> floatConst;			//Vector der Konstanten aus dem Code-Bereich Float
		std::vector<cCodeLine*> Code;			//Vector aller Codezeilen mit deren Funktion und Werten
		//Jmp-Vectoren werden während der Codeanalyse gefüllt um danach die entsprechende Codezeile zu verknüpfen
		std::vector<cJump*> JmpLine; 			//temporärer Vector als Zwischenspeicher für Sprungzeilen
		std::vector<cJump*> JmpDest; 			//temporärer Vector der alle Sprungziele enthält

		//Datenpunkt zur Initialisierung des Codeinterpreter
		bool areaIO;
		bool areaCode;
		bool areaMesh;
		int  faultIO;
		int  faultCode;
		int  faultMesh;

		uint32_t microsCodeOld;
		bool microsCodeInit;

		//Datenpunkte zur Code verarbeitung
		bool VKE[32];		//Logisches Verknüpfungsergebniss, durch Klammern maximale 32 Ebenen erreicht werden
		bool Init[32];		//Initalisierung der Verknüpfungsebene (1:Verknpüfung wird neu gebildet)
		int function[32];	//Funktion mit der das Verknüpfunksergebniss der Ebene mit der darunterliegenden verknüpft wird
		int ebene;			//aktuelle Verknüpfungsebene
		float akkuF;		//Zwischenspeicher für Float-Operationen
		int32_t akkuL;			//Zwischenspeicher für int32_t-Operationen
};
}}

#endif
