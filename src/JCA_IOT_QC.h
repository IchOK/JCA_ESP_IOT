/**********************************************
 * Define:	JMZ_IOT_QC
 * Info: 	Diese Definition listet alle
 *				verwendbaren Quality Codes auf
 *				die Definition ist an Siemens
 *				angelehnt.
 * Version:
 * 	V1.0.0	Erstellt	19.05.2018	JCA
 *		-Define
 **********************************************/

#ifndef _JCA_IOT_QC_H
#define _JCA_IOT_QC_H

/* Code Aufbau 						  QQSSSSLL
									  00xxxxxx	Bad			Schlecht
									  01xxxxxx	Uncertain	Ungewiss
									  10xxxxxx	Good		Gut
									  11xxxxxx	Good		Gut Verkettet
									  xx0000xx	NonSpec		Grund unbekannt
									  xx0001xx	Config		Falsche konfiguration
									  xx0010xx	NotConn		Es besteht keine logische Verbindung
									  xx0011xx	Device		Das Gerät hat einen Fehler
									  xx0100xx	SensDef		Der Sensor hat einen Fehler
									  xx0101xx	LastValue	Verbindungsfehler, letzter Wert verfügbar
									  xx0110xx	CommFault	Verbindungsfehler, kein letzter Wert verfügbar
									  xx0111xx	OutOfServ	Wartung aktiv
									  xx1000xx	SensRange	Sensor auserhalt der Betriebbereichs
									  xx1001xx	ValueRange	Wert ist auserhalb des definierten Bereichs
									  xx1010xx	Override	Der Wert wird lokal überschrieben
									  xx1111xx	InitValue	Startwert
									  xxxxxx00	NotLimit	Wert ist im gültigen Bereich
									  xxxxxx01	LowLimit	Wert ist am unteren Bereich begrenzt
									  xxxxxx10	HighLimit	Wert ist am oberen Bereich begrenzt
									  xxxxxx11	Constant	Wert ist eingefrohren
*/
#define JCA_IOT_QC_GOOD				0b10000000
#define JCA_IOT_QC_INIT				0b01111100
#define JCA_IOT_QC_COMMFAULT		0b00011000
#define JCA_IOT_QC_COMMLAST			0b01010100
#define JCA_IOT_QC_CONFNAME     0b00000100
#define JCA_IOT_QC_CONFTYPE     0b00000101

//Code MAKRO
#define JCA_IOT_QC_IO(wert) ((wert && 0b11000000) <> 0b00000000)

#endif
