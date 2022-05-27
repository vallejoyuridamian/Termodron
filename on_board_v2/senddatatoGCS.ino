const int DSI_SEND = 0; 
const int DSI_WACK = 1;
const int DSI_END = 2;
int DSI_STATUS = DSI_SEND;

int dsipart = 0;
const int dsitimeout = 4e3;
long int dsicounter;
int dsiretry = 0;

void senddatatoGCS(void){
  switch(DSI_STATUS) {
    case DSI_SEND:{
      dsisend();
      break;
    }
    case DSI_WACK:{
      dsiwack();
      break;
    }
    case DSI_END:{
      dsiend();
      break;
    }
    default:
    break;
  }
  
}


void dsisend(void){

  //check time para ver si solicitar las flags
  dsicounter = millis();
  solicitarAntena(); // Desconectamos a Pixhawk y conectamos a Arduino
  if(flagdata2GCS == SEND2GCS_LOG){
    getLogsize();
    datasend_log(dsipart);
    #ifdef debugging
    sermon.print(F("Send Log, part:"));
    sermon.println(dsipart);
    #endif
    
  }else if(flagdata2GCS == SEND2GCS_IMAGE){
    datasend_image(dsipart);    
    #ifdef debugging
    sermon.print(F("Send imagen, part:"));
    sermon.println(dsipart);
    #endif
  }else if(flagdata2GCS == SEND2GCS_SENSOR){
    DSI_STATUS = DSI_END;
    dsiack = 0;
    datasend_sensor();
    #ifdef debugging
    sermon.print(F("Send distance sensor"));
    #endif    
    return;
  }else if(flagdata2GCS == SEND2GCS_ERROR){
    //datasend_error();
    DSI_STATUS = DSI_END;
    dsiack = 0;
    #ifdef debugging
    sermon.print(F("Send error"));
    #endif 
    return;
  }else if(flagdata2GCS == SEND2GCS_GET){
    datasend_get();    
    #ifdef debugging
    sermon.println(F("Query flags to GCS"));
    #endif
  }
  DSI_STATUS = DSI_WACK;
  dsipart++;
}

void dsiwack(void){
  if(flagdata2GCS == SEND2GCS_LOG){
    if(dsiack == 1){
      if (size_log <= (dsipart*248)){
        DSI_STATUS = DSI_END;
        logsendenable = 0;
      }else{
        DSI_STATUS = DSI_SEND;
      }
      dsiack = 0;
    }else if(dsiack == 2){
      dsipart--;
      DSI_STATUS = DSI_SEND;
      dsiack = 0;
    }else{
      if((millis() - dsicounter) > dsitimeout){
        #ifdef debugging
        sermon.println(F("Send log error, timeout"));
        #endif
        DSI_STATUS = DSI_END;
      }
    }
    
  }else if(flagdata2GCS == SEND2GCS_IMAGE){   
    if(dsiack == 1){
      dsiretry = 0;
      if (dsipart < 4){
        DSI_STATUS = DSI_SEND;
      }else{
        DSI_STATUS = DSI_END;
      }
      dsiack = 0;
    }else if(dsiack == 2){
      dsipart--;
      DSI_STATUS = DSI_SEND;
      dsiack = 0;
      #ifdef debugging
      sermon.print(F("Nack, retry send Part:"));
      sermon.println(dsipart);
      #endif
    }else{
      if(((millis() - dsicounter) > dsitimeout)&&(dsiretry<3)){
        dsipart--;
        DSI_STATUS = DSI_SEND;
        dsiack = 0;
        dsiretry++;
        #ifdef debugging
        sermon.print(F("TimeOut, retry send Part:"));
        sermon.println(dsipart);
        #endif
        
      }else if(dsiretry >= 3){
        #ifdef debugging
        sermon.print(F("Abort send imagen, timeout"));
        #endif
        DSI_STATUS = DSI_END;
      }
    }
  }else if(flagdata2GCS == SEND2GCS_SENSOR){
    
  }else if(flagdata2GCS == SEND2GCS_ERROR){
    
  }else if(flagdata2GCS == SEND2GCS_GET){
    if(dsiack == 1){
      DSI_STATUS = DSI_END;
      dsiack = 0;
       #ifdef debugging
      sermon.print(F("Query flags OK"));
      #endif
    }else if(dsiack == 2){
      DSI_STATUS = DSI_SEND;
      dsiack = 0;
    }else{
      if((millis() - dsicounter) > dsitimeout){
        #ifdef debugging
        sermon.println(F("Query flags error, timeout"));
        #endif
        DSI_STATUS = DSI_END;
      }
    }
  }
}

void dsiend(void){
  dsipart = 0;  //Se sacó para DEMO
  dsiretry = 0;
  DSI_STATUS = DSI_SEND;
  devolverAntena(); // se desconecta a Arduino y se conecta Pixhawk (ya está el delay adentro)
  senddatatoGCS_task.disable();
}




