/**********************************************
 * Class:	JMZ_IOT_IO
 * Info: 	Die Klasse beinhaltet alle Grundfunktionen
 *				der IOT-IO's und dient als Ursprung
 *				aller weiternen Klassen
 * Version:
 * 	V1.0.0	Erstellt	19.05.2018	JCA
 *		-init
 *		-update dummy
 *		-getType
 *		-GetName
 *		-get/set Float/Time/int32_t/Bool/Date
 **********************************************/

#ifndef _JCA_IOT_IO_H
#define _JCA_IOT_IO_H

#include "JCA_IOT_QC.h"
//Type-Kennungen der IOs
#define JCA_IOT_IO_T_ROOT		0
#define JCA_IOT_IO_T_LONG		1
#define JCA_IOT_IO_T_FLOAT		2
#define JCA_IOT_IO_T_BOOL		3
#define JCA_IOT_IO_T_DO			10
#define JCA_IOT_IO_T_DI			11
#define JCA_IOT_IO_T_AO			12
#define JCA_IOT_IO_T_AI			13
//Minimale Vector-Größe des IOs
#define JCA_IOT_IO_L_ROOT		2
#define JCA_IOT_IO_L_LONG		2
#define JCA_IOT_IO_L_FLOAT		2
#define JCA_IOT_IO_L_BOOL		2
#define JCA_IOT_IO_L_DO			3
#define JCA_IOT_IO_L_DI			4
#define JCA_IOT_IO_L_AO			5
#define JCA_IOT_IO_L_AI			5


#include <Arduino.h>
#include <string.h>
namespace JCA{ namespace IOT{ namespace IO{
class cRoot {
	public:
		cRoot(const char* strName, int iType);
		int getType();
		char* getName();
		unsigned char  getQC();
		void setQC(unsigned char uQC);
		bool Good();

		virtual void  update();
		virtual void  setString(char* strValue);
		virtual char* getString(char* strValue);
		virtual void  setFloat(float Value);
		virtual float getFloat();
		virtual void  setTime(int32_t Value);
		virtual int32_t  getTime();
		virtual void  setLong(int32_t Value);
		virtual int32_t  getLong();
		virtual void  setBool(bool Value);
		virtual bool  getBool();
		virtual void  setDate(int32_t Value);
		virtual int32_t  getDate();

	protected:
		char Name[30];
		int Type;
		unsigned char QC;
};
}}}

#endif
