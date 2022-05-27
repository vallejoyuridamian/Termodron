#ifdef RTCMdef
/*
void RTCMToPixhawk(byte data[]);
boolean checkheader(byte headerBytes[]);
int getMessageLength(byte messageLengthBytes[]);
void sendToBufferMSG(byte bufferMSG[],byte data[],int bytefrom,int byteto);
*/
void sendRTCM(byte bufferMSG[],int MSGLen);
void TCPOpen();
void TCPClose();
void RTCMReq();
void RTCMHTTPACT();

// Esta tarea se llama periodicamente desde el programa principal, se encarga de gestionar la recepcion de datos de RTCM

void RTCMCallBack(){

  switch(RTCMSTATUS) {
    
    case RTCM_INIT: {
      #ifdef GPSprueba 
      Consola.println("Iniciando RTCM");
      #endif
      TCPOpen();
      break;
    }
    case RTCM_OPENED: {
      #ifdef GPSprueba 
      Consola.println("Abriendo RTCM");
      #endif
      RTCMHTTPACT();
      break;      
    }

    case HTTP_OPENED: {
      RTCMReq(); // Se manda el GET HTTP 
      break;      
    }

    case RTCM_OK: {
      RTCM.disable();// No hace nada
      Consola.println(F("RTCM_OK deshabilito la tarea RTCM"));
      break;      
    }
  
    case RTCM_CLOSE: {
      TCPClose();
      break;
    }
    default:{
      Consola.println(F("Estado RTCM indefinido"));
      break;
    }
  }
}

void TCPOpen(){

  if (TCPOpenReqNum<TCPOpenReqMax){
    Shield3G.println(F("AT+NETOPEN=\"TCP\",2101"));
    //sendATcommand("AT+NETOPEN=\"TCP\",2101", "Network opened", 20000);
    TCPOpenReqNum++;
    TCPOpenCounter = millis();
    RTCM.disable();
  }
  else{
    Consola.println(F("Solicitud de apertura de sesion TCP no fue atendida"));
    RCTM_NA= true;
    RTCM_NACounter=millis();
  } 
}


void RTCMHTTPACT(){

  char url[ ]="201.217.132.178";
  int port= 2101; 
  char aux_str[100];
  sprintf(aux_str, "AT+CHTTPACT=\"%s\",%d", url, port);
  //sendATcommand(aux_str, "+CHTTPACT: REQUEST", 25000);
  
  if (HTTPACTReqNum<HTTPACTReqMax){
    Shield3G.println(aux_str);
    HTTPACTReqNum++;
    HTTPACTCounter = millis();
    RTCM.disable();
  }
  else{
    Consola.println(F("Solicitud de HTTPACT no fue atendida"));
    RCTM_NA= true;
    RTCM_NACounter=millis();
  } 
}

void RTCMReq(){

  char request[ ]="GET /UYMO HTTP/1.1\r\nUser-Agent: Termodron\r\nAccept: */*\r\nConnection: close\r\nAuthorization: Basic dGVybW9kcm9uOnRlcm1vZHJvbjE=\r\n\r\n"; //
  //termodron:termodron1 = dGVybW9kcm9uOnRlcm1vZHJvbjE=     (https://www.base64encode.org/  cambiar encode por decode para decodificar)

  if (RTCMReqNum<RTCMReqMax){
    Shield3G.write(request);
    Shield3G.write(0x1A);
    icy = true;
    RTCMReqNum++;
    RTCMReqCounter = millis();
    RTCM.disable();
  }
  else{
    Consola.println(F("Solicitud de RCTM no fue atendida"));
    RCTM_NA= true;
    RTCM_NACounter=millis();
  } 
}

void TCPClose(){

  if (TCPCloseReqNum<TCPCloseReqMax){
  Shield3G.println("AT+NETCLOSE");
  TCPCloseReqNum++;
  TCPCloseCounter= millis();
  RTCM.disable();    
  //sendATcommand("AT+NETCLOSE", "Network closed", 2000);
  }
  else{
    Consola.println(F("Solicitud de cierre de sesion TCP no fue atendida"));
    RCTM_NA= true;
    RTCM_NACounter=millis();
  }
}




/*

void readRTCM(String* Mens){
  
 
 Consola.println(*Mens);
 //Consola.println(Mens->substring(0,15));

  
}







void readRTCM2(int data_size){

  byte data[TCPPack];
  memset(data, '\0',TCPPack);
  char aux_str[100];
  counter=millis();
  memset(data, '\0',TCPPack);
  memset(aux_str,'\0',100);
  aux_char='\0';
  
  Consola.print(F("esto llego despues: "));
  while(Shield3G.available()!=0){
  Consola.print(Shield3G.read(),HEX); // limpia lo que haya pendiente
  }
  Consola.println();
  //sendATcommand("", "+CHTTPACT:", 10000);
  previous=millis();
  espera=millis();
  while((Shield3G.available()==0)&&(millis()-previous <TIMEOUTRTCM));// nos quedamos esperando datos del serial
  if(millis()-previous > TIMEOUTRTCM){
    Consola.println(F("No hay mas mensajes"));  
  }
  espera=millis()-espera;
  aux_str[0] = Shield3G.read(); // Se supone que es la D
  if (aux_str[0]!= 'D'){
  Consola.print(F("Waiting for the D, got: "));
  }
  previous=millis();
  while ((aux_str[0]!='D')&&(millis()-previous <TIMEOUTRTCM)){
    if(Shield3G.available()>0){
      aux_str[0]=Shield3G.read();
      Consola.print(aux_str[0]);
    }
  }
  if(millis()-previous > TIMEOUTRTCM){
    Consola.println(F("Aint got no D *sadface*"));  
  }

  previous=millis();
  while((Shield3G.available() ==0)&&(millis()-previous<TIMEOUTRTCM));
  if(Shield3G.available()>0){
    aux_char=Shield3G.read();
    Consola.print(aux_char);  // A
  }
  previous=millis();
  while((Shield3G.available() ==0)&&(millis()-previous<TIMEOUTRTCM));
  if(Shield3G.available()>0){
    aux_char=Shield3G.read();
    Consola.print(aux_char); // T
  }
  previous=millis();
  while((Shield3G.available() ==0)&&(millis()-previous<TIMEOUTRTCM));
  if(Shield3G.available()>0){
    aux_char=Shield3G.read();
    Consola.print(aux_char); // A
  }
  previous=millis();
  while((Shield3G.available() ==0)&&(millis()-previous<TIMEOUTRTCM));
  if(Shield3G.available()>0){
    aux_char=Shield3G.read(); //,
    Consola.print(aux_char);
  }
  Consola.println();
  // Reads the packet size
  x=0;
  do{
    previous=millis();
    while((Shield3G.available()==0)&&(millis()-previous <TIMEOUTRTCM));
    if(millis()-previous >TIMEOUTRTCM){
    Consola.println(F("Timeout leyendo el packet size"));
    }
    aux_str[x] = Shield3G.read();
    x++;
  }while((aux_str[x-1] != '\r') && (aux_str[x-1] != '\n'));

  previous=millis();
  while((Shield3G.available() ==0)&&(millis()-previous<TIMEOUTRTCM));
  if(Shield3G.available()>0){
    Shield3G.read();  // \n
  }
  
  aux_str[x-1] = '\0';
  data_size = atoi(aux_str);
  Consola.print(F("data_size: "));
  Consola.println(data_size);
  http_x = 0;

  Consola.print(F("Serial available principio mensaje: "));
  Consola.println(Shield3G.available());
  Consola.println();
  previous=millis();
  while((data_size > 0)&&(millis()-previous < TIMEOUTRTCM)){
    if(Shield3G.available() > 0){
            
      data[http_x] = Shield3G.read(); // aca esta el payload
      
      http_x++;
      data_size--;
    }
    else
      delay(1);
  }

  for (int k=0;k<http_x+1;k++){
    if(data[k]<16){
     Consola.print("0");
    } 
    Consola.print(data[k],HEX);
  }

    
  Consola.println();
  Consola.print(F("data_size fin loop: "));
  Consola.println(data_size);
  Consola.print(F("Serial available fin loop: "));
  Consola.println(Shield3G.available());
  
  RTCMToPixhawk(data);

  counter=millis()-counter;

  Consola.print(F("El loop duro: "));
  Consola.print(counter);
  Consola.println();
  Consola.print(F(" milisegundos, de los cuales "));
  Consola.print(espera);
  Consola.println(F(" estuvimos esperando un mensaje"));

}

void RTCMToPixhawk(byte data[]){
 
  int cantMes=1;
  int i=0;
  byte headerBytes[2]; // Los primeros dos bytes son 0xD3 y 0x0X con X = 0,1,2,3
  byte messageLengthBytes[2]; // El nibble menos significativo del segundo byte y el tercer byte hacen el largo del mensaje
  int messageLength=0;
  int bytefrom,byteto=0;
  int CRCLength=3; // 3 bytes de CRC24
  byte bufferMSG [200];
  previous=millis();
  //conseguimos el header

  
  while(millis()-previous <TIMEOUTRTCM){
    for(int k=0;k<2;k++){
      headerBytes[k]=data[i];
      i++; 
    } // al final i vale 2
    i--; // Se disminuye porque el segundo byte tiene info tanto para el header como para el largo del mensaje
    //conseguimos el mesageLength
    for(int k=0;k<2;k++){
      messageLengthBytes[k]=data[i];
      i++;
    }
    
    if(checkheader(headerBytes)){
      Consola.println();
      Consola.println(F("Header correcto, procesando mensaje"));
    }
    else{
     Consola.println();
     Consola.println(F("Fin del mensaje"));
     break;
    }
     
    messageLength=getMessageLength(messageLengthBytes);
    
    Consola.println(F("Largo del mensaje: "));
    Consola.println(messageLength);
    Consola.println(F(" bytes"));
  
    // en este punto i vale 3, o sea que estamos en el cuarto byte del mensaje y ya sabemos el largo
    // estamos listos para mandarlo a Pixhawk
    
    bytefrom= i - 3; // esto es para incluir el header y el largo // son indices, el primer byte es indice 0
    byteto=bytefrom + 2 + messageLength + CRCLength;
    
    sendToBufferMSG(bufferMSG,data,bytefrom,byteto);
    
    i=byteto+1;

    // En esta parte hay que mandarlo a la PX, por ahora los voy a imprimir nada mas
        
    Consola.print(F("Se ha enviado el mensaje numero "));
    Consola.print(cantMes);
    Consola.print(F(": "));

    cantMes++;
    
    for (int k=0;k<byteto-bytefrom+1;k++){
      if(bufferMSG[k]<16){
       Consola.print(F("0"));
    } 
    Consola.print(bufferMSG[k],HEX);
  }
  Consola.println();
  // fin de esta parte    

  Consola.println();
  Consola.print(F("Data size es: "));
  Consola.print(data_size_RTCM);
  Consola.print(F(" e i es: "));
  Consola.println(i);
      
  }  
  if(millis()-previous <TIMEOUTRTCM){

    Consola.println(F("Todos los mensajes han sido enviados"));
  
  }
  else{
  
    Consola.println(F("Se salio por timeout"));
  
  }
}

boolean checkheader(byte headerBytes[]){

 if((headerBytes[0]==0xD3)&&(headerBytes[1]<0x04)){
 
  return true; 

 }
 else{
 
   return false;
 
 }
}


int getMessageLength(byte messageLengthBytes[]){

 int messageLength=0;

 messageLength=messageLengthBytes[1]+messageLengthBytes[0]*256;

 return messageLength;
  
}

void sendToBufferMSG(byte bufferMSG[],byte data[],int bytefrom,int byteto){
  
  int MSGLen = byteto - bytefrom;

  for(int i =0; i<MSGLen;i++){
    bufferMSG[i]=data[bytefrom+i]; 
 
  }

  if(MSGLen < 110){
    sendRTCM(bufferMSG,MSGLen);
  }
}
*/
void sendRTCM(byte bufferMSG[],int MSGLen){

      #ifdef GPSprueba
     Consola.println(F("ENVIO RTCM"));
      #endif

   
   if(MSGLen<=180){
     mavlink_message_t msg;
     uint8_t buf[MAVLINK_MAX_PACKET_LEN];
     mavlink_msg_gps_rtcm_data_pack(system_id, component_id, &msg, flagRTCM, MSGLen ,&bufferMSG[0]);
     uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
     if (flagRTCM < 248){
      flagRTCM = flagRTCM + 8;
     }
     else{
      flagRTCM = 0;
     }
     PX4.write(buf, len);
   }
   else{
     #ifdef ConsolaMon
     Consola.print(F("No se envio el mensaje porque el tamanio es mayor que 180, es: "));
     Consola.println(MSGLen); 
     #endif

     #ifdef GPSprueba
     Consola.print(F("No se envio el mensaje porque el tamanio es mayor que 180, es: "));
     Consola.println(MSGLen); 
     #endif
   }

}
#endif
