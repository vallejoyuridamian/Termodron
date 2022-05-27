void error_wire(void){
   if (error == 0){
    Serial.print("error=");
    Serial.println(error);
  }
}  

//Byte, que indica el estado de la transmisión:

//0: éxito
//1: datos demasiado largos para encajar en el búfer de transmisión
//2: NACK recibido en la transmisión de la dirección
//3: NACK recibido tras la transmisión de datos
//4: otro error
