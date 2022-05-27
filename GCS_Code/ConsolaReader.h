void arm();
void printMail();
boolean takeoff();
void disarm();
void resetGCS();
boolean sendRTL();
boolean land();
void resetPOP3(bool habilitar);
void consolaReaderCallback(){
  
  while(Consola.available()>0) {  
    Consola.println("RECIBIDO COMANDO");
    String algoMasQueS = Consola.readStringUntil('\n');
    if (algoMasQueS=="disarm"){
      Consola.println(F("RECIBIDO COMANDO DE DISARM"));
      disarm();
      return;
    }
    if (algoMasQueS=="arm"){
      Consola.println(F("RECIBIDO COMANDO DE ARM"));
      arm();
      return;
    }
    if (algoMasQueS=="resetmail"){
      Consola.println(F("RECIBIDO COMANDO DE RESETMAIL"));
      lasttotalemailsint=0;
      resetPOP3(true);
      return;
    }
    if (algoMasQueS=="rtl"){
      Consola.println(F("RECIBIDO COMANDO DE RTL"));
      sendRTL();
      return;
    }
    if (algoMasQueS=="resetgcs"){
      Consola.println(F("RECIBIDO COMANDO DE RESETGCS"));
      resetGCS();
      return;
    }
    if (algoMasQueS=="land"){
      Consola.println(F("RECIBIDO COMANDO DE LAND"));
      land();
      return;
    }
    if (algoMasQueS=="takeoff"){
      Consola.println(F("RECIBIDO COMANDO DE TAKEOFF"));
      takeoff();
      return;
    }
    if (algoMasQueS=="mail"){
      Consola.println(F("RECIBIDO COMANDO DE VER MAIL"));
      printMail();
      return;
    }
    
       
    if (algoMasQueS=="h"){
      Consola.println(F("*****************************************************************"));
      Consola.println(F("disarm - Desarma el Dron"));
      Consola.println(F("arm - Arma el Dron"));
      Consola.println(F("resetmail - Borra la cuenta de mails para leer el ultimo de nuevo"));
      Consola.println(F("rtl - Vuelve al punto de despegue"));
      Consola.println(F("resetgcs - Resetea la GCS"));
      Consola.println(F("land - Aterriza en el punto actual"));
      Consola.println(F("takeoff - Despega en el punto actual"));
      Consola.println(F("*****************************************************************"));
      return;
    }
    
    //11111111111111111111111111111111
    if((algoMasQueS.substring(0,7)=="mailto ")){
      String nuevoMail = algoMasQueS.substring(7);

      for (int kChar = 0; kChar < MAILLENGTH; kChar++){
        to_address[kChar]= nuevoMail[kChar];
        to_name[kChar]= nuevoMail[kChar];
      }  
      //sender_name = Consola.println(algoMasQueS.substring(7));      
      return;
    } 
  
     
        
    Consola.println(F("COMANDO DESCONOCIDO, INGRESE h PARA LISTA DE COMANDOS"));
  }
}



void printMail(){
  Consola.println(to_address);  
  return;
}
