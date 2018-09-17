/**********************************************
 * Class:	JMZ_IOT_IO_DO
 * Info: 	Die Klasse erstellt einen digitalen
 *				Ausgang für einen OnBoard Pin.
 * Version:
 * 	V1.0.0	Erstellt	19.05.2018	JCA
 *		-get/set Float/Time/int32_t/Bool/Date
 **********************************************/
 
#include <JCA_IOT_IO.h>

#ifndef _JCA_IOT_IO_DO_H
#define _JCA_IOT_IO_DO_H
namespace JCA{ namespace IOT{ namespace IO{
class cDO : public cRoot{
	public:
		cDO(const char* strName, int iPort, bool bValue);
		cDO(const char* strName, int iPort);

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
		bool Value;
		int Port;
};
}}}

#endif