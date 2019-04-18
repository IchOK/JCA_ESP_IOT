/**********************************************
 * Class:	JCA_IOT_FUNC_Param
 * Info: 	Die Klasse bildet einen Parameter
 * 					einer Funktion ab.
 * Version:
 * 	V1.0.0	Erstellt	17.04.2019	JCA
 *		-getName
 *		-getType
 *		-getFloat
 *		-getInt
 *		-getBool
 *		-setValue (string)
 **********************************************/

#ifndef _JCA_IOT_FUNC_PARAM_H
#define _JCA_IOT_FUNC_PARAM_H

#define JCA_IOT_FUNC_PARAM_T_NONE	0
#define JCA_IOT_FUNC_PARAM_T_FLOAT	1
#define JCA_IOT_FUNC_PARAM_T_INT	2
#define JCA_IOT_FUNC_PARAM_T_BOOL	3


#include <string.h>
namespace JCA{ namespace IOT{ namespace FUNC{
class cParam {
	public:
		cParam(const char* strName, int iType);
		char* getName();
		int getType();
		float getFloat();
		int32_t getInt();
		bool getBool();
		bool setValue(char* strValue);

	protected:
		char Name[30];
		int Type;
		float fValue;
		int32_t iValue;
};
}}}

#endif
