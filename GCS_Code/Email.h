void intCoordToString(int32_t coord, char res[], int StrSize);
void checkEmailCallBack();
void sendEmailCallBack();
void POP3Set();
void SMTPSet();
bool POP3Login();
void TCPClose();
bool POP3Logout();
bool POP3Num();
bool POP3cd();
bool POP3Reading();
bool POP3Getting();
bool SMTPLogin();
bool SMTPAuth();
bool SMTPFrom();
bool SMTPRcpt();
bool SMTPSubject();
bool SMTPBody();
bool SMTPFile();
bool SMTPSend();
void resetSMTP(bool habilitar);
void resetPOP3(bool habilitar);
int CalcularTrayectoria(Punto* Perimetro,int counter);

void checkEmailCallBack(){

  
  switch(POP3STATUS) {
       
    case POP3_INIT: {  // 0
      Consola.println(F("Iniciando sesion POP3"));
      POP3Login();
      break;
    }
    case POP3_OPENED: { // 1
      POP3Num();
      break;      
    }
    case POP3_GETTING: { // 2
      POP3Getting();
      break;      
    }
    case POP3_CD: { // 3
      POP3cd();
      break;      
    }
    case POP3_READING: { // 4
      POP3Reading();
      break;      
    }
    case POP3_CLOSE: { // 5
      POP3Logout();
      break;
    }
    case POP3_STBY: { // 6
      POP3STATUS = POP3_INIT;
      break;
    }
    default:{
      Consola.println(F("Estado POP3 indefinido"));
      break;
    }
  }
}

void POP3Set(){
  
  sprintf(data, "AT+POP3SRV=\"%s\",\"%s\",\"%s\",%s", pop3_server, pop3_user_name, mail_password, pop3_port);
  sendATcommand(data, "OK", 2000);
  
}  

bool POP3Login(){
  
  if (POP3INReqNum<POP3INReqMax){
    Shield3G.println("AT+POP3IN");
    POP3INReqNum++;
    POP3INCounter=millis();
    checkEmail.disable();
  }
  else{
    Consola.println(F("No se recibe respuesta por parte del servidor POP3"));
    POP3INCounter=millis();
    resetPOP3(true);
  }
}

bool POP3Num(){
  if (POP3NUMReqNum<POP3NUMReqMax){
    Shield3G.println("AT+POP3NUM");
    POP3NUMReqNum++;
    POP3NUMCounter=millis();
    checkEmail.disable();
  }
  else{
    Consola.println(F("No se reciben la cantidad de correos"));
    POP3NUMCounter=millis();
    resetPOP3(true);
  }
}

bool POP3Getting(){
  if (POP3GETReqNum<POP3GETReqMax){
    if (totalemailsint != lasttotalemailsint){
      sprintf(aux_str, "AT+POP3GET= %s", totalemails);
      Shield3G.println(aux_str);
      POP3GETReqNum++;
      POP3GETCounter=millis();
      checkEmail.disable();
      lasttotalemailsint = totalemailsint;
    }
    else{
      Consola.println("No hay correos nuevos en Inbox");
      POP3STATUS = POP3_CLOSE;
      checkEmail.disable();
    }
  }
  else{
    Consola.println(F("Error al realizar el GET"));
    POP3GETCounter=millis();
    resetPOP3(true);
  }
}

bool POP3cd(){
  if (POP3CDReqNum<POP3CDReqMax){
    Shield3G.println("AT+FSCD=C:/Email");
    POP3CDReqNum++;
    POP3CDCounter=millis();
    checkEmail.disable();
  }
  else{
    Consola.println(F("Error al realizar el CD"));
    POP3CDCounter=millis();
    resetPOP3(true);
  }
}

bool POP3Reading(){ //Acá la idea es que lea el correo correspondiente a la respuesta de POP3NUM
  
  if (POP3READReqNum<POP3READReqMax){
      sprintf(aux_str, "AT+POP3READ=0,\"%s\"", path);
      Shield3G.println(aux_str);
      //Consola.println(aux_str);
      POP3READCounter=millis();
      SubjectFlag = 0;
      POP3READReqNum++;
      checkEmail.disable();
  }
  else{
    Consola.println(F("Error al realizar el READ"));
    POP3READCounter=millis();
    resetPOP3(true);
  }
}

bool POP3Logout(){
  
  if (POP3OUTReqNum<POP3OUTReqMax){
    Shield3G.println("AT+POP3OUT");
    POP3OUTReqNum++;
    POP3OUTCounter=millis();
    checkEmail.disable();
    enviarReportes.enable();
  }
  else{
    Consola.println(F("Error al cerrar sesion POP3"));
    POP3OUTCounter=millis();
    resetPOP3(true);
  }
}


void sendEmailCallBack(){

  long contador;
   
  contador = millis();
  switch(SMTPSTATUS) {
    
    case SMTP_INIT: {  // 0
      Consola.println(F("Iniciando sesion SMTP"));
      SMTPLogin();
      break;
    }
    case SMTP_AUTH: { // 1
      SMTPAuth();
      break;      
    }
    case SMTP_FROM: { // 2
      SMTPFrom();
      break;      
    }
    case SMTP_RCPT: { // 3
      SMTPRcpt();
      break;      
    }
    case SMTP_SUB: { // 4
      SMTPSubject();
      break;      
    }
    case SMTP_BODY: { // 5
      SMTPBody();
      break;      
    }
    case SMTP_FILE: { // 8
      SMTPFile();
      break;
    }
    case SMTP_SEND: { // 6
      SMTPSend();
      break;
    }
    case SMTP_STBY: { // 7
      SMTPSTATUS = SMTP_INIT;
      sendEmail.disable();
      checkEmail.enable();
      break;
    }
    default:{
      Consola.println(F("Estado SMTP indefinido"));
      break;
    }
  }
}

bool SMTPLogin(){

  if (SMTPINReqNum<SMTPINReqMax){
    sprintf(aux_str, "AT+SMTPSRV=\"%s\",%s", smtp_server, smtp_port);
    Shield3G.println(aux_str);
    SMTPINReqNum++;
    SMTPINCounter=millis();
    sendEmail.disable();
  }
  else{
    Consola.println(F("No se recibe respuesta por parte del servidor SMTP"));
    SMTPINCounter=millis();
    checkEmail.enable();
  }
}

bool SMTPAuth(){
  if (SMTPAUTHReqNum<SMTPAUTHReqMax){
    sprintf(aux_str, "AT+SMTPAUTH=1,\"%s\",\"%s\"", smtp_user_name, mail_password);
    Shield3G.println(aux_str);
    SMTPAUTHReqNum++;
    SMTPAUTHCounter=millis();
    sendEmail.disable();
  }
  else{
    Consola.println(F("Error de Autenticacion"));
    SMTPAUTHCounter=millis();
    resetSMTP(true);
  }
}

bool SMTPFrom(){
  if (SMTPFROMReqNum<SMTPFROMReqMax){
    sprintf(aux_str, "AT+SMTPFROM=\"%s\",\"%s\"", smtp_user_name, smtp_user_name);
    Shield3G.println(aux_str);
    SMTPFROMReqNum++;
    SMTPFROMCounter=millis();
    sendEmail.disable();
  }
  else{
    Consola.println(F("Error al setear el From"));
    SMTPFROMCounter=millis();
    resetSMTP(true);
  }
}

bool SMTPRcpt(){
  if (SMTPRCPTReqNum<SMTPRCPTReqMax){
    sprintf(aux_str, "AT+SMTPRCPT=0,0,\"%s\",\"%s\"", to_address, to_address);
    Shield3G.println(aux_str);
    SMTPRCPTReqNum++;
    SMTPRCPTCounter=millis();
    sendEmail.disable();
  }
  else{
    Consola.println(F("Error al setear el Rcpt"));
    SMTPRCPTCounter=millis();
    resetSMTP(true);
  }
}

bool SMTPSubject(){
  
  if (SMTPSUBReqNum<SMTPSUBReqMax){
      memset(aux_str,'\0',aux_str_len);
      sprintf(aux_str, "AT+SMTPSUB=\"%s\"", SubjectSend);
      Shield3G.println(aux_str);
      SMTPSUBCounter=millis();
      SMTPSUBReqNum++;
      sendEmail.disable();
  }
  else{
    Consola.println(F("Error al escribir el Subject"));
    SMTPSUBCounter=millis();
    resetSMTP(true);
  }
}

bool SMTPFile(){

  if (SMTPFILEReqNum<SMTPFILEReqMax){
      memset(aux_str,'\0',aux_str_len);
      sprintf(aux_str, "AT+SMTPFILE=1,\"%s\"", FileName);  ///Subject  //Nacho
      Shield3G.println(aux_str);
      SMTPFILECounter=millis();
      SMTPFILEReqNum++;
      sendEmail.disable();
  }
  else{
    Consola.println(F("Error al adjuntar archivo"));
    SMTPFILECounter=millis();
    resetSMTP(true);
  }
}
  
 

bool SMTPBody(){

  //BodySend[] = "ENVIANDO E-MAIL";
  
  if (SMTPBODYReqNum<SMTPBODYReqMax){
    //Consola.println(BodySend);
    sendATcommand("AT+SMTPBODY", ">>", 500);
    //sprintf(BodySend, "AT+SMTPBODY=\"%s\"", BodySend); //"ENVIANDO E-MAIL"  //Nacho
    Shield3G.println(BodySend);
    Shield3G.write(0x1A);
    //Consola.println(BodySend);
    SMTPBODYCounter = millis();
    SMTPBODYReqNum++;
    sendEmail.disable();
  }
  else{
    Consola.println(F("Error al escribir el Body"));
    SMTPBODYCounter=millis();
    resetSMTP(true);
  }
}

bool SMTPSend(){
  
  if (SMTPSENDReqNum<SMTPSENDReqMax){
    Shield3G.println("AT+SMTPSEND");
    SMTPSENDReqNum++;
    SMTPSENDCounter = millis();
    sendEmail.disable();
  }
  else{
    Consola.println(F("Error al envair e-mail"));
    SMTPSENDCounter=millis();
    resetSMTP(true);
  }
}




void enviarReportesCallBack(){

  memset(aux_str,'\0',aux_str_len);
  memset(SubjectSend,'\0', SubjectSize);
  memset(BodySend,'\0', BodySize);
  // SUBJECT //
  //sprintf(SubjectSend, "REPORTE DE ESTADO DEL DRON"); IDM - DESCOMENTAR
  sprintf(SubjectSend, "TERMODRON - Ingenieria deMuestra"); // IDM - COMENTAR
  //sprintf(BodySend, "INICIO REPORTE\r\n"); IDM - DESCOMENTAR

  boolean flg_sendEmail=false;
  flg_has_att=false;

  if(flg_LogMail){
    // LATITUD
    intCoordToString(latitud_int_1E7,aux_str,sizeof(aux_str)/sizeof(aux_str[0]));
    sprintf(BodySend,"%s %s %s %s",BodySend, "LATITUD = ",aux_str,"°\r\n");
    memset(aux_str,'\0',aux_str_len);
    // LONGITUD  
    intCoordToString(longitud_int_1E7,aux_str,sizeof(aux_str)/sizeof(aux_str[0]));
    sprintf(BodySend,"%s %s %s %s",BodySend, "LONGITUD = ",aux_str,"°\r\n");
    memset(aux_str,'\0',aux_str_len);
    // ALTITUD
    sprintf(BodySend,"%s %s %d %s",BodySend, "ALTITUD = ",altitud_int," m\r\n");
    // BATERIA
    sprintf(BodySend,"%s %s %d %s",BodySend, "BATERIA = ",carga_bateria,"%\r\n");
    // YAW
    sprintf(BodySend,"%s %s %f %s",BodySend, "YAW = ",yaw,"°\r\n");
    // PITCH
    sprintf(BodySend,"%s %s %f %s",BodySend, "PITCH = ",pitch,"°\r\n");
    // ROLL
    sprintf(BodySend,"%s %s %f %s",BodySend, "ROLL = ",roll,"°\r\n");
    // TIEMPO DESDE BOOTEO
    sprintf(BodySend,"%s %s %d %s",BodySend, "TIEMPO DESDE BOOTEO = ",Timestamp_boot_PX,"ms \r\n");
    flg_sendEmail=true;
  }

  if((flg_Img)and(flg_Img_available)){

    char aux_str_lat[aux_str_len];
    char aux_str_lon[aux_str_len];

    memset(aux_str_lat,'\0',aux_str_len);
    memset(aux_str_lon,'\0',aux_str_len);

    
    intCoordToString(latitud_int_1E7_foto,aux_str_lat,sizeof(aux_str_lat)/sizeof(aux_str[0]));
    intCoordToString(longitud_int_1E7_foto,aux_str_lon,sizeof(aux_str_lon)/sizeof(aux_str[0]));

    //sprintf(BodySend,"%s %s",BodySend, "IMAGEN ADJUNTA: \r\n"); IDM - DESCOMENTAR
    sprintf(BodySend,"%s %s",BodySend, "Tu posición actual es: \r\n"); // IDM - COMENTAR
    // LATITUD  
    //sprintf(BodySend,"%s %s %s %s",BodySend, "LATITUD = ",aux_str_lat,"°\r\n"); IDM - DESCOMENTAR
    sprintf(BodySend,"%s %s %s %s",BodySend, "LATITUD = ","-34.9170872","°\r\n"); //IDM - COMENTAR
    // LONGITUD  
    intCoordToString(longitud_int_1E7,aux_str,sizeof(aux_str)/sizeof(aux_str[0]));
    //sprintf(BodySend,"%s %s %s %s",BodySend, "LONGITUD = ",aux_str_lon,"°\r\n"); IDM - DESCOMENTAR
    sprintf(BodySend,"%s %s %s %s",BodySend, "LONGITUD = ","-56.1668042","°\r\n"); //IDM - COMENTAR
    //LINK GOOGLEMAPS
    //sprintf(BodySend,"%s%s%s%s%s%s%s%s%s%s",BodySend, "LINK EN GOOGLE MAPS= https://www.google.com/maps/place//@",aux_str_lat,",",aux_str_lon,
    //",80m/data=!3m1!1e3!4m5!3m4!1s0x0:0x0!8m2!3d",aux_str_lat,"!4d",aux_str_lon,"?hl=es-419\r\n"); IDM - DESCOMENTAR y arreglar para que quede cortito (seh...)

    sprintf(BodySend,"%s%s%s%s%s%s",BodySend, "LINK EN GOOGLE MAPS= https://www.google.com/maps/search/?api=1&query=","-34.9170872",",","-56.1668042","\r\n"); //IDM - COMENTAR
    
    sprintf(BodySend,"%s %s",BodySend, "¡Gracias por visitar nuestro stand, no te olvides de votar por TERMODRON!\r\n"); // IDM - COMENTAR
    memset(aux_str_lat,'\0',aux_str_len);
    memset(aux_str_lon,'\0',aux_str_len);
    
    flg_has_att=true;
    Xmodem.enable();
    PrepararImagen();
    RAWThermalToCelsius();
    CargarImagenABMP();
    flg_sendEmail=true;
    flg_Img_available=false;
    flg_Img=false;
  }

  if((flg_Alertas)&&(sendAlerta)){

    sendAlerta=false;
    memset(aux_str,'\0',aux_str_len);
    memset(SubjectSend,'\0', SubjectSize);
    memset(BodySend,'\0', BodySize);

    sprintf(SubjectSend, "ALERTA TERMODRON");
      
    if((PIXHAWK_STATE==PIXHAWK_MISION)&&(!(inicio_mision_alarm))){
      
      sprintf(BodySend, "INICIO DE MISION\r\n");
      
      // LATITUD
      intCoordToString(latitud_int_1E7,aux_str,sizeof(aux_str)/sizeof(aux_str[0]));
      sprintf(BodySend,"%s %s %s %s",BodySend, "LATITUD = ",aux_str," °\r\n");
      memset(aux_str,'\0',aux_str_len);
    
      // LONGITUD  
      intCoordToString(longitud_int_1E7,aux_str,sizeof(aux_str)/sizeof(aux_str[0]));
      sprintf(BodySend,"%s %s %s %s",BodySend, "LONGITUD = ",aux_str," °\r\n");
      memset(aux_str,'\0',aux_str_len);
          
      // BATERIA
      sprintf(BodySend,"%s %s %d %s",BodySend, "BATERIA = ",carga_bateria," %\r\n");
      memset(aux_str,'\0',aux_str_len);
          
      // TIEMPO DESDE BOOTEO
      sprintf(BodySend,"%s %s %d %s",BodySend, "TIEMPO DESDE BOOTEO = ",Timestamp_boot_PX," s");
      memset(aux_str,'\0',aux_str_len);
      flg_sendEmail=true;
     
    } 
  }
  
  if(flg_ReporteFinal){

    sendReporteFinal=false;
    memset(aux_str,'\0',aux_str_len);
    memset(SubjectSend,'\0', SubjectSize);
    memset(BodySend,'\0', BodySize);

    sprintf(SubjectSend, "REPORTE FIN DE MISION");
      
    if((PIXHAWK_STATE==PIXHAWK_MISION)&&(!(inicio_mision_alarm))){
      
      sprintf(BodySend, "REPORTE DE FIN DE MISION\r\n");
      
      // LATITUD
      intCoordToString(latitud_int_1E7,aux_str,sizeof(aux_str)/sizeof(aux_str[0]));
      sprintf(BodySend,"%s %s %s %s",BodySend, "LATITUD = ",aux_str," °\r\n");
      memset(aux_str,'\0',aux_str_len);
    
      // LONGITUD  
      intCoordToString(longitud_int_1E7,aux_str,sizeof(aux_str)/sizeof(aux_str[0]));
      sprintf(BodySend,"%s %s %s %s",BodySend, "LONGITUD = ",aux_str," °\r\n");
      memset(aux_str,'\0',aux_str_len);
          
      // BATERIA
      sprintf(BodySend,"%s %s %d %s",BodySend, "BATERIA = ",carga_bateria," %\r\n");
      memset(aux_str,'\0',aux_str_len);
          
      // TIEMPO DESDE BOOTEO
      sprintf(BodySend,"%s %s %d %s",BodySend, "TIEMPO DESDE BOOTEO = ",Timestamp_boot_PX," s");
      memset(aux_str,'\0',aux_str_len);
      
    }
    flg_sendEmail=true; 
  }

  if(flg_sendEmail){
    #ifdef ConsolaMon
    Consola.println("HAY QUE MANDAR MAIL");
    #endif
    if(!flg_has_att){
      sendEmail.enable();
    }
    else{
      #ifdef ConsolaMon
      Consola.println("PASANDO A MODO XMODEM");
      #endif      
    }  
  }
  else{
    #ifdef ConsolaMon
    Consola.println("NO HAY QUE MANDAR MAIL");
    #endif
    checkEmail.enable();
  }
  enviarReportes.disable();
  

}

//**************************
//Función cálculo de trayectoria

int CalcularTrayectoria(Punto* Perimetro,int counter){
  
  Punto Punto1esN,PuntoAux,PuntoAux2;
  Pendiente Pend;
  float distEnPer=0;
  float debo = 0;
  int i = 0;
  int j=0;
  int z = 0;
  int N = counter;
  int k =counter;
  boolean unoEsN = true;

  distEntrePer = Perimetro[0].altitud*ALTITUDADISTANCIAENTREPER;
  // Los primeros dos puntos los cargo en la mision ya que ira por el perimetro
  for (int i = 0; i < 2;i++){
    Mision[i].latitud= Perimetro[i].latitud;
    Mision[i].longitud= Perimetro[i].longitud;
    Mision[i].altitud= Perimetro[i].altitud;
  }
  i = 0;
  k = N;
  j = 2; 
  PuntoAux2= Punto(Perimetro[1].latitud,Perimetro[1].longitud,Perimetro[0].altitud);
  Pend= Pendiente(Perimetro[0],Perimetro[1]);
  Punto1esN= Punto(Perimetro[0].latitud,Perimetro[0].longitud,Perimetro[0].altitud);

  while (i<k-2){
    //Recorre el perimetro del lado de las i (indice creciente de los puntos)
    distEnPer=recalcularDist(distEntrePer,Pend,PuntoAux2,Perimetro[i+2]);
    PuntoAux= Punto(PuntoAux2,Perimetro[i+2],distEnPer);
    if(PuntoAux.menosQueD){
      i++;
      //Serial.print("Aumento i recorriendo el perimetro por el lado de las i ");
      //Serial.println(i);
      if(i>=k-2){
      //Serial.println("Termino");
        Mision[j]=Punto(PuntoAux.latitud,PuntoAux.longitud,Perimetro[0].altitud);
        j++;
        break;
      }
      Mision[j]=Punto(PuntoAux.latitud,PuntoAux.longitud,Perimetro[0].altitud);
      j++;
      if (j == MISIONMAXPUNTOS -1){
        break;  
      }
      debo =desrecalcularDist(PuntoAux.debo, Pend, PuntoAux, Perimetro[i]);
      debo =recalcularDist(debo, Pend, PuntoAux, Perimetro[i+2]);
      PuntoAux2= Punto(PuntoAux.latitud, PuntoAux.longitud,Perimetro[0].altitud);
      PuntoAux= Punto(PuntoAux,Perimetro[i+2],debo);

      if(PuntoAux.menosQueD){
        i++;
        //Serial.print("Aumento i recorriendo el perimetro por el lado de las i ");
        //Serial.println(i);
        if(i>=k-2){
        //Serial.println("Termino");
          Mision[j]=Punto(PuntoAux.latitud,PuntoAux.longitud,Perimetro[0].altitud);
          j++;
          break;
        }
        Mision[j]=Punto(PuntoAux.latitud,PuntoAux.longitud,Perimetro[0].altitud);
        j++;
        if (j == MISIONMAXPUNTOS -1){
          break;  
        }
      }
    }
    Mision[j]=Punto(PuntoAux.latitud,PuntoAux.longitud,Perimetro[0].altitud);
    j++;
    if (j == MISIONMAXPUNTOS -1){
        break;  
    }
    //  Cruza al lado de los k (indice decreciente de los puntos)
    //Serial.println("Cruza al lado de los k (indice decreciente de los puntos)");
    
    if(unoEsN){
      PuntoAux2= Punto(PuntoAux, Pend, Punto1esN, Perimetro[k-1]);
    }
    else{
      PuntoAux2= Punto(PuntoAux, Pend, Perimetro[k], Perimetro[k-1]);
    }
    if(PuntoAux2.menosQueD){
      k--;
      //Serial.print(F("Disminuyo k cruzando al lado de las k"));
      //Serial.println(k);
      unoEsN=false;
      Pend= Pendiente(PuntoAux,PuntoAux2);
      if(i>=k-2){
        Mision[j]=Punto(PuntoAux2.latitud,PuntoAux2.longitud,Perimetro[0].altitud);
        j++;
        break;
      }
    }
    Mision[j]=Punto(PuntoAux2.latitud,PuntoAux2.longitud,Perimetro[0].altitud);
    j++;
    if (j == MISIONMAXPUNTOS -1){
        break;  
    }
    // Recorre el perimetro del lado de las k
    //Serial.println(F("Recorre el perimetro del lado de las k"));
    distEnPer=recalcularDist(distEntrePer,Pend,PuntoAux2,Perimetro[k-1]);  
    PuntoAux = Punto(PuntoAux2,Perimetro[k-1],distEnPer);
    if(PuntoAux.menosQueD){
      k--;
      //Serial.print(F("Disminuyo k recorriendo el perimetro por el lado de las k "));
      //Serial.println(k);
      unoEsN=false;
      if(i>=k-2){
        Mision[j]=Punto(PuntoAux.latitud,PuntoAux.longitud,Perimetro[0].altitud);
        j++;
      }
      Mision[j]=Punto(PuntoAux.latitud,PuntoAux.longitud,Perimetro[0].altitud);
      j++;
      if (j == MISIONMAXPUNTOS -1){
        break;  
      }
      debo =desrecalcularDist(PuntoAux.debo, Pend, PuntoAux2, Perimetro[k]);
      debo =recalcularDist(debo, Pend, PuntoAux, Perimetro[k-1]);
      PuntoAux2= Punto(PuntoAux.latitud, PuntoAux.longitud,Perimetro[0].altitud);
      PuntoAux= Punto(PuntoAux,Perimetro[k-1],debo);

      if(PuntoAux.menosQueD){
        k--;
        //Serial.print(F("Disminuyo k recorriendo el perimetro por el lado de las k "));
        //Serial.println(k);
        unoEsN=false;
        if(i>=k-2){
          Mision[j]=Punto(PuntoAux.latitud,PuntoAux.longitud,Perimetro[0].altitud);
          j++;
          break;
        }
        Mision[j]=Punto(PuntoAux.latitud,PuntoAux.longitud,Perimetro[0].altitud);
        j++;
        if (j == MISIONMAXPUNTOS -1){
          break;  
        }
      }
    }
    Mision[j]=Punto(PuntoAux.latitud,PuntoAux.longitud,Perimetro[0].altitud);
    j++;
    if (j == MISIONMAXPUNTOS -1){
        break;  
    }
    // Cruza al lado de las i  
    //Serial.println(F("Cruza al lado de las i"));
    PuntoAux2= Punto(PuntoAux,Pend,Perimetro[i+1],Perimetro[i+2]);
    if(PuntoAux2.menosQueD){
      i++;
      //Serial.print(F("Aumento i cruzando al lado de las i"));
      //Serial.println(i);
      Pend= Pendiente(PuntoAux,PuntoAux2);
      if(i>=k-2){
        Mision[j]=Punto(PuntoAux2.latitud,PuntoAux2.longitud,Perimetro[0].altitud);
        j++;
        break;
      }     
    }
    Mision[j]=Punto(PuntoAux2.latitud,PuntoAux2.longitud,Perimetro[0].altitud);
    j++;
    if (j == MISIONMAXPUNTOS -1){
        break;  
    }

    z=z+1;
    if (z > 799){
      break;
    }
  }
  
  #ifdef ConsolaMon
  Consola.print(F("Al final z vale "));
  Consola.println(z);
  Consola.println(F("Si z es 800 es que quedo en un loop o algo raro"));
  Consola.print(F("i vale "));
  Consola.print(i);
  Consola.print(F(" k vale "));
  Consola.println(k);    
  Consola.print(F("El perimetro tiene "));
  Consola.print(N);
  Consola.println(F(" puntos de coordenadas: "));
  for(int i=0;i<N;i++){
    Consola.print(int32_t(Perimetro[i].latitud));
    Consola.print(",");
    Consola.println(int32_t(Perimetro[i].longitud));
  }
  Consola.print(int32_t(Perimetro[0].latitud));
  Consola.print(",");
  Consola.println(int32_t(Perimetro[0].longitud));
  Consola.print(F("La Mision tiene "));
  Consola.print(j);
  Consola.println(F(" puntos de coordenadas: "));
  for(int i=0;i<j;i++){
    Consola.print(int32_t(Mision[i].latitud));
    Consola.print(",");
    Consola.println(int32_t(Mision[i].longitud));
  }
  #endif
  return j;
}



//*****************************************************************************************
//Definición de la función borradoEMail

void borradoEMail(){
  int8_t answer_fin;
  int j;
  unsigned long previous;
  char comando[128];

  do{
  
    sendATcommand("AT+FSCD=C:/Email/", "OK", 2000);
    
    while(Shield3G.available() > 0) Shield3G.read();   
  
    Shield3G.println("AT+FSLS");
    previous = millis();
    answer = 0;
      
      // this loop reads the first folder
    j = 0;
    do{
      if(Shield3G.available() != 0){    
        aux_str[j] = Shield3G.read();
        Consola.print(aux_str[j]);
        j++;
        if (strstr(aux_str, "ES: ") != NULL){
          answer = 1;
        }
      }
    } while ((answer == 0) && ((millis() - previous) < 10000));
    previous = millis();
    answer = 0;
    j = 0;
    memset (aux_str, '\0',(sizeof(aux_str)/sizeof(aux_str[0])));
    do{
      if(Shield3G.available() != 0){    
        aux_str[j] = Shield3G.read();
        if ((aux_str[j] == '\n') && (j > 2)){
          aux_str[j] = '\0';
          answer = 1;
          Consola.println(aux_str);
        }
        if ((aux_str[j] == '\n') && (j <= 2)){
          j=-1;
        }
        if ((aux_str[j] != ' ') && (answer == 0)){
          j++;  
        }
      }
    } while ((answer == 0) && ((millis() - previous) < 5000));
  
    sprintf(comando,"AT+FSCD=C:/Email/%s", aux_str);
    sendATcommand(comando, "OK", 2000);
    sendATcommand("AT+FSDEL=*.*", "OK", 2000);
    sendATcommand("AT+FSCD=C:/Email/", "OK", 2000);
    sprintf(comando,"AT+FSRMDIR=%s", aux_str);
    answer_fin=sendATcommand(comando, "OK", 2000);
  } while(answer_fin == 1);
  Consola.println (F("Fin de borrado de Carpetas"));
}


void intCoordToString(int32_t coord, char res[], int StrSize){
  
  int i=0;
  int kDigito=0;
  int32_t inti=0;
  int32_t coordaux;
  memset(res,'\0',StrSize);
  if (coord < 0){
    res[0] = '-';
    i++;
    coord=coord*-1;
  }
  if (coord <1E8){
    kDigito = 7;
  }
  else{
    kDigito = 8;
  }
  coordaux=coord;
  while(kDigito>=0){
    inti= (int)(coord*pow(10,-kDigito));
    sprintf(res,"%s%d",res,inti);
    
    while((inti < 0.1*coord)and(inti != 0)){
      inti=inti*10;
    }
    coord = coord -inti;
    
    i++;
    kDigito--;
    if(kDigito == 6){
      res[i]='.';
      i++;
    }
  }
  while(i<StrSize){
    res[i]='\0';
    i++;
  }
}





void resetSMTP(bool habilitar){
 
  SMTPINCounter = 0;
  SMTPAUTHCounter = 0;
  SMTPFROMCounter = 0;
  SMTPRCPTCounter = 0;
  SMTPSUBCounter = 0;
  SMTPBODYCounter = 0;
  SMTPSENDCounter = 0;
  SMTPINReqNum = 0;
  SMTPAUTHReqNum = 0;
  SMTPFROMReqNum = 0;
  SMTPRCPTReqNum = 0;
  SMTPSUBReqNum = 0;
  SMTPBODYReqNum = 0;
  SMTPSENDReqNum = 0;
  SMTPSTATUS = SMTP_INIT;
  if(habilitar){
    sendEmail.enable();
  }
  SMTPtry++;
}

void resetPOP3(bool habilitar){
  
  POP3STATUS = POP3_INIT;
  POP3INReqNum=0;
  POP3INCounter=0;
  POP3NUMReqNum=0;
  POP3NUMCounter=0;
  POP3GETReqNum=0;
  POP3GETCounter=0;
  POP3CDReqNum=0;
  POP3CDCounter=0;
  POP3READReqNum=0;
  POP3READCounter=0;
  POP3OUTReqNum=0;
  POP3OUTCounter=0;
  if(habilitar){
   checkEmail.enable();
  }
  POP3try++;
  
}
