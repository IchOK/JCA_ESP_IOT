/**********************************************
 * Class:	JMZ_IOT_JUMP
 * Info: 	Die Klasse stellt eine Sprung-
 *				verfnüpfung dar.
 * Version:
 * 	V1.0.0	Erstellt	24.05.2018	JCA
 *		-get/set
 **********************************************/

#ifndef _JCA_IOT_JUMP_H
#define _JCA_IOT_JUMP_H

#include <string.h>
namespace JCA{ namespace IOT{
class cJump {
	public:
		cJump(int iPos, char* strName);

		int		pos;
		char	Name[30];
};
}}

#endif
