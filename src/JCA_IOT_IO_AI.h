/**********************************************
 * Class:	JMZ_IOT_IO_AI
 * Info: 	Die Klasse erstellt einen digitalen
 *				Ausgang für einen OnBoard Pin.
 * Version:
 * 	V1.0.0	Erstellt	22.05.2018	JCA
 *		-get/set Float/Time/int32_t/Bool/Date
 **********************************************/

#include "JCA_IOT_IO.h"

#ifndef _JCA_IOT_IO_AI_H
#define _JCA_IOT_IO_AI_H
namespace JCA{ namespace IOT{ namespace IO{
class cAI : public cRoot{
	public:
		cAI(const char* strName, int iPort, float fMin, float fMax, float fValue);
		cAI(const char* strName, int iPort, float fMin, float fMax);

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
		float Value;
		float Min;
		float Max;
		int Port;
};
}}}

#endif
