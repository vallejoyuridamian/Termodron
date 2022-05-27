int calcrc(char *ptr, int count);
void ResetXmodem();
void Xmodem_Init();
void Xmodem_Del();
void Xmodem_Set_File();
void Xmodem_Trans();
void Xmodem_EOT();
void Xmodem_ETB();


void XmodemCallBack(){

  switch(XMODEMSTATUS) {
       
    case XMODEM_INIT: {  // 0
      #ifdef ConsolaMon
      Consola.println(F("Iniciando transferencia Xmodem"));
      #endif
      Xmodem_Init();
      break;
    }
    case XMODEM_DEL: { // 1
      Xmodem_Del();
      break;      
    }
    case XMODEM_SET_FILE: { // 2
      Xmodem_Set_File();
      break;      
    }
    case XMODEM_TRANS: { // 3
      Xmodem_Trans();
      break;      
    }
    case XMODEM_EOT: { // 4
      Xmodem_EOT();
      break;      
    }
    case XMODEM_ETB: { // 5
      Xmodem_ETB();
      break;
    }
    default:{
      #ifdef ConsolaMon
      Consola.println(F("Estado XMODEM indefinido"));
      #endif
    break;
    }
  }
}

void Xmodem_Init(){

  resetPOP3(false);
  sendEmail.disable();
  resetSMTP(false);
  checkEmail.disable();

  if (XMODEMINITReqNum<XMODEMINITReqMax){
    Shield3G.println("AT+FSCD=C:/Picture");
    XMODEMINITReqNum++;
    XMODEMINITCounter=millis();
    Xmodem.disable();
  }
  else{
    #ifdef ConsolaMon
    Consola.println(F("Error al realizar el CD"));
    #endif
    ResetXmodem();
    XMODEMINITCounter=millis();
  }
}


void Xmodem_Del(){

  if (XMODEMDELReqNum<XMODEMDELReqMax){
    memset(aux_str,'\0',aux_str_len);
    sprintf(aux_str,"AT+FSDEL=%s",FileName);    
    Shield3G.println(aux_str);
    XMODEMDELReqNum++;
    XMODEMDELCounter=millis();
    Xmodem.disable();
  }
  else{
    #ifdef ConsolaMon
    Consola.println(F("Error al realizar el Delete"));
    #endif
    ResetXmodem();
    XMODEMDELCounter=millis();
  }
    
}

void Xmodem_Set_File(){
  //sendATcommand("AT+CRXFILE=\"tecsto.txt\"", "OK", 2000);

  if (XMODEMSETFILEReqNum<XMODEMSETFILEReqMax){
    memset(aux_str,'\0',aux_str_len);
    sprintf(aux_str,"AT+CRXFILE=\"%s\"",FileName);    
    Shield3G.println(aux_str);
    XMODEMSETFILEReqNum++;
    XMODEMSETFILECounter=millis();
    Xmodem.disable();
  }
  else{
    #ifdef ConsolaMon
    Consola.println(F("Error al realizar el Set File"));
    #endif
    ResetXmodem();
    XMODEMSETFILECounter=millis();
  }
  
}



void Xmodem_Trans(){
  
  if (XMODEMTRANSReqNum<XMODEMTRANSReqMax){
    
   
    Shield3G.print(SOH);
    Shield3G.print(Xmodem_count);
    Shield3G.print(Xmodem_count_A1);
   
    
    for(int i = 0; i < XmodemDataSize;i++){
      Shield3G.print(XmodemData[i]);          
    }
    for(int i = 0; i < XmodemCRCSize;i++){
      Shield3G.print(XmodemCRC[i]);
    }
    
    XMODEMTRANSReqNum++;
    XMODEMTRANSCounter=millis();
    Xmodem.disable();
  }
  else{
    #ifdef ConsolaMon
    Consola.println(F("Error al enviar el archivo"));
    #endif
    ResetXmodem();
    XMODEMTRANSCounter=millis();
  }
  
}  
        
      
void Xmodem_EOT(){
  
  if (XMODEMEOTReqNum<XMODEMEOTReqMax){
    
    Shield3G.print(EOT);
    
    XMODEMEOTReqNum++;
    XMODEMEOTCounter=millis();
    Xmodem.disable();
  }
  else{
    #ifdef ConsolaMon
    Consola.println(F("Error al enviar el EOT"));
    #endif
    ResetXmodem();
    XMODEMEOTCounter=millis();
  }
   
}

void Xmodem_ETB(){

  if (XMODEMETBReqNum<XMODEMETBReqMax){
    
    Shield3G.print(ETB);
    XMODEMETBReqNum++;
    XMODEMETBCounter=millis();
    #ifdef ConsolaMon
    Consola.println(F("Fin transferencia Xmodem"));
    #endif
    XMODEMETBReqNum=0;
    XMODEMSTATUS=XMODEM_INIT;
    Xmodem.disable();
    XmodemReader.disable();
    Shield3GReader.enable();
    sendEmail.enable();
  }
  else{
    #ifdef ConsolaMon
    Consola.println(F("Error al enviar el ETB"));
    #endif
    ResetXmodem();
    XMODEMETBCounter=millis();
  }
    
}

void ResetXmodem(){
  XMODEMSTATUS = XMODEM_INIT;
  XMODEMINITReqNum = 0;
  XMODEMDELReqNum = 0;
  XMODEMSETFILEReqNum = 0;
  XMODEMTRANSReqNum = 0;
  XMODEMEOTReqNum = 0;
  XMODEMETBReqNum = 0;
  Xmodem.disable();
  XmodemReader.disable();
  Shield3GReader.enable();
}
