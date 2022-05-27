void LoadSettings();
void LoadArea();
void LoadMission();
void GetPhoto();
void GetLog();
void GetStatus();
void ContinueLoadedMission();
boolean takeoff();
void GoHome();
void LandNow();


void emailReaderCallBack(){
  Consola.print("CODE: ");
  Consola.println(Code);
  switch (Code){
    case 00:{     //LoadSettings

      LoadSettings();
      emailReader.disable();
      break;
    }
    case 10:{     //LoadArea
      
      LoadArea();
      emailReader.disable();
      //enviarReportes.enable(); // No sé si está bien que esté esto acá
      // además checkEmail sigue prendida?
      break;
    }
    case 20:{      //LoadMission

      LoadMission();
      emailReader.disable();
      break;
    }
    case 30:{      //GetPhoto

      GetPhoto();
      emailReader.disable();
      break;
    }
    case 40:{      //GetLog

      GetLog();
      emailReader.disable();
      break;
    }
    case 50:{      //GetStatus

      GetStatus();
      emailReader.disable();
      break;
    }
    case 60:{      //ContinueLoadedMission

      ContinueLoadedMission();
      emailReader.disable();
      break;
    }
    case 70:{      //Takeoff

      takeoff();
      emailReader.disable();
      break;
    }
    case 80:{      //GoHome

      GoHome();
      emailReader.disable();
      break;
    }
    case 90:{      //LandNow

      LandNow();
      emailReader.disable();
      break;
    }
    
    default:
      emailReader.disable();
      break;
  }
}

void LoadArea(){
    if (iBody < 2){
      memset(SubjectSend,'\0', SubjectSize);
      sprintf(SubjectSend, "Misión PX4 cancelada");
      memset(BodySend,'\0', BodySize);
      sprintf(BodySend, "La mision solicitada no contiene una cantidad de puntos apropiada.");
      //sendEmail.enable();//Enviar un mail
        
      #ifdef  ConsolaMon
      Consola.println("Mision vacía");
      #endif
      return;
    }
    int i = 0;
    int j = 0;
    counter = 0;
    memset(aux_str,'\0', 128);
    #ifdef  ConsolaMon
    Consola.println(F("Comienza evaluacion de mision"));
    Consola.println(F("Puntos recibidos:"));
    Consola.print(Body);
    #endif
    j=0;
    do{
      i=0;
      do{ //Latitud
        if ((Body[j] == ',') or (Body[j] == '.')){
          j++;
        }
        else{
          aux_str[i] = Body[j];
          i++;
          j++;
          coordenadas[counter].latitud = atoi(aux_str);
        }
      }while (Body[j] != ';');
	  if (i<10){
		if (aux_str[0]=='-'){
		  for (i;i<10;i++){
		    aux_str[i]='0';
		  }
		}
		else{
		  for (i;i<9;i++){
		    aux_str[i]='0';
		  }
		}
	  }
      
      #ifdef  ConsolaMon
      Consola.print(F("Latitud: "));
      Consola.println(int32_t(coordenadas[counter].latitud));
      #endif
      memset(aux_str,'\0', 128);
      i=0;
      j++;
      do{ //Longitud
        if ((Body[j] == ',') or (Body[j] == '.')){
          j++;
        }
        else{
          aux_str[i] = Body[j];
          i++;
          j++;
          coordenadas[counter].longitud = atoi(aux_str);
        }
      }while (Body[j] != ';');
	  if (i<10){
  		if (aux_str[0]=='-'){
  		  for (i;i<10;i++){
  		    aux_str[i]='0';
  		  }
  		}
  		else{
  		  for (i;i<9;i++){
  		    aux_str[i]='0';
  		  }
  		}
	  }
      
      #ifdef  ConsolaMon
      Consola.print(F("Longitud: "));
      Consola.println(int32_t(coordenadas[counter].longitud));
      #endif
      memset(aux_str,'\0', 128);
      i=0;
      j++;
      do{ //Altitud
        if ((Body[j] == ',') or (Body[j] == '.')){
          j++;
        }
        else{
          aux_str[i] = Body[j];
          i++;
          j++;
          coordenadas[counter].altitud = atoi(aux_str); 
        }
      }while (Body[j] != ';');
      
      #ifdef  ConsolaMon
      Consola.print(F("Altitud: "));
      Consola.println(coordenadas[counter].altitud);
      #endif
      if (Body[j]==';'){
        j=j+3; //saltea el \r\n del final de cada línea
      }
      memset(aux_str,'\0', 128);
      counter++;
    }while(j < iBody);
    
    if (counter >= AREAMAXPUNTOS){
      memset(SubjectSend,'\0', SubjectSize);
      sprintf(SubjectSend, "Misión PX4 cancelada");
      memset(BodySend,'\0', BodySize);
      sprintf(BodySend, "La mision solicitada contiene más puntos que los soportados. La cantidad de puntos soportados para el calculo de trayectoria es de 10.");
      //Enviar un mail
        
      #ifdef  ConsolaMon
      Consola.println("Mision con demasiados puntos");
      #endif
      return;
    }
    
    iBody = 0;
    memset(Body,'\0', 300);
    #ifdef  ConsolaMon
    Consola.println(F("Fin lectura de puntos de mision"));
    #endif

    /*float alcancemax = 0;
    for (int i=0; i<(counter); i++){
      float distvertice = sqrt(sq(coordenadas[i].latitud - homepos[0].latitud) + sq(coordenadas[i].longitud - homepos[0].longitud));
      if(alcancemax < distvertice){
        alcancemax = distvertice;
      }
    }
    if (alcance3DR < alcancemax){
      memset(SubjectSend,'\0', 300);
      sprintf(SubjectSend, "Misión PX4 cancelada");
      memset(BodySend,'\0', 300);
      sprintf(BodySend, "Área de misión por fuera del alcance de comunicación. Alcance máximo de comunicación: %d m. Alcance máximo de misión: %d m.", alcance3DR, alcancemax);
      //Enviar un mail
      
      #ifdef  ConsolaMon
      Consola.println(F("Área de misión por fuera del alcance de comunicación"));
      #endif
      return;
    }*/

    
    NPuntos = CalcularTrayectoria(coordenadas,counter);
    float totaltray;
    for (int i=0; i<(NPuntos-1); i++){
       totaltray = totaltray + sqrt(sq(Mision[i].latitud - Mision[i+1].latitud) + sq(Mision[i].longitud - Mision[i+1].longitud));
    }
 /*   
    if (carga_bateria < totaltray*tray2bat){
      memset(SubjectSend,'\0', 300);
      sprintf(SubjectSend, "Misión PX4 cancelada");
      memset(BodySend,'\0', 300);
      sprintf(BodySend, "Batería insuficiente para la misión. Batería cargada al: %d %. Batería necesaria: %d%.", carga_bateria, totaltray*tray2bat);
      //Enviar un mail
      
      #ifdef  ConsolaMon
      Consola.println(F("Batería insuficiente para la misión"));
      #endif
      return;
    }
*/
    EnviarMision();
    SubjectFlag=0;
}

void LoadSettings(){
 
  if(strstr(Body,"LOGMAIL: TRUE")!=NULL){   
    flg_LogMail = true;       
  }
  if(strstr(Body,"IMG: TRUE")!=NULL){   
    flg_Img = true;       
  }
  if(strstr(Body,"ALERTAS: TRUE")!=NULL){   
    flg_Alertas = true;       
  }
  if(strstr(Body,"REPORTE: TRUE")!=NULL){   
    flg_ReporteFinal = true;       
  }

  if(strstr(Body,"MODO_DET: FOLLOW")!=NULL){   
    MISSION_MODE = MISSION_MODE_FOLLOW;       
  }
  if(strstr(Body,"MODO_DET: RTL")!=NULL){   
    MISSION_MODE = MISSION_MODE_RTL;       
  }
  if(strstr(Body,"MODO_DET: CONT")!=NULL){   
    MISSION_MODE = MISSION_MODE_CONT;       
  }


  uint8_t data_OB[2];
  int count_data_OB;
  
  data[0] = ONBOARDHEADER;
  data[1] = MISSION_MODE; 
  count_data_OB = sizeof(data_OB);
  sendToOnBoard(count_data_OB, data_OB);
  
  enviarReportes.enable();
  
}

void LoadMission(){
  if (iBody < 2){
      memset(SubjectSend,'\0', SubjectSize);
      sprintf(SubjectSend, "Misión PX4 cancelada");
      memset(BodySend,'\0', BodySize);
      sprintf(BodySend, "La mision solicitada no contiene una cantidad de puntos apropiada.");
      //sendEmail.enable();//Enviar un mail
        
      #ifdef  ConsolaMon
      Consola.println("Mision vacía");
      #endif
      return;
    }
    int i = 0;
    int j = 0;
    counter = 0;
    memset(aux_str,'\0', 128);
    #ifdef  ConsolaMon
    Consola.println(F("Comienza evaluacion de mision"));
    Consola.println(F("Puntos recibidos:"));
    Consola.print(Body);
    #endif
    j=0;
    do{
      i=0;
      do{ //Latitud
        if ((Body[j] == ',') or (Body[j] == '.')){
          j++;
        }
        else{
          aux_str[i] = Body[j];
          i++;
          j++;
          coordenadas[counter].latitud = atoi(aux_str);
        }
      }while (Body[j] != ';');
    if (i<10){
      if (aux_str[0]=='-'){
        for (i;i<10;i++){
          aux_str[i]='0';
        }
      }
    else{
      for (i;i<9;i++){
        aux_str[i]='0';
        }
      }
    }
      
    #ifdef  ConsolaMon
    Consola.print(F("Latitud: "));
    Consola.println(int32_t(coordenadas[counter].latitud));
    #endif
    memset(aux_str,'\0', 128);
    i=0;
    j++;
    do{ //Longitud
      if ((Body[j] == ',') or (Body[j] == '.')){
        j++;
      }
      else{
        aux_str[i] = Body[j];
        i++;
        j++;
        coordenadas[counter].longitud = atoi(aux_str);
      }
    }while (Body[j] != ';');
    if (i<10){
      if (aux_str[0]=='-'){
        for (i;i<10;i++){
          aux_str[i]='0';
        }
      }
      else{
        for (i;i<9;i++){
          aux_str[i]='0';
        }
      }
    }
      
      #ifdef  ConsolaMon
      Consola.print(F("Longitud: "));
      Consola.println(int32_t(coordenadas[counter].longitud));
      #endif
      memset(aux_str,'\0', 128);
      i=0;
      j++;
      do{ //Altitud
        if ((Body[j] == ',') or (Body[j] == '.')){
          j++;
        }
        else{
          aux_str[i] = Body[j];
          i++;
          j++;
          coordenadas[counter].altitud = atoi(aux_str); 
        }
      }while (Body[j] != ';');
      
      #ifdef  ConsolaMon
      Consola.print(F("Altitud: "));
      Consola.println(coordenadas[counter].altitud);
      #endif
      if (Body[j]==';'){
        j=j+3; //saltea el \r\n del final de cada línea
      }
      memset(aux_str,'\0', 128);
      counter++;
    }while(j < iBody);
    
    if (counter >= MISIONMAXPUNTOS){
      memset(SubjectSend,'\0', SubjectSize);
      sprintf(SubjectSend, "Misión PX4 cancelada");
      memset(BodySend,'\0', BodySize);
      sprintf(BodySend, "La mision solicitada contiene más puntos que los soportados. La cantidad de puntos soportados para el calculo de trayectoria es de 50.");
      sendEmail.enable();//Enviar un mail
        
      #ifdef  ConsolaMon
      Consola.println("Mision con demasiados puntos");
      #endif
      return;
    }
    
    iBody = 0;
    memset(Body,'\0', 300);
    #ifdef  ConsolaMon
    Consola.println(F("Fin lectura de puntos de mision"));
    #endif
    
    NPuntos = counter;
    /*float totaltray;
    for (int i=0; i<(NPuntos-1); i++){
       totaltray = totaltray + sqrt(sq(Mision[i].latitud - Mision[i+1].latitud) + sq(Mision[i].longitud - Mision[i+1].longitud));
    }*/
    for (int i = 0; i < counter; i++){
      Mision[i].latitud = coordenadas[i].latitud;
      Mision[i].longitud = coordenadas[i].longitud;
      Mision[i].altitud = coordenadas[i].altitud;
    }
    EnviarMision();
    SubjectFlag=0;
  
}

void GetPhoto(){
  
}

void GetLog(){
  
}

void GetStatus(){
  
}

void ContinueLoadedMission(){
  
}


boolean Takeoff(){

  int altura = 3;
  // Si el Dron esta armado, no hacemos nada
  if(Pixhawk_landed_state != MAV_LANDED_STATE_ON_GROUND){
    #ifdef  ConsolaMon
    Consola.println(F("No se puede garantizar que el dron esté en tierra, no se realizará el despegue"));
    #endif  
    return false;
  }

  // Si esta desarmado le borramos los wp que tenga guardados  
  clearall();
  

  // Esperamos un GPS LOCK para setear el HOME
  haveToSetHome=true;
  waitfor(MAVLINK_MSG_ID_GLOBAL_POSITION_INT, GPSTIMEOUT);
  
  if (GPSLock == 1) {
    #ifdef  ConsolaMon
    Consola.println(F("GPSLock, seteando HOME"));
    #endif
    setHome();
  }
  else{
    #ifdef  ConsolaMon
    Consola.println(F("No hay GPSLock"));
    #endif
    return false;
  }

  NPuntos = 2; // La mision tiene dos puntos, el primero a la altura indicada y el segundo el land

  // Comienza el intercamibo de mensajes (se carga la mision)

  received=false;
  sendwaypointreq=0;
  seq=0;
  while((!received)and(sendwaypointreq<sendwaypointmax)){
    
    #ifdef ConsolaMon
    Consola.println();
    Consola.print("Enviando la cuenta de WP: ");
    Consola.print(NPuntos);
    Consola.print(" puntos por vez numero ");
    Consola.println(sendwaypointreq+1);
    #endif
        
    sendwaypointcount(NPuntos);
    #ifdef CoordenadasEnFloat
    waitfor(MAVLINK_MSG_ID_MISSION_REQUEST, MISSIONREQUESTTIMEOUT);
    #else
    waitfor(MAVLINK_MSG_ID_MISSION_REQUEST_INT, MISSIONREQUESTTIMEOUT); 
    #endif

    // Punto 0
    command=MAV_CMD_NAV_TAKEOFF; // CMD_ID = 22
    current=0; // Punto actual
    autocontinue  = 1; // True 
    param1=0; // Tiempo de hold  
    param2=0.5; // Radio de aceptacion en metros
    param3=0; // 0 para pasar por el WP
    param4=0; // YAW deseado
    #ifdef CoordenadasEnFloat
    #ifdef ConsolaMon
    Consola.println();
    Consola.print(F("Enviando punto (float): numero "));
    Consola.print(seq);
    Consola.print(F(" Latitud: "));
    Consola.print(float(latitud_home_int_1E7)*1e-6,7);
    Consola.print(F(", Longitud: "));
    Consola.print(float(longitud_home_int_1E7)*1e-6,7);
    Consola.print(F(", Altitud: "));
    Consola.print(altura);
    Consola.println();
    #endif
    sendwaypointFloat(seq,frame,command,current,autocontinue,param1,param2,param3,param4,float(latitud_home_int_1E7)*1e-6,float(longitud_home_int_1E7)*1e-6,altura);
    #else
    #ifdef ConsolaMon
    Consola.println();
    Consola.print(F("Enviando punto (int): numero "));
    Consola.print(seq);
    Consola.print(F(" Latitud: "));
    Consola.print(int32_t(latitud_home_int_1E7));
    Consola.print(F(", Longitud: "));
    Consola.print(int32_t(longitud_home_int_1E7));
    Consola.print(F(", Altitud: "));
    Consola.print(altura);
    Consola.println();
    #endif
    sendwaypointInt(seq,frame,command,current,autocontinue,param1,param2,param3,param4,latitud_home_int_1E7,longitud_home_int_1E7,altura);
    #endif
    sendwaypointreq++;
  }

  received=false;
  sendwaypointreq = 0;
  while((!received)and(sendwaypointreq<sendwaypointmax)){

    //espero el request
    #ifdef CoordenadasEnFloat
    waitfor(MAVLINK_MSG_ID_MISSION_REQUEST, MISSIONREQUESTTIMEOUT);
    #else
    waitfor(MAVLINK_MSG_ID_MISSION_REQUEST_INT, MISSIONREQUESTTIMEOUT); 
    #endif

    // Punto 1
    command=MAV_CMD_NAV_LAND; // CMD_ID = 21
    current=0; // Punto actual
    autocontinue  = 1; // True 
    param1=0;  
    param2=0; 
    param3=0; 
    param4=0; 
    #ifdef CoordenadasEnFloat
    /*#ifdef ConsolaMon
    Consola.println();
    Consola.print(F("Enviando punto (float): numero "));
    Consola.print(seq);
    Consola.print(F(" Latitud: "));
    Consola.print(float(latitud_home_int_1E7)*1e-6,7);
    Consola.print(F(", Longitud: "));
    Consola.print(float(longitud_home_int_1E7)*1e-6,7);
    Consola.print(F(", Altitud: "));
    Consola.print(altura);
    Consola.println();
    #endif*/
    sendwaypointFloat(seq,frame,command,current,autocontinue,param1,param2,param3,param4,float(latitud_home_int_1E7)*1e-6,float(longitud_home_int_1E7)*1e-6,0);
    #else
    /*#ifdef ConsolaMon
    Consola.println();
    Consola.print(F("Enviando punto (int): numero "));
    Consola.print(seq);
    Consola.print(F(" Latitud: "));
    Consola.print(int32_t(latitud_home_int_1E7));
    Consola.print(F(", Longitud: "));
    Consola.print(int32_t(longitud_home_int_1E7));
    Consola.print(F(", Altitud: "));
    Consola.print(altura);
    Consola.println();
    #endif*/
    sendwaypointInt(seq,frame,command,current,autocontinue,param1,param2,param3,param4,latitud_home_int_1E7,longitud_home_int_1E7,0);
    #endif
    sendwaypointreq++;
  }

  if(!received){
    #ifdef ConsolaMon
    Consola.println();
    Consola.println("No fue posible enviar mision porque no se recibio respuesta al WP count.");
    #endif    
    return false;
  }
  waitfor(MAVLINK_MSG_ID_MISSION_ACK, MISSIONACKTIMEOUT);
  if(!received){
    #ifdef ConsolaMon
    Consola.println();
    Consola.println("No se recibio el ACK del takeoff.");
    #endif    
    return false;
  }
  // Pide que se ponga en modo mision
  setMode(MAV_MODE_AUTO_ARMED);
  // Pide que arme 
  arm();
  return true;
   
}


void GoHome(){  //MAV_CMD_NAV_RETURN_TO_LAUNCH
  
}

void LandNow(){  //MAV_CMD_DO_LAND_START
  EnviarLandNow();
}

