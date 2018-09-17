/**********************************************
 * Class:	JMZ_IOT_CODELINE
 * Info: 	Die Klasse beinhaltet alle
 *				aufbereiteten Daten einer
 *				Codezeile
 * Version:
 * 	V1.0.0	Erstellt	22.05.2018	JCA
 *		-get/set
 **********************************************/
 
#ifndef _JCA_IOT_CODELINE_H
#define _JCA_IOT_CODELINE_H

namespace JCA{ namespace IOT{
class cCodeLine {
	public:
		cCodeLine(int iFunction, int iType, int iPos);
		
		int function;		//Übersetzung des Programmcode-String in Online-Code (Sprungadresse)
		int type;			//Type des Werts: 1=IO, 2=boolConst, 3=longConst, 4=floatConst
		int pos;			//Position im entsprechenden Vector (IO, Const, Code bei Jump)
};
}}

#endif