void errorwriteSDfile(void){
  /*#ifdef SD_present
    myFile = SD.open("log.txt", FILE_WRITE);
    if (myFile) {
      myFile.print("Reset board.");
      myFile.print("\n");
      myFile.close();
    }  
  #endif*/
  sermon.print("Error SD.");
  //definir como resetear la placa
}




void errorWire(void){
   if (error != 0){
    #ifdef SD_present
      myFile = SD.open("log.txt", FILE_WRITE);
      if (myFile) {
        myFile.print("Error Wire");
        switch (error) {
          case 1:
            myFile.print("Error Wire: data too long to fit in transmit buffer");
            myFile.print("\n");
            break;
          case 2:
            myFile.print("Error Wire: received NACK on transmit of address");
            myFile.print("\n");
            break;
          case 3:
            myFile.print("Error Wire: received NACK on transmit of data");
            myFile.print("\n");
            break;
          case 4:
            myFile.print("Error Wire: other error");
            myFile.print("\n");
            break;
          default: 
            break;
        }
        myFile.close();
      }
    #endif
    }
}  

//Byte, que indica el estado de la transmisión:

//0: éxito
//1: datos demasiado largos para encajar en el búfer de transmisión
//2: NACK recibido en la transmisión de la dirección
//3: NACK recibido tras la transmisión de datos
//4: otro error



