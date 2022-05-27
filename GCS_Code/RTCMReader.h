#ifdef RTCMdef
int getRTCMLength();
void printRTCM();
boolean checkheader();

void RTCMReaderCallBack(){
 
  while((Shield3G.available()>0)and(data_size_RTCM>0)){

    chequearPin(pinShieldVivo);
    //Consola.println(Shield3G.available());
    
    MensajeRTCM[iMENSRTCM] = Shield3G.read();
    iMENSRTCM++;
    data_size_RTCM--;

    if(iMENSRTCM==2){
      if(checkheader()){
        Consola.println(F("Header correcto"));

      }
      else{
        Consola.println(F("Header incorrecto"));  // TODO Resetear el RTCM o algo asi 
        Consola.println(Shield3G.available());
        RTCMReader.disable();
        Shield3GReader.enable();
        //RTCMReqNum = 0;     

      }  
    }

    if(iMENSRTCM==3){
      RTCMLength=getRTCMLength();
      Consola.print(F("El largo del mensaje es: "));
      Consola.println(RTCMLength+6);
    }

    if(iMENSRTCM >= RTCMLength+6){ // 3 de header y 3 de CRC
      sendRTCM(MensajeRTCM,iMENSRTCM);
      
      Consola.println("Mensaje enviado");
      /*for(int i=0; i<iMENSRTCM;i++){
        if(MensajeRTCM[i] < 16){
          Consola.print(F("0"));
        }
          Consola.print(MensajeRTCM[i],HEX);
      }
      Consola.println();*/
      memset(MensajeRTCM,'\0',MAXMENSLENRTCM);
      iMENSRTCM=0;
      RTCMLength=0;
    }
  }

  if(data_size_RTCM==0){
    
    if(iMENSRTCM >= RTCMLength+6){
      iMENSRTCM=0;
      RTCMLength=0;
      memset(MensajeRTCM,'\0',MAXMENSLENRTCM);
    }
    
    RTCMReader.disable();
    Shield3GReader.enable();  
  }
}


boolean checkheader(){
 if((MensajeRTCM[0]==0xD3)&&(MensajeRTCM[1]<0x04)){
  return true; 
 }
 else{
   return false;
 }
}

int getRTCMLength(){
  int messageLength = 0;
  messageLength=MensajeRTCM[2]+MensajeRTCM[1]*256;
  return messageLength;
}


void printRTCM(){
  Consola.println("Mensaje: ");
  for(int i=0; i<iMENSRTCM;i++){
    if(MensajeRTCM[i] < 16){
       Consola.print(F("0"));
     }
     Consola.print(MensajeRTCM[i],HEX);
  }
  Consola.println();
  Consola.println();
}
#endif
