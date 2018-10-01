/**********************************************
 * Class:	JMZ_IOT_CODE
 * Info: 	Die Klasse beinhaltet alle Funktionen
 *				zur interpretation des IOT
 *				Programmcodes.
 * Codeaufbau:
			$IO
			Type;Par1;Par2;...
			Type;...
			$IO_END
			$MESH
			RemStation;RemTag;LokalTag;Zyklus;R/S(Recv/Send)
			RemStation;...
			$MESH_END
			$CODE
			Befehl" "Wert
						{IO}
						Konstante/Ziel
						#x#Spezialkonstante
			[Ziel]
				Sprungadresse
			$CODE_END

 * Version:
 * 	V1.0.0	Erstellt	20.05.2018	JCA
 *		-analyseIOs
 *		-analyseCode
 *		-doCode
 **********************************************/

#ifndef _JCA_IOT_CODE_H
#define _JCA_IOT_CODE_H

#include "JCA_IOT_QC.h"
#include "JCA_IOT_CODELINE.h"
#include "JCA_IOT_JUMP.h"
//#include <JCA_IOT_IO_All.h>
#include "JCA_IOT_IO.h"
#include "JCA_IOT_IO_DO.h"
#include "JCA_IOT_IO_DI.h"
#include "JCA_IOT_IO_AO.h"
#include "JCA_IOT_IO_AI.h"
#include "JCA_IOT_IO_Bool.h"
#include "JCA_IOT_IO_Long.h"
#include "JCA_IOT_IO_Float.h"

#define JCA_IOT_CODE_IO_NOTFOUND	-1
#define JCA_IOT_CODE_IO_OK			0
#define JCA_IOT_CODE_IO_NONAME		1
#define JCA_IOT_CODE_IO_MINPAR		2
#define JCA_IOT_CODE_IO_UNKNOWN		3

#define JCA_IOT_CODE_CMD_OK			0
#define JCA_IOT_CODE_CMD_NOCMD		1
#define JCA_IOT_CODE_CMD_MINPAR		2
#define JCA_IOT_CODE_CMD_UNKNOWN	3
#define JCA_IOT_CODE_CMD_VALUE		4


#define JCA_IOT_CODE_CMD_AK			001	//"A("		n++, function[n] = A, Init[n] = 1
#define JCA_IOT_CODE_CMD_OK			002	//"O("		n++, function[n] = O, Init[n] = 1
#define JCA_IOT_CODE_CMD_ANK		003	//"AN("		n++, function[n] = AN, Init[n] = 1
#define JCA_IOT_CODE_CMD_ONK		004	//"ON("		n++, function[n] = ON, Init[n] = 1
#define JCA_IOT_CODE_CMD_KE			051	//")"		VKE[n-1] = VKE[n-1] function[n] VKE[n], n--
#define JCA_IOT_CODE_CMD_N			052	//"N"		VKE[n] = NICHT VKE[n] (Init[n] : VKE[n] = 1, Init[n] = 0)

#define JCA_IOT_CODE_CMD_A			101 //"A"		VKE[n] = VKE[n] UND Variable (Init[n] : VKE[n] = 1, Init[n] = 0)
#define JCA_IOT_CODE_CMD_O			102 //"O"		VKE[n] = VKE[n] ODER Variable (Init[n] : VKE[n] = 0, Init[n] = 0)
#define JCA_IOT_CODE_CMD_AN			103 //"AN"		VKE[n] = VKE[n] UND NICHT Variable (Init[n] : VKE[n] = 1, Init[n] = 0)
#define JCA_IOT_CODE_CMD_ON			104 //"ON"		VKE[n] = VKE[n] ODER NICHT Variable (Init[n] : VKE[n] = 0, Init[n] = 0)
#define JCA_IOT_CODE_CMD_Z			151 //"="		Variable = VKE[n], Init[n] = 1
#define JCA_IOT_CODE_CMD_S			152 //"S"		WENN VKE[n] == 1 : Variable = 1
#define JCA_IOT_CODE_CMD_R			153 //"R"		WENN VKE[n] == 1 : Variable = 0

#define JCA_IOT_CODE_CMD_L_L		201	//"LL"		AkkuL = Variable
#define JCA_IOT_CODE_CMD_L_SUB		202	//"L-"		AkkuL = AkkuL - Variable
#define JCA_IOT_CODE_CMD_L_ADD		203	//"L+"		AkkuL = AkkuL + Variable
#define JCA_IOT_CODE_CMD_L_MUL		204	//"L*"		AkkuL = AkkuL * Variable
#define JCA_IOT_CODE_CMD_L_DIV		205	//"L/"		AkkuL = AkkuL / Variable
#define JCA_IOT_CODE_CMD_L_GT		206	//"L>"		VKE[n] = AkkuL >  Variable
#define JCA_IOT_CODE_CMD_L_LT		207	//"L<"		VKE[n] = AkkuL <  Variable
#define JCA_IOT_CODE_CMD_L_GE		208	//"L>="		VKE[n] = AkkuL >= Variable
#define JCA_IOT_CODE_CMD_L_LE		209	//"L<="		VKE[n] = AkkuL <= Variable
#define JCA_IOT_CODE_CMD_L_NE		210	//"L<>"		VKE[n] = AkkuL <> Variable
#define JCA_IOT_CODE_CMD_L_EQ		211	//"L=="		VKE[n] = AkkuL == Variable
#define JCA_IOT_CODE_CMD_L_T		251	//"LT"		Variable = AkkuL

#define JCA_IOT_CODE_CMD_F_L		301	//"FL"		AkkuR = Variable
#define JCA_IOT_CODE_CMD_F_SUB		302	//"F-"		AkkuR = AkkuR - Variable
#define JCA_IOT_CODE_CMD_F_ADD		303	//"F+"		AkkuR = AkkuR + Variable
#define JCA_IOT_CODE_CMD_F_MUL		304	//"F*"		AkkuR = AkkuR * Variable
#define JCA_IOT_CODE_CMD_F_DIV		305	//"F/"		AkkuR = AkkuR / Variable
#define JCA_IOT_CODE_CMD_F_GT		306	//"F>"		VKE[n] = AkkuR >  Variable
#define JCA_IOT_CODE_CMD_F_LT		307	//"F<"		VKE[n] = AkkuR <  Variable
#define JCA_IOT_CODE_CMD_F_GE		308	//"F>="		VKE[n] = AkkuR >= Variable
#define JCA_IOT_CODE_CMD_F_LE		309	//"F<="		VKE[n] = AkkuR <= Variable
#define JCA_IOT_CODE_CMD_F_NE		310	//"F<>"		VKE[n] = AkkuR <> Variable
#define JCA_IOT_CODE_CMD_F_EQ		311	//"F=="		VKE[n] = AkkuR == Variable
#define JCA_IOT_CODE_CMD_F_T		351	//"FT"		Variable = AkkuR

#define JCA_IOT_CODE_CMD_SPB		901
#define JCA_IOT_CODE_CMD_SPN		902
#define JCA_IOT_CODE_CMD_DEST		911

#define JCA_IOT_CODE_VECTOR_IO		1
#define JCA_IOT_CODE_VECTOR_BOOL	2
#define JCA_IOT_CODE_VECTOR_LONG	3
#define JCA_IOT_CODE_VECTOR_FLOAT	4
#define JCA_IOT_CODE_VECTOR_JMP		5



#define JCA_IOT_CODE_NAMELEN		30
#define JCA_IOT_CODE_LINELEN		80
#define JCA_IOT_CODE_TIEFE			32

//#include "JCA_IOT_MESH_Handler.h"


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
