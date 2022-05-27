const int MAILLENGHT = 50;
char to_address[MAILLENGHT];
void take_picture();
void take_picture();

void consolaReaderCallback(){
  
  while(sermon.available()>0) {  
    sermon.println("RECIBIDO COMANDO");
    String algoMasQueS = sermon.readStringUntil('\n');
   
    if((algoMasQueS.substring(0,7)=="mailto ")){
      String nuevoMail = algoMasQueS.substring(7);
      for (int kChar = 0; kChar < MAILLENGHT; kChar++){
        to_address[kChar]= nuevoMail[kChar];
      }
      datasend_mailto();
      take_picture();
      memcpy(Celsius_thermal_image, image, sizeof(image));
      flagdata2GCS = SEND2GCS_IMAGE;
      senddatatoGCS_task.enable();
      return;
    }

    if((algoMasQueS.substring(0,5)=="reset")){
      reset_onboard();
      return;
    } 
      
    sermon.println(F("COMANDO DESCONOCIDO"));
  }
}



void printMail(){
  sermon.println(to_address);  
  return;
}
