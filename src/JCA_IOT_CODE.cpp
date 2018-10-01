/**********************************************
 * Beschreibung: siehe Header File
 **********************************************/

#include <JCA_IOT_CODE.h>
//#define _DEBUG_ON

using namespace JCA::IOT;
/**********************************************
 * Methode: Constructor()
 * Info:	Initialisierung der statischen Werte
 * Param:
 * Version:
 *	V1.0.0	Erstellt	20.05.2018	JCA
 **********************************************/
cCode::cCode(){
	#ifdef _DEBUG_ON
		Serial.println("cCode::cCode");
	#endif
	this->setStart();
}

/**********************************************
 * Methode: setStart()
 * Info:	stellt die internen Datenpunkt so
 *				ein dass die Codeauswertung
 *				neu begonnen wird
 * Param:
 * Return:
 * Version:
 *	V1.0.0	Erstellt	20.05.2018	JCA
 **********************************************/
void cCode::setStart(){
	#ifdef _DEBUG_ON
		Serial.println("cCode::setStart");
	#endif
	this->faultIO = 0;
	this->faultCode = 0;
	this->areaIO = false;
	this->areaCode = false;
	this->microsCodeInit = true;
}

/**********************************************
 * Methode: pharsFile(FIle* file)
 * Info:	Code-File einlesen und daraus IOs
 *				und Code erzeugen
 * Param:	strLine : Codezeile
 * Return:	Fehler als char* (NULL = IO)
 * Version:
 *	V1.0.0	Erstellt	21.05.2018	JCA
 **********************************************/
bool cCode::pharseFile(File* file, void (*xAddToList)(char*, char*, int, int, char)){
	#ifdef _DEBUG_ON
		Serial.println("cCode::pharseFile");
	#endif
	if(file){
		#ifdef _DEBUG_ON
			Serial.println("cCode::pharseFile - File Found");
		#endif
		this->setStart();
		while(file->available()){
			char strLine[JCA_IOT_CODE_LINELEN] = "";
			size_t sLine = file->readBytesUntil('\n', strLine, JCA_IOT_CODE_LINELEN);
			#ifdef _DEBUG_ON
				Serial.print("cCode::pharseFile - Line: ");
				Serial.println(strLine);
			#endif
			this->analyseIOs(strLine);
			this->analyseCode(strLine);
			this->analyseMesh(strLine, xAddToList);
		}
	}
	#ifdef _DEBUG_ON
		else{
			Serial.println("File Not Found");
		}
	#endif
}

/**********************************************
 * Methode: analyseIOs()
 * Info:	Wertet die einzelnen Codezeilen aus,
 *				sucht darin nach dem Begin des
 *				IO-Bereichs und erzeugt dann
 *				die einzelnen IO-Objekte
 * Param:	strLine : Codezeile
 * Return:	Fehler als char* (NULL = IO)
 * Version:
 *	V1.0.0	Erstellt	20.05.2018	JCA
 **********************************************/
char* cCode::analyseIOs(char* strLine){
	#ifdef _DEBUG_ON
		Serial.println("cCode::analyseIOs");
	#endif
	bool analyse = false;
	int lineIO = JCA_IOT_CODE_IO_OK;
	char* komment;

	komment = strstr(strLine, "//");
	if(komment != NULL){
		*komment = 0;
		komment++;
		komment++;
		#ifdef _DEBUG_ON
			Serial.print("cCode::analyseIOs - Komment: ");
			Serial.println(komment);
		#endif
	}
	if(strlen(strLine) > 1){
		//Code befindet sich im IO-Bereich -> auf Ende prüfen
		if(this->areaIO){
			if(strstr(strLine,"$IO_END")){
				#ifdef _DEBUG_ON
					Serial.println("cCode::analyseIOs - IO-Ende");
				#endif
				this->areaIO = false;
			}else{
				#ifdef _DEBUG_ON
					Serial.println("cCode::analyseIOs - IO-Zeile");
				#endif
				lineIO = this->insertIO(strLine);
			}
		}else{
			if(strstr(strLine,"$IO")){
				#ifdef _DEBUG_ON
					Serial.println("cCode::analyseIOs - IO-Start");
				#endif
				this->areaIO = true;
			}
		}

	}
	switch(lineIO){
		case JCA_IOT_CODE_IO_UNKNOWN :
			return "IO unbekannt";
			break;
		case JCA_IOT_CODE_IO_NONAME	:
			return "IO Name fehlt";
			break;
		case JCA_IOT_CODE_IO_MINPAR :
			return "IO Parameter nicht ausreichend";
			break;
		case JCA_IOT_CODE_IO_OK :
			return NULL;
			break;
		default :
			return "IO Unbekannter Fehler";
			break;
	}
}

/**********************************************
 * Methode: analyseCode()
 * Info:	Wertet den Code aus und erzeugt
 *				daraus einen vector
 * Param:	strLine : Codezeile
 * Return:	Fehler als char* (NULL = IO)
 * Version:
 *	V1.0.0	Erstellt	22.02.2018	JCA
 **********************************************/
char* cCode::analyseCode(char* strLine){
	#ifdef _DEBUG_ON
		Serial.println("cCode::analyseCode");
	#endif
	bool analyse = false;
	int lineIO = JCA_IOT_CODE_CMD_OK;
	char* komment;

	komment = strstr(strLine, "//");
	if(komment != NULL){
		*komment = 0;
		komment++;
		komment++;
		#ifdef _DEBUG_ON
			Serial.print("cCode::analyseCode - Komment: ");
			Serial.println(komment);
		#endif
	}
	if(strlen(strLine) > 1){
		//Code befindet sich im IO-Bereich -> auf Ende prüfen
		if(this->areaCode){
			if(strstr(strLine,"$CODE_END")){
				#ifdef _DEBUG_ON
					Serial.println("cCode::analyseCode - Code-Ende");
				#endif
				this->areaCode = false;
			}else{
				#ifdef _DEBUG_ON
					Serial.println("cCode::analyseCode - Code-Zeile");
				#endif
				lineIO = this->insertCode(strLine);
			}
		}else{
			if(strstr(strLine,"$CODE")){
				#ifdef _DEBUG_ON
					Serial.println("cCode::analyseCode - Code-Start");
				#endif
				this->areaCode = true;
			}
		}

	}
	switch(lineIO){
		case JCA_IOT_CODE_CMD_UNKNOWN :
			return "Code-Befehl unbekannt";
			break;
		case JCA_IOT_CODE_CMD_MINPAR :
			return "Code-Befehl zu wenig Parameter";
			break;
		case JCA_IOT_CODE_CMD_NOCMD	:
			return "Code-Befehl kein Befehl vorhanden";
			break;
		case JCA_IOT_CODE_CMD_VALUE	:
			return "Code-Befehl Wert ungültig";
			break;
/*		case JCA_IOT_CODE_CMD_OK :
			return NULL;
			break;
*/		default :
			return "Code-Befehl unbekannter Fehler";
			break;
	}
}

/**********************************************
 * Methode: analyseMesh()
 * Info:	Wertet die einzelnen Codezeilen aus,
 *				sucht darin nach dem Begin des
 *				Mesh-Bereichs und erzeugt dann
 *				die einzelnen Mesh-Objekte
 * Param:	strLine : Codezeile
 * Return:	Fehler als char* (NULL = IO)
 * Version:
 *	V1.0.0	Erstellt	17.09.2018	JCA
 **********************************************/
char* cCode::analyseMesh(char* strLine, void (*xAddToList)(char*, char*, int, int, char)){
	#ifdef _DEBUG_ON
		Serial.println("cCode::analyseMesh");
	#endif
	bool analyse = false;
	int lineIO = JCA_IOT_CODE_IO_OK;
	char* komment;
	std::vector<char*> Par;
	char* tokken;
	int iIndex;

	komment = strstr(strLine, "//");
	if(komment != NULL){
		*komment = 0;
		komment++;
		komment++;
		#ifdef _DEBUG_ON
			Serial.print("cCode::analyseMesh - Komment: ");
			Serial.println(komment);
		#endif
	}
	if(strlen(strLine) > 1){
		//Code befindet sich im IO-Bereich -> auf Ende prüfen
		if(this->areaMesh){
			if(strstr(strLine,"$MESH_END")){
				#ifdef _DEBUG_ON
					Serial.println("cCode::analyseMesh - Mesh-Ende");
				#endif
				this->areaMesh = false;
			}else{
				#ifdef _DEBUG_ON
					Serial.println("cCode::analyseMesh - Mesh-Zeile");
				#endif
				//Char* aufsplitten
				tokken = strtok(strLine, ";");
				while(tokken != NULL){
					Par.push_back(tokken);
					tokken = strtok(NULL, ";");
				}
				if(Par.size() >= 5){
					iIndex = this->findIO(Par[2]);
					if(iIndex >= 0){
						xAddToList(Par[0], Par[1], atoi(Par[3]), iIndex, *Par[4]);
					}else{
						lineIO = JCA_IOT_CODE_IO_NOTFOUND;
					}
				}else{
					lineIO = JCA_IOT_CODE_IO_MINPAR;
				}


				lineIO = this->insertIO(strLine);
			}
		}else{
			if(strstr(strLine,"$MESH")){
				#ifdef _DEBUG_ON
					Serial.println("cCode::analyseMesh - Mesh-Start");
				#endif
				this->areaMesh = true;
			}
		}

	}
	switch(lineIO){
		case JCA_IOT_CODE_IO_NOTFOUND :
			return "IO zum Mesh nicht gefunden";
			break;
		case JCA_IOT_CODE_IO_MINPAR :
			return "Mesh Parameter nicht ausreichend";
			break;
		case JCA_IOT_CODE_IO_OK :
			return NULL;
			break;
		default :
			return "Mesh Unbekannter Fehler";
			break;
	}
}

/**********************************************
 * Methode: doIOs()
 * Info:	bearbeitet den IO-Vector
 * Param:
 * Return:
 *		int  = erste fehlerhafte Zeile
 * Version:
 *	V1.0.0	Erstellt	21.05.2018	JCA
 **********************************************/
int cCode::doIOs(){
	#ifdef _DEBUG_ON
		Serial.println("cCode::diIOs");
	#endif
	int i;
	this->faultIO = 0;
	for(i=0;i<this->IOs.size();i++){
		this->IOs[i]->update();
		if(this->faultIO == 0 && !this->IOs[i]->Good()){
			this->faultIO = i;
		}
	}
	return this->faultIO;
}

/**********************************************
 * Methode: doCode(ulMicros)
 * Info:	bearbeitet den Code-Vector
 * Param:	ulMicros : Microsekunden Umlaufzähler
 * Return:
 *		int  = erste fehlerhafte Zeile
 * Version:
 *	V1.0.0	Erstellt
 **********************************************/
int cCode::doCode(uint32_t ulMicros){
	uint32_t microsDiff;
	if(this->microsCodeInit){
		this->microsCodeOld = ulMicros;
		this->microsCodeInit = false;
	}
	microsDiff = ulMicros - this->microsCodeOld;
	this->microsCodeOld = ulMicros;

	#ifdef _DEBUG_ON
		Serial.println("cCode::doCode");
		Serial.print("Code-Size:");
		Serial.println(this->Code.size());
	#endif
	bool bValue;
	int32_t lValue;
	float fValue;
	int n=0;
	int i;
	int iFunc;

	this->Init[0] = true;

	for(i=0;i<this->Code.size();i++){
		iFunc = this->Code[i]->function;
		#ifdef _DEBUG_ON
			Serial.print("Code-Line:");
			Serial.print(i);
			Serial.print(" Function:");
			Serial.print(iFunc);
		#endif

		if(iFunc > 100 && iFunc < 150){	//Bool-Funktion lesend
			if(this->Code[i]->type == JCA_IOT_CODE_VECTOR_IO){
				#ifdef _DEBUG_ON
					Serial.print(" IO-Pos:");
					Serial.print(this->Code[i]->pos);
				#endif
				bValue = this->IOs[this->Code[i]->pos]->getBool();
			}else{
				#ifdef _DEBUG_ON
					Serial.print(" Const-Pos:");
					Serial.print(this->Code[i]->pos);
				#endif
				bValue = this->boolConst[this->Code[i]->pos];
			}
			#ifdef _DEBUG_ON
				Serial.print(" bValue:");
				Serial.print(bValue);
			#endif
		}
		if(iFunc > 200 && iFunc < 250){	//int32_t-Funktion lesend
			if(this->Code[i]->type == JCA_IOT_CODE_VECTOR_IO){
				lValue = this->IOs[this->Code[i]->pos]->getLong();
				#ifdef _DEBUG_ON
					Serial.print(" lValue[IO]:");
					Serial.print(lValue);
				#endif
			}else{
				lValue = this->longConst[this->Code[i]->pos];
				#ifdef _DEBUG_ON
					Serial.print(" lValue[Const]:");
					Serial.print(lValue);
				#endif
			}
		}
		if(iFunc > 300 && iFunc < 350){	//Float-Funktion lesend
			if(this->Code[i]->type == JCA_IOT_CODE_VECTOR_IO){
				fValue = this->IOs[this->Code[i]->pos]->getFloat();
				#ifdef _DEBUG_ON
					Serial.print(" fValue[IO]:");
					Serial.print(fValue);
				#endif
			}else{
				fValue = this->floatConst[this->Code[i]->pos];
				#ifdef _DEBUG_ON
					Serial.print(" fValue[Const]:");
					Serial.print(fValue);
				#endif
			}
		}
		if(iFunc > 0 && iFunc < 50){	//Klammer auf Funktion
			if(n > JCA_IOT_CODE_TIEFE - 2){
				#ifdef _DEBUG_ON
					Serial.print(" Fehler Klammern zu tief:");
					Serial.println(n);
				#endif
				return i;
			}
		}
		switch(iFunc){
			case 0 :
				;
				break;
			case JCA_IOT_CODE_CMD_AK	:
				n++;
				this->function[n] = JCA_IOT_CODE_CMD_A;
				this->Init[n] = true;
				break;
			case JCA_IOT_CODE_CMD_OK	:
				n++;
				this->function[n] = JCA_IOT_CODE_CMD_O;
				this->Init[n] = true;
				break;
			case JCA_IOT_CODE_CMD_ANK	:
				n++;
				this->function[n] = JCA_IOT_CODE_CMD_AN;
				this->Init[n] = true;
				break;
			case JCA_IOT_CODE_CMD_ONK	:
				n++;
				this->function[n] = JCA_IOT_CODE_CMD_ON;
				this->Init[n] = true;
				break;
			case JCA_IOT_CODE_CMD_KE	:
				if(n <= 0){
					#ifdef _DEBUG_ON
						Serial.print(" Fehler Klammern n<=0:");
						Serial.println(n);
					#endif
					return i;
				}
				switch(this->function[n]){
					case JCA_IOT_CODE_CMD_A		:
						this->VKE[n-1] = this->VKE[n-1] && this->VKE[n];
						break;
					case JCA_IOT_CODE_CMD_O		:
						this->VKE[n-1] = this->VKE[n-1] || this->VKE[n];
						break;
					case JCA_IOT_CODE_CMD_AN	:
						this->VKE[n-1] = this->VKE[n-1] && !this->VKE[n];
						break;
					case JCA_IOT_CODE_CMD_ON	:
						this->VKE[n-1] = this->VKE[n-1] || !this->VKE[n];
						break;
					default :
						return i;
						break;
				}
				n--;
				break;
			case JCA_IOT_CODE_CMD_N		:
				if(this->Init[n]){this->VKE[n]=true;this->Init[n]=false;}
				this->VKE[n] = !this->VKE[n];
				break;
			case JCA_IOT_CODE_CMD_A		:
				if(this->Init[n]){this->VKE[n]=true;this->Init[n]=false;}
				this->VKE[n] = this->VKE[n] && bValue;
				#ifdef _DEBUG_ON
					Serial.print(" VKE:");
					Serial.println(VKE[n]);
				#endif
				break;
			case JCA_IOT_CODE_CMD_O		:
				if(this->Init[n]){this->VKE[n]=false;this->Init[n]=false;}
				this->VKE[n] = this->VKE[n] || bValue;
				break;
			case JCA_IOT_CODE_CMD_AN	:
				if(this->Init[n]){this->VKE[n]=true;this->Init[n]=false;}
				this->VKE[n] = this->VKE[n] && !bValue;
				#ifdef _DEBUG_ON
					Serial.print(" VKE:");
					Serial.println(VKE[n]);
				#endif
				break;
			case JCA_IOT_CODE_CMD_ON	:
				if(this->Init[n]){this->VKE[n]=false;this->Init[n]=false;}
				this->VKE[n] = this->VKE[n] || !bValue;
				break;
			case JCA_IOT_CODE_CMD_Z		:
				if(this->Code[i]->type == JCA_IOT_CODE_VECTOR_IO){
					this->IOs[this->Code[i]->pos]->setBool(this->VKE[n]);
				}else{
					return i;
				}
				this->Init[n]=true;
				break;
			case JCA_IOT_CODE_CMD_S		:
				if(this->VKE[n]){
					#ifdef _DEBUG_ON
						Serial.print(" SET IO:");
					#endif
					if(this->Code[i]->type == JCA_IOT_CODE_VECTOR_IO){
						#ifdef _DEBUG_ON
							Serial.println(this->IOs[this->Code[i]->pos]->getName());
						#endif
						this->IOs[this->Code[i]->pos]->setBool(true);
					}else{
						#ifdef _DEBUG_ON
							Serial.println("NO IO");
						#endif
						return i;
					}
				}
				this->Init[n]=true;
				break;
			case JCA_IOT_CODE_CMD_R		:
				if(this->VKE[n]){
					#ifdef _DEBUG_ON
						Serial.print(" RESET IO:");
					#endif
					if(this->Code[i]->type == JCA_IOT_CODE_VECTOR_IO){
						#ifdef _DEBUG_ON
							Serial.println(this->IOs[this->Code[i]->pos]->getName());
						#endif
						this->IOs[this->Code[i]->pos]->setBool(false);
					}else{
						#ifdef _DEBUG_ON
							Serial.println("NO IO");
						#endif
						return i;
					}
				}
				this->Init[n]=true;
				break;
			case JCA_IOT_CODE_CMD_L_L	:
				this->akkuL = lValue;
				break;
			case JCA_IOT_CODE_CMD_L_SUB	:
				this->akkuL = this->akkuL - lValue;
				break;
			case JCA_IOT_CODE_CMD_L_ADD	:
				this->akkuL = this->akkuL + lValue;
				break;
			case JCA_IOT_CODE_CMD_L_MUL	:
				this->akkuL = this->akkuL * lValue;
				break;
			case JCA_IOT_CODE_CMD_L_DIV	:
				this->akkuL = this->akkuL / lValue;
				break;
			case JCA_IOT_CODE_CMD_L_GT	:
				this->VKE[n] = this->akkuL > lValue;
				break;
			case JCA_IOT_CODE_CMD_L_LT	:
				this->VKE[n] = this->akkuL < lValue;
				break;
			case JCA_IOT_CODE_CMD_L_GE	:
				this->VKE[n] = this->akkuL >= lValue;
				break;
			case JCA_IOT_CODE_CMD_L_LE	:
				this->VKE[n] = this->akkuL <= lValue;
				break;
			case JCA_IOT_CODE_CMD_L_NE	:
				this->VKE[n] = this->akkuL != lValue;
				break;
			case JCA_IOT_CODE_CMD_L_EQ	:
				this->VKE[n] = this->akkuL == lValue;
				break;
			case JCA_IOT_CODE_CMD_L_T	:
				if(this->Code[i]->type == JCA_IOT_CODE_VECTOR_IO){
					this->IOs[this->Code[i]->pos]->setLong(this->akkuL);
				}else{
					return i;
				}
				break;
			case JCA_IOT_CODE_CMD_F_L	:
				this->akkuF = fValue;
				break;
			case JCA_IOT_CODE_CMD_F_SUB	:
				this->akkuF = this->akkuF - fValue;
				break;
			case JCA_IOT_CODE_CMD_F_ADD	:
				this->akkuF = this->akkuF + fValue;
				break;
			case JCA_IOT_CODE_CMD_F_MUL	:
				this->akkuF = this->akkuF * fValue;
				break;
			case JCA_IOT_CODE_CMD_F_DIV	:
				this->akkuF = this->akkuF / fValue;
				break;
			case JCA_IOT_CODE_CMD_F_GT	:
				this->VKE[n] = this->akkuF > fValue;
				break;
			case JCA_IOT_CODE_CMD_F_LT	:
				this->VKE[n] = this->akkuF < fValue;
				break;
			case JCA_IOT_CODE_CMD_F_GE	:
				this->VKE[n] = this->akkuF >= fValue;
				break;
			case JCA_IOT_CODE_CMD_F_LE	:
				this->VKE[n] = this->akkuF <= fValue;
				break;
			case JCA_IOT_CODE_CMD_F_NE	:
				this->VKE[n] = this->akkuF != fValue;
				break;
			case JCA_IOT_CODE_CMD_F_EQ	:
				this->VKE[n] = this->akkuF == fValue;
				break;
			case JCA_IOT_CODE_CMD_F_T	:
				if(this->Code[i]->type == JCA_IOT_CODE_VECTOR_IO){
					this->IOs[this->Code[i]->pos]->setFloat(this->akkuF);
				}else{
					return i;
				}
				break;
			case JCA_IOT_CODE_CMD_SPB	:
				if(this->VKE[n]){
					this->Init[n] = true;
					i = this->Code[i]->pos;
				}
				break;
			case JCA_IOT_CODE_CMD_SPN	:
				if(!this->VKE[n]){
					this->Init[n] = true;
					i = this->Code[i]->pos;
				}
				break;
			case JCA_IOT_CODE_CMD_DEST	:
				;
				break;
			default :
				return i;
				break;
		}
	}
	return 0;
}

/**********************************************
 * Methode: insertIO(char* strLine)
 * Info:	Wertet den Code aus und erzeugt
 *				daraus einen vector
 * vector:
 *		Par[0] = IO-Type
 *		Par[1] = IO-Name
 *		Par[2-n] = Type abhängige Parameter
 * Param:	strLine : Codezeile
 * Return:	Fehler als char* (NULL = IO)
 * Version:
 *	V1.0.0	Erstellt	21.05.2018	JCA
 **********************************************/
int cCode::insertIO(char* strLine){
	#ifdef _DEBUG_ON
		Serial.println("cCode::insertIO");
	#endif
	std::vector<char*> Par;
	char* tokken;
	int Len;
	tokken = strtok(strLine, ";");
	//Char* aufsplitten
	while(tokken != NULL){
		Par.push_back(tokken);
		tokken = strtok(NULL, ";");
	}
	Len = Par.size();
	#ifdef _DEBUG_ON
		Serial.print("cCode::insertIO - ParAnz: ");
		Serial.println(Len);
	#endif
	if(Len >= JCA_IOT_IO_L_ROOT){
		//Mindestgrösse gegeben
		int Type;
		int MinLen;
		int Index;
		if(this->getTypeIO(Par[0], &Type, &MinLen)){
			#ifdef _DEBUG_ON
				Serial.print("cCode::insertIO - Type: ");
				Serial.println(Type);
				Serial.print("      :---------- MinLen: ");
				Serial.println(MinLen);
			#endif
			if(Len >= MinLen){
				Index = this->findIO(Par[1]);
				if(Index >= 0){
					this->updateIO(Par, Type, Index);
				}else{
					this->addIO(Par, Type);
				}
				return JCA_IOT_CODE_IO_OK;
			}else{
				return JCA_IOT_CODE_IO_MINPAR;
			}
		}else{
			return JCA_IOT_CODE_IO_UNKNOWN;
		}
	}else{
		return JCA_IOT_CODE_IO_NONAME;
	}
}

/**********************************************
 * Methode: deleteIO(char* strName)
 * Info:	Löscht ein IO anhand des Namen
 * Param:	strName : IO-Name
 * Return:	true wenn gelöscht
 * Version:
 *	V1.0.0	Erstellt	21.05.2018	JCA
 **********************************************/
bool cCode::deleteIO(char* strName){
	#ifdef _DEBUG_ON
		Serial.println("cCode::deleteIO(char*)");
	#endif
	return this->deleteIO(this->findIO(strName));
}

/**********************************************
 * Methode: getIO(char* strName)
 * Info:	Gibt den Pointer auf ein IO-Object
 *				zurück.
 * Param:	strName : Name des IO
 * Return:	Pointer of cRoot
 * Version:
 *	V1.0.0	Erstellt	25.05.2018	JCA
 **********************************************/
IO::cRoot* cCode::getIO(char* strName){
	#ifdef _DEBUG_ON
		Serial.println("cCode::getIO(Name)");
	#endif
	int ioPos = this->findIO(strName);
	if(ioPos >= 0){
		return IOs[ioPos];
	}else{
		return NULL;
	}
}
IO::cRoot* cCode::getIO(int iIndex){
	#ifdef _DEBUG_ON
		Serial.println("cCode::getIO(Index)");
	#endif
	if(iIndex < this->IOs.size()){
		return IOs[iIndex];
	}else{
		return NULL;
	}
}

/**********************************************
 * Methode: getCodeSize()
 * Info:	Gibt die Größe des Code-Vectors zurück
 * Param:
 * Return:	Anzahl Code Zeilen [int]
 * Version:
 *	V1.0.0	Erstellt	23.05.2018	JCA
 **********************************************/
int cCode::getCodeSize(){
	#ifdef _DEBUG_ON
		Serial.println("cCode::getCodeSize()");
	#endif
	return this->Code.size();
}

/**********************************************
 * Methode: getIoSize()
 * Info:	Gibt die Größe des IO-Vectors zurück
 * Param:
 * Return:	Anzahl IOs [int]
 * Version:
 *	V1.0.0	Erstellt	17.09.2018	JCA
 **********************************************/
int cCode::getIoSize(){
	#ifdef _DEBUG_ON
		Serial.println("cCode::getIoSize()");
	#endif
	return this->IOs.size();
}

/**********************************************
 * Methode: getFaultIO()
 * Info:	Gibt die erste fehlerhafte IO-Zeile zurück
 * Param:
 * Return:	Fehlerhafte IO-Zeile [int]
 * Version:
 *	V1.0.0	Erstellt	13.09.2018	JCA
 **********************************************/
int cCode::getFaultIO(){
	#ifdef _DEBUG_ON
		Serial.println("cCode::getFaultIO()");
	#endif
	return this->faultIO;
}

/**********************************************
 * Methode: getFaultCode()
 * Info:	Gibt die erste fehlerhafte Code-Zeile zurück
 * Param:
 * Return:	Fehlerhafte Code-Zeile [int]
 * Version:
 *	V1.0.0	Erstellt	13.09.2018	JCA
 **********************************************/
int cCode::getFaultCode(){
	#ifdef _DEBUG_ON
		Serial.println("cCode::getFaultCode()");
	#endif
	return this->faultCode;
}

/**********************************************
 * Methode: addIO(std::vector<char*> vPar, int iType)
 * Info:	fügt dem IO-Vector ein Element hinzu
 * Param:
 *		vPar = Vector aller Char Parameter
 *		iType = Type-Kennung des IOs
 * Return:
 * Version:
 *	V1.0.0	Erstellt	21.05.2018	JCA
 **********************************************/
void cCode::addIO(std::vector<char*> vPar, int iType){
	#ifdef _DEBUG_ON
		Serial.println("cCode::addIO");
	#endif
	int Size = vPar.size();
	int Port;
	int32_t lValue;
	float fValue;
	bool bValue;
	switch(iType){
		case JCA_IOT_IO_T_LONG :
			if(Size == JCA_IOT_IO_L_LONG){
				this->IOs.push_back(new IO::cLong(vPar[1]));
			}else{
				this->IOs.push_back(new IO::cLong(vPar[1], atoi(vPar[2])));
			}
			break;
		case JCA_IOT_IO_T_FLOAT :
			if(Size == JCA_IOT_IO_L_FLOAT){
				this->IOs.push_back(new IO::cFloat(vPar[1]));
			}else{
				this->IOs.push_back(new IO::cFloat(vPar[1], atof(vPar[2])));
			}
			break;
		case JCA_IOT_IO_T_BOOL :
			if(Size == JCA_IOT_IO_L_BOOL){
				this->IOs.push_back(new IO::cBool(vPar[1]));
			}else{
				this->IOs.push_back(new IO::cBool(vPar[1], (bool)atoi(vPar[2])));
			}
			break;
		case JCA_IOT_IO_T_DO :
			if(Size == JCA_IOT_IO_L_DO){
				this->IOs.push_back(new IO::cDO(vPar[1],atoi(vPar[2])));
			}else{
				this->IOs.push_back(new IO::cDO(vPar[1],atoi(vPar[2]),(bool)atoi(vPar[3])));
			}
			break;
		case JCA_IOT_IO_T_DI :
			if(Size == JCA_IOT_IO_L_DI){
				this->IOs.push_back(new IO::cDI(vPar[1],atoi(vPar[2]),(bool)atoi(vPar[3])));
			}else{
				this->IOs.push_back(new IO::cDI(vPar[1],atoi(vPar[2]),(bool)atoi(vPar[3]),(bool)atoi(vPar[4])));
			}
			break;
		case JCA_IOT_IO_T_AO :
			if(Size == JCA_IOT_IO_L_AO){
				this->IOs.push_back(new IO::cAO(vPar[1],atoi(vPar[2]),atof(vPar[3]),atof(vPar[4])));
			}else{
				this->IOs.push_back(new IO::cAO(vPar[1],atoi(vPar[2]),atof(vPar[3]),atof(vPar[4]),atof(vPar[5])));
			}
			break;
		case JCA_IOT_IO_T_AI :
			if(Size == JCA_IOT_IO_L_AI){
				this->IOs.push_back(new IO::cAI(vPar[1],atoi(vPar[2]),atof(vPar[3]),atof(vPar[4])));
			}else{
				this->IOs.push_back(new IO::cAI(vPar[1],atoi(vPar[2]),atof(vPar[3]),atof(vPar[4]),atof(vPar[5])));
			}
			break;
	}
}

/**********************************************
 * Methode: updateIO(std::vector<char*> vPar, int iType)
 * Info:	Wertet den Code aus und erzeugt
 *				daraus einen vector
 * Param:
 *		vPar = Vector aller Char Parameter
 *		iType = Typekennung des IO
 *		iIndex = Index des IO im IOS-Vector
 * Return:
 * Version:
 *	V1.0.0	Erstellt	21.05.2018	JCA
 **********************************************/
void cCode::updateIO(std::vector<char*> vPar, int iType, int iIndex){
	#ifdef _DEBUG_ON
		Serial.println("cCode::updateIO");
	#endif
	int Port;
	int32_t lValue;
	float fValue;
	bool bValue;
	if(this->IOs[iIndex]->getType() != iType){
		if(this->deleteIO(iIndex)){
			this->addIO(vPar, iType);
		}
	}
}

/**********************************************
 * Methode: findIO(char* strName)
 * Info:	Sucht ein IO im Vector anhand des
 *				Namen.
 * Param:	strName = Name des gesuchten IOs
 * Return:	Index des gesuchten IOs (-1 = nicht gefunden)
 * Version:
 *	V1.0.0	Erstellt	21.05.2018	JCA
 **********************************************/
int cCode::findIO(char* strName){
	#ifdef _DEBUG_ON
		Serial.print("cCode::findIO - ");
		Serial.println(strName);
	#endif
	int i;
	for(i=0; i<this->IOs.size(); i++){
		#ifdef _DEBUG_ON
			Serial.print("cCode::findIO - Index:");
			Serial.println(i);
			Serial.print(" Name:");
			Serial.println(this->IOs[i]->getName());
		#endif
		if(0 == strcmp(strName, this->IOs[i]->getName())){
			return i;
		}
	}

	return -1;
}

/**********************************************
 * Methode: getTypeIO(char* strType, int* iType, int* iMinLen)
 * Info:	Ermittelt die Typekennung und die
 *				Mindestlänge der Parameter
 * Param:
 *		strType = Type-String
 *		*iType  = Rückgabewert des gefunden Types
 *		*iMinLen= Mindest anzahl an Parametern
 * Return:	true wenn gefunden
 * Version:
 *	V1.0.0	Erstellt	21.05.2018	JCA
 **********************************************/
bool cCode::getTypeIO(char* strType, int* iType, int* iMinLen){
	#ifdef _DEBUG_ON
		Serial.println("cCode::getTypeIO");
	#endif
	if(0 == strcmp(strType, "int32_t")){
		*iType   = JCA_IOT_IO_T_LONG;
		*iMinLen = JCA_IOT_IO_L_LONG;
		return true;
	}
	if(0 == strcmp(strType, "Float")){
		*iType   = JCA_IOT_IO_T_FLOAT;
		*iMinLen = JCA_IOT_IO_L_FLOAT;
		return true;
	}
	if(0 == strcmp(strType, "Bool")){
		*iType   = JCA_IOT_IO_T_BOOL;
		*iMinLen = JCA_IOT_IO_L_BOOL;
		return true;
	}
	if(0 == strcmp(strType, "DO")){
		*iType   = JCA_IOT_IO_T_DO;
		*iMinLen = JCA_IOT_IO_L_DO;
		return true;
	}
	if(0 == strcmp(strType, "DI")){
		*iType   = JCA_IOT_IO_T_DI;
		*iMinLen = JCA_IOT_IO_L_DI;
		return true;
	}
	if(0 == strcmp(strType, "AO")){
		*iType   = JCA_IOT_IO_T_AO;
		*iMinLen = JCA_IOT_IO_L_AO;
		return true;
	}
	if(0 == strcmp(strType, "AI")){
		*iType   = JCA_IOT_IO_T_AI;
		*iMinLen = JCA_IOT_IO_L_AI;
		return true;
	}

	//Keine Type gefunden
	return false;
}

/**********************************************
 * Methode: deleteIO(int iIndex)
 * Info:	Löscht ein IO anhand des Index
 * Param:	iIndex = IO-Index im Vector
 * Return:	true wenn gelöscht
 * Version:
 *	V1.0.0	Erstellt	21.05.2018	JCA
 **********************************************/
bool cCode::deleteIO(int iIndex){
	#ifdef _DEBUG_ON
		Serial.println("cCode::deleteIO(int)");
	#endif
	if(iIndex >= 0 && iIndex < this->IOs.size()){
		std::vector<IO::cRoot*>::iterator iter = this->IOs.begin() + iIndex;
		this->IOs.erase(iter);
		return true;
	}else{
		return false;
	}
}

/**********************************************
 * Methode: insertCode(char* strLine)
 * Info:	Wertet den Code aus und erzeugt
 *				daraus einen vector
 * vector:
 *		Par[0] = Code-Type
 *		Par[1] = Code-Wert
 * Param:	strLine : Codezeile
 * Return:	Fehler als int (0 = IO)
 * Version:
 *	V1.0.0	Erstellt	22.05.2018	JCA
 **********************************************/
int cCode::insertCode(char* strLine){
	#ifdef _DEBUG_ON
		Serial.println("cCode::insertCode");
	#endif
	std::vector<char*> Par;
	char* tokken;
	int Len;
	int i;
	char strText[JCA_IOT_CODE_NAMELEN] = "";

	tokken = strtok(strLine, " ");
	//Char* aufsplitten
	while(tokken != NULL){
		Par.push_back(tokken);
		tokken = strtok(NULL, " ");
	}
	Len = Par.size();
	#ifdef _DEBUG_ON
		Serial.print("cCode::insertCode - ParAnz: ");
		Serial.println(Len);
	#endif
	if(Len >= 1){
		//Mindestgrösse gegeben
		int Type;
		int valuePos;
		int MinLen;
		int Index;
		int Function;
		if(this->getFunctionCode(Par[0], &Function, &MinLen)){
			#ifdef _DEBUG_ON
				Serial.print("cCode::insertCode - Function: ");
				Serial.println(Function);
				Serial.print("      :------------ MinLen: ");
				Serial.println(MinLen);
			#endif
			if(MinLen == 1){
				if(Function == JCA_IOT_CODE_CMD_DEST){
					//Die Code-Zeile ist ein Sprungziel
					// im Vector der Sprungziele wird der Name und die Codzeile hinterlegt
					tokken = Par[0];
					while(*tokken){
						if(*tokken == ' ' || *tokken == '[' || *tokken == ']'){
							tokken++;
						}else{
							strText[i] = *tokken;
							i++;
							tokken++;
						}
					}
					this->JmpDest.push_back(new cJump(this->Code.size(), strText));
				}
				this->Code.push_back(new cCodeLine(Function,0,0));
				return JCA_IOT_CODE_CMD_OK;
			}else if(Len >= MinLen){
				if(this->getValueCode(Par[1], Function, &Type, &valuePos)){
					#ifdef _DEBUG_ON
						Serial.print("      :------------ Type: ");
						Serial.println(Type);
						Serial.print("      :------------ Pos: ");
						Serial.println(valuePos);
					#endif
					this->Code.push_back(new cCodeLine(Function, Type, valuePos));
					return JCA_IOT_CODE_CMD_OK;
				}else{
					return JCA_IOT_CODE_CMD_VALUE;
				}
			}else{
				return JCA_IOT_CODE_CMD_MINPAR;
			}
		}else{
			return JCA_IOT_CODE_CMD_UNKNOWN;
		}
	}else{
		return JCA_IOT_CODE_CMD_NOCMD;
	}
}

/**********************************************
 * Methode: getFunctionCode(char* strFunc, int* iFunc, int* iMinLen)
 * Info:	Ermittelt die Funktionskennung und die
 *				Mindestlänge der Parameter
 * Param:
 *		strFunc = FunktionsString
 *		*iFunc  = Rückgabewert der gefunden Funktion
 *		*iMinLen= Mindest anzahl an Parametern
 * Return:	true wenn gefunden
 * Version:
 *	V1.0.0	Erstellt	22.05.2018	JCA
 **********************************************/
bool cCode::getFunctionCode(char* strFunc, int* iFunc, int* iMinLen){
	#ifdef _DEBUG_ON
		Serial.println("cCode::getFunctionCode");
	#endif
	//Sprünge
	if(0 == strcmp(strFunc, "SPB")){
		*iFunc   = JCA_IOT_CODE_CMD_SPB;
		*iMinLen = 2;
		return true;
	}
	if(0 == strcmp(strFunc, "SPN")){
		*iFunc   = JCA_IOT_CODE_CMD_SPN;
		*iMinLen = 2;
		return true;
	}
	if(strstr(strFunc, "[") && strstr(strFunc, "]")){
		*iFunc   = JCA_IOT_CODE_CMD_DEST;
		*iMinLen = 1;
		return true;
	}
	//Klammer Funktionen
	if(0 == strcmp(strFunc, "A(")){
		*iFunc   = JCA_IOT_CODE_CMD_AK;
		*iMinLen = 1;
		return true;
	}
	if(0 == strcmp(strFunc, "O(")){
		*iFunc   = JCA_IOT_CODE_CMD_OK;
		*iMinLen = 1;
		return true;
	}
	if(0 == strcmp(strFunc, "AN(")){
		*iFunc   = JCA_IOT_CODE_CMD_ANK;
		*iMinLen = 1;
		return true;
	}
	if(0 == strcmp(strFunc, "ON(")){
		*iFunc   = JCA_IOT_CODE_CMD_ONK;
		*iMinLen = 1;
		return true;
	}
	if(0 == strcmp(strFunc, ")")){
		*iFunc   = JCA_IOT_CODE_CMD_KE;
		*iMinLen = 1;
		return true;
	}
	//Logik Funktionen
	if(0 == strcmp(strFunc, "N")){
		*iFunc   = JCA_IOT_CODE_CMD_N;
		*iMinLen = 1;
		return true;
	}
	if(0 == strcmp(strFunc, "A")){
		*iFunc   = JCA_IOT_CODE_CMD_A;
		*iMinLen = 2;
		return true;
	}
	if(0 == strcmp(strFunc, "O")){
		*iFunc   = JCA_IOT_CODE_CMD_O;
		*iMinLen = 2;
		return true;
	}
	if(0 == strcmp(strFunc, "AN")){
		*iFunc   = JCA_IOT_CODE_CMD_AN;
		*iMinLen = 2;
		return true;
	}
	if(0 == strcmp(strFunc, "ON")){
		*iFunc   = JCA_IOT_CODE_CMD_ON;
		*iMinLen = 2;
		return true;
	}
	if(0 == strcmp(strFunc, "=")){
		*iFunc   = JCA_IOT_CODE_CMD_Z;
		*iMinLen = 2;
		return true;
	}
	if(0 == strcmp(strFunc, "S")){
		*iFunc   = JCA_IOT_CODE_CMD_S;
		*iMinLen = 2;
		return true;
	}
	if(0 == strcmp(strFunc, "R")){
		*iFunc   = JCA_IOT_CODE_CMD_R;
		*iMinLen = 2;
		return true;
	}
	//Festpunkt Funtionen
	if(0 == strcmp(strFunc, "LL")){
		*iFunc   = JCA_IOT_CODE_CMD_L_L;
		*iMinLen = 2;
		return true;
	}
	if(0 == strcmp(strFunc, "L-")){
		*iFunc   = JCA_IOT_CODE_CMD_L_SUB;
		*iMinLen = 2;
		return true;
	}
	if(0 == strcmp(strFunc, "L+")){
		*iFunc   = JCA_IOT_CODE_CMD_L_ADD;
		*iMinLen = 2;
		return true;
	}
	if(0 == strcmp(strFunc, "L*")){
		*iFunc   = JCA_IOT_CODE_CMD_L_MUL;
		*iMinLen = 2;
		return true;
	}
	if(0 == strcmp(strFunc, "L/")){
		*iFunc   = JCA_IOT_CODE_CMD_L_DIV;
		*iMinLen = 2;
		return true;
	}
	if(0 == strcmp(strFunc, "L>")){
		*iFunc   = JCA_IOT_CODE_CMD_L_GT;
		*iMinLen = 2;
		return true;
	}
	if(0 == strcmp(strFunc, "L<")){
		*iFunc   = JCA_IOT_CODE_CMD_L_LT;
		*iMinLen = 2;
		return true;
	}
	if(0 == strcmp(strFunc, "L>=")){
		*iFunc   = JCA_IOT_CODE_CMD_L_GE;
		*iMinLen = 2;
		return true;
	}
	if(0 == strcmp(strFunc, "L<=")){
		*iFunc   = JCA_IOT_CODE_CMD_L_LE;
		*iMinLen = 2;
		return true;
	}
	if(0 == strcmp(strFunc, "L<>")){
		*iFunc   = JCA_IOT_CODE_CMD_L_NE;
		*iMinLen = 2;
		return true;
	}
	if(0 == strcmp(strFunc, "L==")){
		*iFunc   = JCA_IOT_CODE_CMD_L_EQ;
		*iMinLen = 2;
		return true;
	}
	if(0 == strcmp(strFunc, "LT")){
		*iFunc   = JCA_IOT_CODE_CMD_L_T;
		*iMinLen = 2;
		return true;
	}
	//Gleitpunkt Funtionen
	if(0 == strcmp(strFunc, "FL")){
		*iFunc   = JCA_IOT_CODE_CMD_F_L;
		*iMinLen = 2;
		return true;
	}
	if(0 == strcmp(strFunc, "F-")){
		*iFunc   = JCA_IOT_CODE_CMD_F_SUB;
		*iMinLen = 2;
		return true;
	}
	if(0 == strcmp(strFunc, "F+")){
		*iFunc   = JCA_IOT_CODE_CMD_F_ADD;
		*iMinLen = 2;
		return true;
	}
	if(0 == strcmp(strFunc, "F*")){
		*iFunc   = JCA_IOT_CODE_CMD_F_MUL;
		*iMinLen = 2;
		return true;
	}
	if(0 == strcmp(strFunc, "F/")){
		*iFunc   = JCA_IOT_CODE_CMD_F_DIV;
		*iMinLen = 2;
		return true;
	}
	if(0 == strcmp(strFunc, "F>")){
		*iFunc   = JCA_IOT_CODE_CMD_F_GT;
		*iMinLen = 2;
		return true;
	}
	if(0 == strcmp(strFunc, "F<")){
		*iFunc   = JCA_IOT_CODE_CMD_F_LT;
		*iMinLen = 2;
		return true;
	}
	if(0 == strcmp(strFunc, "F>=")){
		*iFunc   = JCA_IOT_CODE_CMD_F_GE;
		*iMinLen = 2;
		return true;
	}
	if(0 == strcmp(strFunc, "F<=")){
		*iFunc   = JCA_IOT_CODE_CMD_F_LE;
		*iMinLen = 2;
		return true;
	}
	if(0 == strcmp(strFunc, "F<>")){
		*iFunc   = JCA_IOT_CODE_CMD_F_NE;
		*iMinLen = 2;
		return true;
	}
	if(0 == strcmp(strFunc, "F==")){
		*iFunc   = JCA_IOT_CODE_CMD_F_EQ;
		*iMinLen = 2;
		return true;
	}
	if(0 == strcmp(strFunc, "FT")){
		*iFunc   = JCA_IOT_CODE_CMD_F_T;
		*iMinLen = 2;
		return true;
	}

	//Keine Funktion gefunden
	return false;
}

/**********************************************
 * Methode: getValueCode(char* strValue, int* iType, int* iPos)
 * Info:	Ermittelt den Variablentype und
 *				sucht diesen in den IOs bzw.
 *				schreibt ihn in die Konsatnten.
 * Param:
 *		strValue= VariablenString
 *		iFunc   = Befehls Funktions-Code als Int
 *		*iType  = VectorType für die Variable
 *		*iPos   = IN : Codezeile
 *				  OUT: Position der Variable im Vektor
 * Wertaufbau:
 *		{IO}
 *		Konstante/Ziel
 *		#x#Spezialkonstante
 *		[Sprungmarke]
 * Return:	true wenn gefunden
 * Version:
 *	V1.0.0	Erstellt	22.05.2018	JCA
 **********************************************/
bool cCode::getValueCode(char* strValue, int iFunc, int* iType, int* iPos){
	#ifdef _DEBUG_ON
		Serial.println("cCode::getValueCode");
	#endif

	bool valueFound = false;
	char* pch;
	char strText[JCA_IOT_CODE_NAMELEN] = "";
	int i = 0;

	//Function ist eine Klammeroperation
	if(iFunc < 100){
		*iType = 0;
		return true;
	}

	//Function ist ein Sprung
	if(iFunc > 900 && iFunc < 910){
		*iType = 0;
		pch = strValue;
		while(*pch){
			if(*pch == ' '){
				pch++;
			}else{
				strText[i] = *pch;
				i++;
				pch++;
			}
		}
		this->JmpLine.push_back(new cJump(*iPos, strText));
		return true;
	}

	//Variable ist ein IO
	if(strstr(strValue, "{") && strstr(strValue, "}")){
		*iType = JCA_IOT_CODE_VECTOR_IO;
		pch = strstr(strValue, "{");
		pch++;
		strcpy(strText, pch);
		pch = strstr(strText, "}");
		*pch = 0;
		*iPos = this->findIO(strText);
		if(*iPos < 0){
			return false;
		}
		return true;
	}

	//Variale ist eine Spezialkonstante, Konvertierung zu int32_t / Float
	if(strstr(strValue, "#")){
		//noch nicht unterstützt
		return false;
	}

	//Variable ist eine Konstante
	if(iFunc > 100 && iFunc < 200){
		//Bool
		bool vConst = (bool)atoi(strValue);
		*iType = JCA_IOT_CODE_VECTOR_BOOL;
		*iPos = this->boolConst.size();
		this->boolConst.push_back(vConst);
		return true;
	}
	if(iFunc > 200 && iFunc < 300){
		//int32_t
		int32_t vConst = (int32_t)atoi(strValue);
		*iType = JCA_IOT_CODE_VECTOR_LONG;
		*iPos = this->longConst.size();
		this->longConst.push_back(vConst);
		return true;
	}
	if(iFunc > 300 && iFunc < 400){
		//Float
		float vConst = (float)atof(strValue);
		*iType = JCA_IOT_CODE_VECTOR_FLOAT;
		*iPos = this->floatConst.size();
		this->floatConst.push_back(vConst);
		return true;
	}

	//Keine Funktion gefunden
	//return false;
}
