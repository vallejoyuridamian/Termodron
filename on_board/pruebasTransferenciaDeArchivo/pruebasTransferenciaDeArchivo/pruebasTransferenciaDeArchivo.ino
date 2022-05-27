#define ConsolaMon
#define PX4def

/*
#include "/home/damian/Dropbox/Termodron_Proyecto/Software/MAVLink/c_library_v2/common/mavlink.h" // Libreria para MAVLink     
#include "/home/damian/Dropbox/Termodron_Proyecto/Software/MAVLink/TaskScheduler/TaskScheduler.h" // Libreria para multithreading
*/
#include "C:/Users/nacho15/Dropbox/Termodron_Proyecto/Software/MAVLink/c_library_v2/common/mavlink.h" // Libreria para MAVLink   
#include "C:/Users/nacho15/Dropbox/Termodron_Proyecto/Software/MAVLink/TaskScheduler/TaskScheduler.h" // Libreria para multithreading

#include "Constantes.h"
#include "PX4Reader.h"
#include "PX4.h"
#include <avr/pgmspace.h>

void setup() {

  runner.init();
  Consola.begin(115200);
  PX4.begin(57600);
  
  pinMode(PXEnablePin,OUTPUT);
  pinMode(ArdEnablePin,OUTPUT);
  
  digitalWrite(ArdEnablePin,HIGH);// Desconectamos a Arduino
  digitalWrite(PXEnablePin,LOW); // Conectamos a Pixhawk
  

  for(int i = 0;i<DATATOGCSSIZE;i++){ 
   dataToGCS[i] = 0xD5;

  }
  
  
  runner.addTask(heartBeat);
  runner.addTask(PX4Reader);
  heartBeat.enable();
  PX4Reader.enable();
  sendHeartBeat();

  
}

void loop() {
 runner.execute();
}
