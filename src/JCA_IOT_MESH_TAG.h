/**********************************************
 * Class:	JMZ_IOT_MESH_TAG
 * Info: 	Die Klasse beinhaltet alle
 *				Daten eines Mesh-Tags
 * Version:
 * 	V1.0.0	Erstellt	14.09.2018	JCA
 **********************************************/

#ifndef _JCA_IOT_MESH_TAG_H
#define _JCA_IOT_MESH_TAG_H

#include <Arduino.h>
#include <string.h>
namespace JCA{ namespace IOT{ namespace MESH{
class cTag {
	public:
		cTag(char* strTag, int iSetTime, int iIndex, bool bRecv);

		char	name[30];
		int		ioIndex;
		uint32_t	setTime;	//(Konfigurierte Zeit)
		uint32_t	count;	//(Micros-Zähler, Read=WD / Write=Zyklus)
		bool	recv;			//(1 = Mesh -> IO, 0 = IO -> Mesh)
		bool	found;
		bool	linked;
};
}}}

#endif
