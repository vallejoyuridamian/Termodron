/*
    GSC_Code

    Version:           1.0
    Contacto:          agustin.barriola@hotmail.com; vallejoyuridamian@gmail.com; reyesjoseignacio@gmail.com

*/


#define xnacho
#define xagustin
#define damian

#define xRTCMdef
#define Emaildef
#define xshield3Gdef
#define xGPSprueba
#define PX4def
#define ConsolaMon
#define xCoordenadasEnFloat
#define xHIL
#define xWeather

#ifdef damian
#include "/home/damian/Dropbox/Termodron_Proyecto/Software/MAVLink/c_library_v2/common/mavlink.h" // Libreria para MAVLink     
#include "/home/damian/Dropbox/Termodron_Proyecto/Software/MAVLink/TaskScheduler/TaskScheduler.h" // Libreria para multithreading
#endif

#ifdef agustin
#include "/Users/abarriol/Dropbox/Termodron_Proyecto/Software/MAVLink/c_library_v2/common/mavlink.h" // Libreria para MAVLink     
#include "/Users/abarriol/Dropbox/Termodron_Proyecto/Software/MAVLink/TaskScheduler/TaskScheduler.h" // Libreria para multithreading
#endif

#ifdef nacho
#include "C:/Users/nacho15/Dropbox/Termodron_Proyecto/Software/MAVLink/c_library_v2/common/mavlink.h" // Libreria para MAVLink     
#include "C:/Users/nacho15/Dropbox/Termodron_Proyecto/Software/MAVLink/TaskScheduler/TaskScheduler.h" // Libreria para multithreading
#endif

#include "Geometria.h"
#include "Constantes.h"
#include "ConsolaReader.h"
#include "PX4Reader.h"
#include "PX4.h"
#include "Shield3G.h"
#include "Imagen.h"
#include "Email.h"
#include "EmailReader.h"
#include "SMS.h"
#include "RTCM.h"
#include "Shield3GReader.h"
#include "RTCMReader.h"
#include "Xmodem.h"
#include "XmodemReader.h"
//#include <avr/pgmspace.h>
#include <malloc.h>
//#include <avr/wdt.h>

//#include "DHT.h"
#ifdef Weather
DHT dht(DHTPIN, DHTTYPE);
#endif

void setup() {

  pinMode(powerPin, OUTPUT);
  pinMode(pinShieldVivo, OUTPUT);
  pinMode(pinAntenaViva, OUTPUT);

  digitalWrite(powerPin, HIGH);
  digitalWrite(pinShieldVivo, LOW);
  digitalWrite(pinAntenaViva, LOW);

  runner.init();
#ifdef ConsolaMon
  Consola.begin(115200);
  runner.addTask(consolaReader);
  consolaReader.enable();
#endif

#ifdef GPSprueba
  Consola.begin(115200);
  Consola.println("Iniciando consola");
#endif

#ifdef shield3Gdef
  pinMode(rstModulePin, OUTPUT);
  pinMode(onModulePin, OUTPUT);
  Shield3G.begin(19200);
#ifdef ConsolaMon
  Consola.println(F("Apagando..."));
#endif
  power_off();
  delay(40);
#ifdef ConsolaMon
  Consola.println(F("Encendiendo..."));
#endif
  power_on();
#ifdef ConsolaMon
  Consola.println(F("Encendido"));
#endif
  setSim();
  while ((sendATcommand("AT+CREG?", "+CREG: ", 500) == 0));
  memset(MensajeShield3G, '\0', MAXMENSLEN);
  runner.addTask(Shield3GReader);
  Shield3GReader.enable();
#endif

#ifdef PX4def
  PX4.begin(57600);
  runner.addTask(heartBeat);
  runner.addTask(PX4Reader);
  heartBeat.enable();
  PX4Reader.enable();
  sendHeartBeat();
#ifdef GPSprueba
  float GPS_PRUEBA_TIME = 1e3;
  requestMSG(MAVLINK_MSG_ID_GPS_RAW_INT, GPS_PRUEBA_TIME); // #24
  requestMSG(MAVLINK_MSG_ID_GLOBAL_POSITION_INT, GPS_PRUEBA_TIME); //#33
#endif
#endif

#ifdef shield3Gdef
#ifdef Emaildef
  POP3Set();
  runner.addTask(checkEmail);
  runner.addTask(sendEmail);
  runner.addTask(enviarReportes);
  runner.addTask(emailReader);
  runner.addTask(Xmodem);
  runner.addTask(XmodemReader);
  checkEmail.enable();
  //enviarReportes.enable();
#endif

#ifdef RTCMdef
  runner.addTask(RTCMReader);
  runner.addTask(RTCM);
  RTCM.enable();
#ifdef GPSprueba
  Consola.println("Prendiendo RTCM");
#endif
#endif
#endif

#ifdef Weather
  dht.begin();
  pinMode(RainPIN, INPUT);
#endif

}

void loop() {
  runner.execute();
}


extern char _end;
extern "C" char *sbrk(int i);
char *ramstart = (char *)0x20070000;
char *ramend = (char *)0x20088000;

void memoria() {
  char *heapend = sbrk(0);
  register char * stack_ptr asm ("sp");
  struct mallinfo mi = mallinfo();
  printf("\nDynamic ram used: %d\n", mi.uordblks);
  printf("Program static ram used %d\n", &_end - ramstart);
  printf("Stack ram used %d\n\n", ramend - stack_ptr);
  printf("My guess at free mem: %d\n", stack_ptr - heapend + mi.fordblks);
}
