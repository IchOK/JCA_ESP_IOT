/**********************************************
 * Class:	JMZ_IOT_FUNC_CLOCKSWITCH
 * Info: 	Die Klasse eine Zeitschaltuhr mit
 *				zwei unterschiedlichen Modus.
 *				1. ON-OFF
 *					liegt die Zeit zwischen Time1 und Time 2 -> Output = 1
 *				2. PULSE
 *					nach Time1 ist Output = 1 für die Dauert Time2
 * Version:
 * 	V1.0.0	Erstellt	17.04.2019	JCA
 *		-update
 **********************************************/

#ifndef _JCA_IOT_FUNC_CLOCKSWITCH_H
#define _JCA_IOT_FUNC_CLOCKSWITCH_H

#define JCA_IOT_FUNC_CLOCKSWITCH_T_ONOFF	1
#define JCA_IOT_FUNC_CLOCKSWITCH_T_PULSE	2

#include "JCA_IOT_Func.h"

namespace JCA{ namespace IOT{ namespace FUNC{
class cClockSwitch : public cRoot{
	public:
		cClockSwitch(const char* strName, std::vector<JCA::IOT::IO::cRoot*>& vIOs, int iConfig, uint32_t iTime1, uint32_t iTime2, int iIndexOutput);

		virtual void  update(uint32_t tTime);

	protected:
};
}}}

#endif
