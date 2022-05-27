// Xmodem Reader
void RefreshXmodemData();
void CalculateCRC();
void XmodemReaderCallBack(){

  //Consola.println("Entre al XmodemReader");

  if(Shield3G.available()>0){
    chequearPin(pinShieldVivo); 
    ShieldByte = Shield3G.read();
    if (ShieldByte==C_XMOD){
      #ifdef ConsolaMon
      Consola.println(F("Inicio transferencia Xmodem"));
      #endif
      flg_Xmodem_enviando=true;
      CursorXmodem=0;
      Xmodem_count=0x01; //era 1
      Xmodem_count_A1=0xFE; //era 254
      RefreshXmodemData();
      CalculateCRC();
      XMODEMTRANSReqNum=0;
      Xmodem.enable(); 
      checkEmail.disable();
      sendEmail.disable();
    }
   
   
    if (ShieldByte==ACK){

  
      if((XMODEMSTATUS==XMODEM_ETB)and(XMODEMETBReqNum>0)){
        
        XMODEMSTATUS=XMODEM_INIT;
        #ifdef ConsolaMon
        Consola.println(F("Fin transferencia Xmodem"));
        #endif
        Shield3GReader.enable();
        checkEmail.enable();
        XmodemReader.disable();
      }
      
      
      if((XMODEMSTATUS==XMODEM_EOT)and(XMODEMEOTReqNum>0)){
        XMODEMSTATUS=XMODEM_ETB;
        XMODEMEOTReqNum=0;
        XMODEMETBReqNum=0;
        Xmodem.enable();
      }
      
      if((XMODEMSTATUS==XMODEM_TRANS)and(XMODEMTRANSReqNum>0)){
     
        if(Xmodem_count == 0xFF){
          Xmodem_count=0x00;
          Xmodem_count_A1=0xFF;
          
        } 
        else{
          Xmodem_count++;
          Xmodem_count_A1--;
          
        }

        if(flg_Xmodem_ultimodato){
          flg_Xmodem_ultimodato=false;
          XMODEMSTATUS = XMODEM_EOT;  
        }
        else{
         RefreshXmodemData();
         CalculateCRC();
         XMODEMTRANSReqNum=0;
        }
        Xmodem.enable();
      }
    }
   
    if (ShieldByte==NACK){
      
      if(XMODEMSTATUS==XMODEM_TRANS){
        XMODEMTRANSReqNum=0;
        Xmodem.enable();
      }
  
      if(XMODEMSTATUS==XMODEM_EOT){
        Xmodem.enable();
      }
  
      if(XMODEMSTATUS==XMODEM_ETB){
        Xmodem.enable();
      }
    }
  }
  
}

void RefreshXmodemData(){  
  for (int i = 0; i < XmodemDataSize; i++){
    if(CursorXmodem < ImagenBMPSize){
      XmodemData[i] = ImagenBMP[CursorXmodem];
      CursorXmodem++;
    }
    else{
      XmodemData[i] = PADDING_CHAR;
      flg_Xmodem_ultimodato=true;
    }
  }
  if (CursorXmodem == ImagenBMPSize){
    flg_Xmodem_ultimodato=true;
  }
  if(flg_Xmodem_ultimodato==true){
    CursorXmodem = 0;
  }
 
}


int calcrc(char *ptr, int count){
  int  crc;
  char i;
  crc = 0;
  while (--count >= 0){
    crc = crc ^ (int) *ptr++ << 8;
    i = 8;
    do{
      if (crc & 0x8000)
          crc = crc << 1 ^ 0x1021;
      else
          crc = crc << 1;
    }while(--i);
  }
  return (crc);
}



void CalculateCRC(){
  Xmodem_CRC = calcrc(XmodemData,XmodemDataSize);
  XmodemCRC[0] = char((Xmodem_CRC & 0x0000FF00)>>8);
  XmodemCRC[1] = char(Xmodem_CRC & 0x000000FF);
}
