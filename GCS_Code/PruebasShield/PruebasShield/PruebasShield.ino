
#define Consola Serial
#define Shield3G Serial3
#define px4 Serial2
#define sermon Serial

#include "/home/damian/Dropbox/Termodron_Proyecto/Software/MAVLink/c_library_v2/common/mavlink.h" // Libreria para MAVLink     
#include "/home/damian/Dropbox/Termodron_Proyecto/Software/MAVLink/TaskScheduler/TaskScheduler.h" // Libreria para multithreading
#include "/home/damian/Dropbox/Termodron_Proyecto/Software/GCS_Code/CalculadorZigZag/Geometria.h"
#include "/home/damian/Dropbox/Termodron_Proyecto/Software/GCS_Code/Constantes.h"
#include "/home/damian/Dropbox/Termodron_Proyecto/Software/GCS_Code/sendATcommand.ino"
#include "/home/damian/Dropbox/Termodron_Proyecto/Software/GCS_Code/setsim.ino"
#include "/home/damian/Dropbox/Termodron_Proyecto/Software/GCS_Code/Shield3G_lib.ino"
#include "/home/damian/Dropbox/Termodron_Proyecto/Software/GCS_Code/Func_MAVLink.ino"

void setup() {
  Shield3G.begin(19200);
  Consola.begin(9600);
  Consola.println(millis()); 
  Consola.println("Prendiendo Shield"); 
  // Se inicializa el scheduler y se le agrega el HB
  runner.init();
  runner.addTask(heartBeat);
  heartBeat.enable();

  // Se prende el Shield
  power_off();
  power_on();
  Consola.println(millis());
  // Se setea la SIM
  //setsim();
  // Consola.println(millis());
  
}

void loop() {
  runner.execute();
}
