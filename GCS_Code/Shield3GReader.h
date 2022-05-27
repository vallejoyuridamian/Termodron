//Shield3GReader
void Shield3GRead();
void printMSG();
void printMSGHEX();


void Shield3GReaderCallBack(){

  if(Shield3G.available()>0){
    chequearPin(pinShieldVivo);
    Shield3GRead();
  }
}


void Shield3GRead(){
  char data_size_char[4]="\0";// Variable donde se guarda el data_size de los CHTTP DATA
  int j;
  int h;
  
        
  while(Shield3G.available() > 0){

    MensajeShield3G[iMENS3G] = Shield3G.read();
    iMENS3G++;

    if(iMENS3G > 1){

      //*************************************************//
      //Verifica que los últimos 2 caracteres sean "\r\n"//
      //*************************************************//
      
      if((MensajeShield3G[iMENS3G-2] == 0x0D)and(MensajeShield3G[iMENS3G-1] == 0x0A)){ 

        //************************************//
        //Esperando respuesta "Network opened"//
        //************************************//
        #ifdef RTCMdef
        if(TCPOpenReqNum>0){
          if((strstr(MensajeShield3G,"Network opened")!=NULL)or(strstr(MensajeShield3G,"Network is already opened")!=NULL)){
     
           TCPOpenReqNum = 0;
           RTCM.enable();
           RTCMSTATUS=RTCM_OPENED;
     
           Consola.println();
           Consola.println(F("Encontre el Network opened"));
           printMSG();

            
           iMENS3G=0;
           memset(MensajeShield3G,'\0',MAXMENSLEN);
           break; 
         }
         else{
             if(millis() - TCPOpenCounter > TCPOpenTimeout){ // Si nos pasamos del timeout prendemos el RCTM para que pida de nuevo 
               RTCM.enable();
             }
           }
        }
        
        //***************************************//
        //Esperando respuesta "CHTTPACT: REQUEST"//
        //***************************************//
        
        if(HTTPACTReqNum>0){  
          if(strstr(MensajeShield3G,"CHTTPACT: REQUEST")!=NULL){
    
            HTTPACTReqNum = 0;
            RTCM.enable();
            RTCMSTATUS=HTTP_OPENED;
      
            Consola.println();
            Consola.println("Encontre el CHTTPACT: REQUEST");
            printMSG();
      
            iMENS3G=0;
            memset(MensajeShield3G,'\0',MAXMENSLEN);
            break;
             
          }
          else{
            if(millis() - HTTPACTCounter > HTTPACTTimeout){ // Si nos pasamos del timeout prendemos el RCTM para que pida de nuevo 
              RTCM.enable();
            }
          }
        }
        
        //********************************************************//
        //Esperando respuesta CHTTPACT: DATA con los mensajes RTCM//
        //********************************************************//
        
        if(RTCMReqNum>0){
          if(strstr(MensajeShield3G,"CHTTPACT: DATA,")!=NULL){
      
            RTCMReqNum = 1; // lo dejamos en 1 para seguir leyendo los mensajes que vayan viniendo
            RTCMSTATUS=RTCM_OK;

  
            if(icy){
              //Consola.println("Era un ICY"); // El primero es un ICY pero después aparece info de la placa
              printMSG();
              iMENS3G=0;
              memset(MensajeShield3G,'\0',MAXMENSLEN);
              icy = false;
              break;
            }

            #ifdef ConsolaMon
            Consola.println("Encontre el +CHTTPACT: DATA,");
            printMSG();
            #endif
            
            //lee el size del "CHTTPACT: DATA"
            for (int j=16; j < iMENS3G-2; j++){
              data_size_char[j-16] = MensajeShield3G[j];
            }
            data_size_RTCM = atoi(data_size_char);
            
            
            iMENS3G=0;
            memset(MensajeShield3G,'\0',MAXMENSLEN);  //acá ya debería estar leyendo RTCM
            Shield3GReader.disable();
            RTCMReader.enable();

            break;
          }
          else{
            if((millis() - RTCMReqCounter > RTCMReqTimeout)and(RTCMSTATUS==HTTP_OPENED)){ //Sale por timeout pero habilita el RTCM para la próxima vuelta
              RTCM.enable();
            }
          }
        }

        
        //************************************//
        //Esperando respuesta "Network closed"//
        //************************************//
        
        if(TCPCloseReqNum>0){
          if((strstr(MensajeShield3G,"Network closed")!=NULL)or(strstr(MensajeShield3G,"Network is already closed")!=NULL)){
     
           TCPCloseReqNum = 0;
           RTCM.disable();
           RTCMSTATUS=RTCM_INIT;
           POP3STATUS= POP3_INIT;
           checkEmail.enable();
           Consola.println();
           Consola.println(F("Encontre el Network closed"));
           printMSG();
            
           iMENS3G=0;
           memset(MensajeShield3G,'\0',MAXMENSLEN);
           break; 
         }
         else{
             if(millis() - TCPCloseCounter > TCPCloseTimeout){ // Si nos pasamos del timeout prendemos el RCTM para que pida de nuevo 
               RTCM.enable();
             }
           }
        }
        #endif  
         
        //*****************************//
        //Esperando respuesta "POP3IN:"//
        //*****************************//
        if(POP3INReqNum>0){
          if(strstr(MensajeShield3G,"POP3: SUCCESS")!=NULL){
    
            Consola.println(F("Encontre el POP3: SUCCESS"));
            printMSG();      
            iMENS3G=0;
            memset(MensajeShield3G,'\0',MAXMENSLEN);
            POP3INReqNum = 0;
            POP3STATUS = POP3_OPENED;
            checkEmail.enable();
            break;
             
          }
         else{
            if(millis() - POP3INCounter > POP3INTimeout){ // Si nos pasamos del timeout prendemos el RCTM para que pida de nuevo 
              checkEmail.enable();
            }
          }
        }

        //******************************//
        //Esperando respuesta "POP3NUM:"//
        //******************************//
        if(POP3NUMReqNum>0){
          if(strstr(MensajeShield3G,"+POP3: ")!=NULL){
    
            Consola.println("Encontre el +POP3:");
            printMSG();

            j = 7;
            while(MensajeShield3G[j] != ','){
              totalemails[j-7] = MensajeShield3G[j];
              j++;
            }
            totalemailsint = atoi(totalemails);
            
            POP3NUMReqNum = 0;
            checkEmail.enable();
            POP3STATUS = POP3_GETTING;
            iMENS3G=0;
            memset(MensajeShield3G,'\0',MAXMENSLEN);
            break;
             
          }
          else{
            if(millis() - POP3NUMCounter > POP3NUMTimeout){ // Si nos pasamos del timeout prendemos el RCTM para que pida de nuevo 
              checkEmail.enable();
            }
          }
        }

        //******************************//
        //Esperando respuesta "POP3GET:"//
        //******************************//
        if(POP3GETReqNum>0){
          if(strstr(MensajeShield3G,"C:/Email")!=NULL){
    
            POP3GETReqNum = 0;
            checkEmail.enable();
            POP3STATUS = POP3_CD;
      
            //Consola.println();
            Consola.println("Encontre el C:/Email");
            printMSG();
            
            h = 0;
            for (j=9; j<iMENS3G-2; j++){
              if ((MensajeShield3G[j] == ' ') || (MensajeShield3G[j] == ',')){
               
              }
              else{
                path[h] = MensajeShield3G[j];
                h++;
              }
            }
            iMENS3G=0;
            memset(MensajeShield3G,'\0',MAXMENSLEN);
            break;
             
          }
          else{
            if(millis() - POP3GETCounter > POP3GETTimeout){ // Si nos pasamos del timeout prendemos el RCTM para que pida de nuevo 
              checkEmail.enable();
            }
          }
        }

        //**************************************//
        //Esperando respuesta "+FSCD: C:/Email/"//
        //**************************************//
        if(POP3CDReqNum>0){
          if(strstr(MensajeShield3G,"AT+FSCD=C:/Email")!=NULL){
            //POP3STATUS = POP3_OK; //Lo pongo en este estado para que pase a leer la siguiente línea
            POP3CDReqNum = 0;
            checkEmail.enable();
            POP3STATUS = POP3_READING;
      
            //Consola.println();
            Consola.println("Se movio a la carpeta");
            printMSG();
         
            iMENS3G=0;
            memset(MensajeShield3G,'\0',MAXMENSLEN);
            break;
          }

          else{
            if(millis() - POP3CDCounter > POP3CDTimeout){ // Si nos pasamos del timeout prendemos el RCTM para que pida de nuevo 
              checkEmail.enable();
            }
          }
        }

        //*****************************//
        //Esperando respuesta "Reading"//
        //*****************************//
        if(POP3READReqNum>0){
          if(strstr(MensajeShield3G,"Subject: ")!=NULL){

            for(j=9; j<11; j++){
              Code_subject[j-9] = MensajeShield3G[j];
            }
            Code = atoi(Code_subject);
            SubjectFlag = 1;
            iMENS3G=0;
            memset(MensajeShield3G,'\0',MAXMENSLEN);
            break;
          }
          if((strstr(MensajeShield3G,"From: ")!=NULL)||(strstr(MensajeShield3G,"Date: ")!=NULL)||(strstr(MensajeShield3G,"To: ")!=NULL)){
            iMENS3G=0;
            memset(MensajeShield3G,'\0',MAXMENSLEN);
            break;
          }
          if(strstr(MensajeShield3G,"FIN;")!=NULL){ //Puede no ser necesario esto y finalizar el cuerpo del mensaje cuando encuentre la palabra OK
    
            POP3READReqNum = 0;
            checkEmail.enable();
            emailReader.enable();
            POP3STATUS = POP3_CLOSE;
      
            Consola.println(F("Se leyo el correo")); //Acá conviene llamar una función para leer el correo
            Consola.println(Code);
            iMENS3G=0;
            memset(MensajeShield3G,'\0',MAXMENSLEN);
            break;
          }
          if(SubjectFlag == 1){
            for(j=0; j<iMENS3G; j++){
              Body[iBody] = MensajeShield3G[j];
              iBody++;
            }
            Consola.println(Body);
            iMENS3G=0;
            memset(MensajeShield3G,'\0',MAXMENSLEN);
            break;
          }
          else{
            if((millis() - POP3READCounter > POP3READTimeout)){ // Si nos pasamos del timeout prendemos el RCTM para que pida de nuevo 
              checkEmail.enable();
            }
          }
        }

        //******************************//
        //Esperando respuesta "POP3OUT:"//
        //******************************//
        if(POP3OUTReqNum>0){
          if(strstr(MensajeShield3G,"+POP3: SUCCESS")!=NULL){
    
            POP3OUTReqNum = 0;
            POP3STATUS = POP3_INIT;
            checkEmail.disable();
            enviarReportes.enable();
            
            //Consola.println();
            Consola.println("Encontre el SUCCESS del POP3OUT:");
            printMSG();      
            iMENS3G=0;
            memset(MensajeShield3G,'\0',MAXMENSLEN);
            break;
             
          }
          else{
            if(millis() - POP3OUTCounter > POP3OUTTimeout){ // Si nos pasamos del timeout prendemos el RCTM para que pida de nuevo 
              checkEmail.enable();
            }
          }
        }

        //******************************//
        //Esperando respuesta "SMTPIN:"//
        //******************************//
        if(SMTPINReqNum>0){
          if(strstr(MensajeShield3G,"OK")!=NULL){
    
            SMTPINReqNum = 0;
            sendEmail.enable();    //Habilita tarea para continuar con el envío de correo
            SMTPSTATUS = SMTP_AUTH;
      
            //Consola.println();
            Consola.println("Encontre el SMTPSRV");
            printMSG();      
            iMENS3G=0;
            memset(MensajeShield3G,'\0',MAXMENSLEN);
            break;
             
          }
          else{
            if(millis() - SMTPINCounter > SMTPINTimeout){ // Si nos pasamos del timeout prendemos el RCTM para que pida de nuevo 
              sendEmail.enable();
            }
          }
        }

        //******************************//
        //Esperando respuesta "SMTPAUTH:"//
        //******************************//
        if(SMTPAUTHReqNum>0){
          if(strstr(MensajeShield3G,"OK")!=NULL){
    
            SMTPAUTHReqNum = 0;
            sendEmail.enable();    //Habilita tarea para continuar con el envío de correo
            SMTPSTATUS = SMTP_FROM;
      
            //Consola.println();
            Consola.println("Encontre el SMTPAUTH:");
            printMSG();      
            iMENS3G=0;
            memset(MensajeShield3G,'\0',MAXMENSLEN);
            break;
             
          }
          else{
            if(millis() - SMTPAUTHCounter > SMTPAUTHTimeout){ // Si nos pasamos del timeout prendemos el RCTM para que pida de nuevo 
              sendEmail.enable();
            }
          }
        }

        //******************************//
        //Esperando respuesta "SMTPFROM:"//
        //******************************//
        if(SMTPFROMReqNum>0){
          if(strstr(MensajeShield3G,"OK")!=NULL){
    
            SMTPFROMReqNum = 0;
            sendEmail.enable();    //Habilita tarea para continuar con el envío de correo
            SMTPSTATUS = SMTP_RCPT;
      
            //Consola.println();
            Consola.println("Encontre el SMTPFROM:");
            printMSG();      
            iMENS3G=0;
            memset(MensajeShield3G,'\0',MAXMENSLEN);
            break;
             
          }
          else{
            if(millis() - SMTPFROMCounter > SMTPFROMTimeout){ // Si nos pasamos del timeout prendemos el RCTM para que pida de nuevo 
              sendEmail.enable();
            }
          }
        }

        //******************************//
        //Esperando respuesta "SMTPRCPT:"//
        //******************************//
        if(SMTPRCPTReqNum>0){
          if(strstr(MensajeShield3G,"OK")!=NULL){
    
            SMTPRCPTReqNum = 0;
            sendEmail.enable();    //Habilita tarea para continuar con el envío de correo
            SMTPSTATUS = SMTP_SUB;
      
            //Consola.println();
            Consola.println("Encontre el SMTPRCPT:");
            printMSG();      
            iMENS3G=0;
            memset(MensajeShield3G,'\0',MAXMENSLEN);
            break;
             
          }
          else{
            if(millis() - SMTPRCPTCounter > SMTPRCPTTimeout){ // Si nos pasamos del timeout prendemos el RCTM para que pida de nuevo 
              sendEmail.enable();
            }
          }
        }

        //******************************//
        //Esperando respuesta "SMTPSUB:"//
        //******************************//
        if(SMTPSUBReqNum>0){
          if(strstr(MensajeShield3G,"OK")!=NULL){
    
            SMTPSUBReqNum = 0;
            sendEmail.enable();    //Habilita tarea para continuar con el envío de correo
            SMTPSTATUS = SMTP_BODY;
      
            //Consola.println();
            Consola.println("Encontre el SMTPSUB:");
            printMSG();      
            iMENS3G=0;
            memset(MensajeShield3G,'\0',MAXMENSLEN);
            break;
             
          }
          else{
            if(millis() - SMTPSUBCounter > SMTPSUBTimeout){ // Si nos pasamos del timeout prendemos el RCTM para que pida de nuevo 
              sendEmail.enable();
            }
          }
        }

        //******************************//
        //Esperando respuesta "SMTPBODY:"//
        //******************************//
        if(SMTPBODYReqNum>0){
          if(strstr(MensajeShield3G,"OK")!=NULL){
    
            SMTPBODYReqNum = 0;
            sendEmail.enable();    //Habilita tarea para continuar con el envío de correo
            
            if(flg_has_att){
              SMTPSTATUS = SMTP_FILE;
            }
            else{
              SMTPSTATUS = SMTP_SEND;
            }
      
            //Consola.println();
            Consola.println("Encontre el SMTPBODY:");
            printMSG();      
            iMENS3G=0;
            memset(MensajeShield3G,'\0',MAXMENSLEN);
            break;
             
          }
          else{
            if(millis() - SMTPBODYCounter > SMTPBODYTimeout){ // Si nos pasamos del timeout prendemos el RCTM para que pida de nuevo 
              sendEmail.enable();
            }
          }
        }

        //******************************//
        //Esperando respuesta "SMTPFILE:"//
        //******************************//
        if(SMTPFILEReqNum>0){
          if(strstr(MensajeShield3G,"OK")!=NULL){
    
            SMTPFILEReqNum = 0;
            sendEmail.enable();    //Habilita tarea para continuar con el envío de correo
            
            SMTPSTATUS = SMTP_SEND;
                  
            //Consola.println();
            Consola.println("Encontre el SMTPFILE:");
                
            iMENS3G=0;
            memset(MensajeShield3G,'\0',MAXMENSLEN);
            break;
             
          }
          else{
            if(millis() - SMTPFILECounter > SMTPFILETimeout){ // Si nos pasamos del timeout prendemos el RCTM para que pida de nuevo 
              sendEmail.enable();
            }
          }
        }


        
        //******************************//
        //Esperando respuesta "SMTPSEND:"//
        //******************************//
        if(SMTPSENDReqNum>0){
          if(strstr(MensajeShield3G,"+SMTP: SUCCESS")!=NULL){
    
            SMTPSENDReqNum = 0;
            checkEmail.enable();
            sendEmail.disable();
            //Habilita tarea para continuar con el envío de correo
            SMTPSTATUS = SMTP_INIT;
      
            //Consola.println();
            Consola.println("Encontre el +SMTP: SUCCESS:");
            printMSG();      
            iMENS3G=0;
            memset(MensajeShield3G,'\0',MAXMENSLEN);
            sendEmail.disable();
            break;
             
          }
          else{
            if(millis() - SMTPSENDCounter > SMTPSENDTimeout){ // Si nos pasamos del timeout prendemos el RCTM para que pida de nuevo 
              sendEmail.enable();
            }
          }
        }



        //************************************//
        //Esperando respuesta "+FSCD:"//
        //************************************//
        
        if(XMODEMINITReqNum>0){
          if(strstr(MensajeShield3G,"OK")!=NULL){
     
           XMODEMINITReqNum = 0;
           Xmodem.enable();
           XMODEMSTATUS=XMODEM_DEL;
     
           Consola.println();
           Consola.println(F("Encontre el +FSCD:"));
           
           checkEmail.disable(); // Apagamos para que no se mezclen
           
           iMENS3G=0;
           memset(MensajeShield3G,'\0',MAXMENSLEN);
           break; 
         }
         else{
             if(millis() - XMODEMINITCounter > XMODEMINITTimeout){
               Xmodem.enable();
             }
           }
        }

        //************************************//
        //Esperando respuesta "OK" del Delete//
        //************************************//
        
        if(XMODEMDELReqNum>0){
          if((strstr(MensajeShield3G,"OK")!=NULL)or(strstr(MensajeShield3G,"ERROR")!=NULL)){
     
           XMODEMDELReqNum = 0;
           Xmodem.enable();
           XMODEMSTATUS=XMODEM_SET_FILE;
     
           Consola.println();
           Consola.println(F("Encontre el OK del Delete"));
            
           iMENS3G=0;
           memset(MensajeShield3G,'\0',MAXMENSLEN);
           break; 
         }
         else{
             if(millis() - XMODEMDELCounter > XMODEMDELTimeout){
               Xmodem.enable();
             }
           }
        }

        //************************************//
        //Esperando respuesta "OK" del SetFile//
        //************************************//
        
        if(XMODEMSETFILEReqNum>0){
          if(strstr(MensajeShield3G,"OK")!=NULL){
     
           XMODEMSETFILEReqNum = 0;
           Xmodem.disable();
           XMODEMSTATUS=XMODEM_TRANS;
     
           Consola.println();
           Consola.println(F("Encontre el OK del Set File"));


           Shield3GReader.disable();
           XmodemReader.enable(); 
           iMENS3G=0; 
           memset(MensajeShield3G,'\0',MAXMENSLEN);
           break; 
         }
         else{
             if(millis() - XMODEMSETFILECounter > XMODEMSETFILETimeout){
               Xmodem.enable();
             }
           }
        }














        
        //****************************************************//
        //Aquí es la parte en que sale si no entró a ningún IF//
        //****************************************************//
        
        Consola.println(F("Mensaje no esperado"));
        
      /*  if(strstr(MensajeShield3G,"ERROR")!=NULL){
         printMSG();
         delay(100);
         resetGCS(); 
         }

*/
        if(strstr(MensajeShield3G,"+POP3: SERVER ERROR")!=NULL){
          printMSG();
          resetPOP3(true);
          iMENS3G=0;
          memset(MensajeShield3G,'\0',MAXMENSLEN); 
          #ifdef ConsolaMon
          Consola.print("Reseteando tarea POP3 por vez numero ");
          Consola.println(POP3try);
          #endif
        }
          
        if(strstr(MensajeShield3G,"+SMTP: SERVER ERROR")!=NULL){
          printMSG();
          resetSMTP(true); 
          iMENS3G=0;
          memset(MensajeShield3G,'\0',MAXMENSLEN);
          #ifdef ConsolaMon
          Consola.print("Reseteando tarea SMTP por vez numero ");
          Consola.println(SMTPtry);
          #endif
        }
      

        if(strstr(MensajeShield3G,"ERROR")!=NULL){
          printMSG();
          if (POP3STATUS != POP3_INIT){
            resetPOP3(true); 
            iMENS3G=0;
            memset(MensajeShield3G,'\0',MAXMENSLEN);
            #ifdef ConsolaMon
            Consola.print("Reseteando tarea SMTP por vez numero ");
            Consola.println(POP3try);
            #endif            
          }
          if (SMTPSTATUS != SMTP_INIT){
            resetSMTP(true); 
            iMENS3G=0;
            memset(MensajeShield3G,'\0',MAXMENSLEN);
            #ifdef ConsolaMon
            Consola.print("Reseteando tarea SMTP por vez numero ");
            Consola.println(SMTPtry);
            #endif    
          }

        }

       /* if(strstr(MensajeShield3G,"START")!=NULL){

         printMSG();
         delay(100);
         resetGCS(); 
         }
*/
  /*       if(strstr(MensajeShield3G,"ERROR\r\n")!=NULL){
         printMSG();
         delay(100);
         resetGCS(); 
         }*/
 
        printMSG();
        iMENS3G=0;
        memset(MensajeShield3G,'\0',MAXMENSLEN);
        break; 
      }
    }
  }
}


//*********//
//FUNCIONES//
//*********//

void printMSG(){
  for(int i=0; i<iMENS3G;i++){
    Consola.print(MensajeShield3G[i]);
  }
  //Consola.println();
}



void printMSGHEX(){
  for(int i=0; i<iMENS3G;i++){

    if(MensajeShield3G[i] < 16){
      Consola.print("0");
    }
    
    Consola.print(MensajeShield3G[i],HEX);
  }
  Consola.println();
  Consola.println();
}

