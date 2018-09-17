/**********************************************
 * Class:	JMZ_IOT_MESH_TAG
 * Info: 	Die Klasse beinhaltet alle
 *				Daten eines Mesh-Tags
 * Version:
 * 	V1.0.0	Erstellt	14.09.2018	JCA
 **********************************************/
 
#ifndef _JCA_IOT_MESH_TAG_H
#define _JCA_IOT_MESH_TAG_H

#include "Arduino.h"
#include <string.h>
namespace JCA{ namespace IOT{ namespace MESH{
class cTag {
	public:
		cTag(char* strTag, int iSetTime, int iIndex, bool bRecv);
		
		char	Name[30];
		int		ioIndex;
		uint32_t	SetTime;	//(Konfigurierte Zeit)
		uint32_t	Count;	//(Micros-Zähler, Read=WD / Write=Zyklus)
		bool	Recv;			//(1 = Mesh -> IO, 0 = IO -> Mesh)
		bool	Found;
		bool	Linked;
};
}}}

#endif