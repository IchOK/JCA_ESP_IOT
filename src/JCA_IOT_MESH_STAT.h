/**********************************************
 * Class:	JMZ_IOT_MESH_STAT
 * Info: 	Die Klasse beinhaltet alle
 *				Daten einer Mesh-Station
 *				und eine Auflistung der Tags
 * Version:
 * 	V1.0.0	Erstellt	14.09.2018	JCA
 **********************************************/

#ifndef _JCA_IOT_MESH_STAT_H
#define _JCA_IOT_MESH_STAT_H

#include "JCA_IOT_MESH_TAG.h"
#include <Arduino.h>
#include <string.h>
namespace JCA{ namespace IOT{ namespace MESH{
class cStation {
	public:
		cStation(char* strStation);

		char	name[30];
		uint32_t		id;
		uint32_t	watchDog;
		std::vector<cTag*> tags;
};
}}}

#endif
