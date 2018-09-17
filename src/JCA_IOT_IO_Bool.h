/**********************************************
 * Class:	JMZ_IOT_IO_Bool
 * Info: 	Die Klasse stellt einen statischen
 *				Real-Wert zur verfügung der als
 *				IO als Speicher oder Parameter
 *				verwendet werden kann.
 * Version:
 * 	V1.0.0	Erstellt	22.05.2018	JCA
 *		-get/set Float/Time/int32_t/Bool/Date
 **********************************************/

#include "JCA_IOT_IO.h"

#ifndef _JCA_IOT_IO_BOOL_H
#define _JCA_IOT_IO_BOOL_H
namespace JCA{ namespace IOT{ namespace IO{
class cBool : public cRoot{
	public:
		cBool(const char* strName, bool bValue);
		cBool(const char* strName);

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
		bool Value;
};
}}}

#endif
