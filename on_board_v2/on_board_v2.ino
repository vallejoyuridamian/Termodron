//Include de Librerías
#define xnacho
#define xagustin
#define damian
#define xwindows
#define linux

#ifdef agustin
#include "C:/Users/abarriol/Dropbox/Termodron_Proyecto/Software/MAVLink/c_library_v2/common/mavlink.h" // Libreria para MAVLink   
#include "C:/Users/abarriol/Dropbox/Termodron_Proyecto/Software/MAVLink/TaskScheduler/TaskScheduler.h" // Libreria para multithreading
#include "C:/Users/abarriol/Dropbox/Termodron_Proyecto/Software/Camara_Termica/arduinoONBoard/SPI-nueva/SPI.h"
#endif

#ifdef nacho
#include "C:/Users/nacho15/Dropbox/Termodron_Proyecto/Software/MAVLink/c_library_v2/common/mavlink.h" // Libreria para MAVLink   
#include "C:/Users/nacho15/Dropbox/Termodron_Proyecto/Software/MAVLink/TaskScheduler/TaskScheduler.h" // Libreria para multithreading
#include "C:/Users/nacho15/Desktop/PROGRAMS/arduino-nightly/libraries/SPI/spi.h"
#endif

#ifdef damian
#ifdef linux
#include "/home/damian/Dropbox/Termodron_Proyecto/Software/MAVLink/c_library_v2/common/mavlink.h" // Libreria para MAVLink   
#include "/home/damian/Dropbox/Termodron_Proyecto/Software/MAVLink/TaskScheduler/TaskScheduler.h" // Libreria para multithreading
#include "/home/damian/Dropbox/Termodron_Proyecto/Software/Camara_Termica/arduinoONBoard/SPI-nueva/SPI.h"
#endif
#ifdef windows
#include "C:/MAVLink/c_library_v2/common/mavlink.h" // Libreria para MAVLink   
#include "C:/MAVLink/TaskScheduler/TaskScheduler.h" // Libreria para multithreading
#include "C:/Camara_Termica/arduinoONBoard/SPI-nueva/SPI.h"
#endif
#endif

#include <TimeLib.h>
#include <Wire.h>

#include <math.h>
#include <avr/pgmspace.h>
#include <malloc.h>
//#include <WatchDog.h>

// Include con definiciones
#include "lepton_def.h"
#include "mavlink_def.h"

//Si hay tarjeta SD, si no la hay comentar la siguiente linea
#define SD_present
//Si se está debugueando
#define debugging
//Si recibe los waypoint en int y no en float
#define waypoint_int

#ifdef SD_present
#include <SD.h>
//#include "SdFat.h"
//SdFat SD;
#define csSD 4

//Variables relativas al modulo SD
File myFile;
int image_number = 0;
char image_file[15];
char log_file[10] = "log.txt";
char aux_file[25];
#endif

//Chip select de la Flir Lepton
#define csFLIR 10

//Pin para reset
#define resetSPIPin 13

//Pin para reset
#define resetPin 12

//Pin para habilitar la antena para Pixhawk
#define ArdEnablePin 7

//Pin para habilitar la antena para Arduino
#define PXEnablePin 6

#ifdef debugging
#define sermon Serial
#endif

//Serial para conectar on-board a la Pixhawk
#define sermav Serial2

//Serial para conectar con antena 3dr
#define serant Serial3

//byte x = 0;

//variables relativas a la transferencia de archivos a la GCS
const int  DATATOGCSSIZE = 251;

//variables relativas a los sensores
const int sensor_echo[4] = {30, 32, 36, 34};
const int sensor_trig[4] = {31, 33, 37, 35};
int sensor_dist[4] = {0, 0, 0, 0};    //0:Abajo; 1:Izquierda; 2:Frente; 3:Derecha
bool sensor_act[4] = {false, false, false, false};

//Variables relativas al sketch
bool runonce = 0;

//Variables relativas al Watchdog
int watchdogTime_ms = 5000;

// esto tiene que estar para que ande
void watchdogSetup(void) {}

//variables relativas a la imagen
#define image_x (60)
#define image_y (80)
#define image_size (4800)
int image[image_x][image_y];  //word
int Celsius_thermal_image[image_x][image_y];
bool checked[image_x][image_y]; //arreglo auxiliar con los puntos ya chequeados


int r = 0; //cantidad de objetos
int s = 0; //cantidad de elementos en el objeto mayor
struct Indice {
  char a;
  char b;
};
Indice D;  //indices del punto medio del objeto mayor
Indice A[image_size];  //arreglo con los indices del cuerpo que va siendo encontrado

//valores a configurar para la deteccion de cuerpo caliente
const int T_max = 9000;//45;   //valor máximo a detectar
const int T_min = 9000;//8125;//25;   //valor mínimo a detectar
const int pixels_min = 1;//(int) 800 / (h_n * 0.011 * h_n * 0.011);
float cam_temp;
float fpa_temp;

//valores para configurar archivo .bmp
const int tmin_bmp = 0;
const int tmax_bmp = 40;
int tp_bmp = (tmax_bmp + tmin_bmp) / 2;

//Constantes de los estados
long int wait_time = 1000;
int detour = 5;
int state;

//Batería constantes
int bat_to_home = 25;
int bat_to_land = 8;

void consolaReaderCallback();

// Callback de las tareas
void heartBeatCallback() {
  sendHeartBeat();
}
void up_dateCallback() {
  up_date();
}
void statesCallback() {
  states();
}
void senddatatoGCSCallback() {
  senddatatoGCS();
}
void patWatchDogCallback() {
  watchdogReset();
}
// Periodos de las tareas
const int PERIODOheartBeat = 1e3;
const int PERIODOup_date = 1e2;
const int PERIODOstates = 1e3;
const int PERIODOsenddatatoGCS = 1e2;
const int PERIODOpatWatchDog = 1e3;
const int PERIODOCONSOLAREADER = 1;

// Tareas
Task heartBeat(PERIODOheartBeat, TASK_FOREVER, &heartBeatCallback); // Manda un heartbeat cada un segundo para siempre
Task up_date_task(PERIODOup_date, TASK_FOREVER, &up_dateCallback); //
Task states_task(PERIODOstates, TASK_FOREVER, &statesCallback); //
Task senddatatoGCS_task(PERIODOsenddatatoGCS, TASK_FOREVER, &senddatatoGCSCallback); //
Task patWatchDog(PERIODOpatWatchDog, TASK_FOREVER, &patWatchDogCallback); //
Task consolaReader(PERIODOCONSOLAREADER, TASK_FOREVER, &consolaReaderCallback);


Scheduler runner; // se crea el scheduler
#define wdtimer WatchDog

////////////////////Setup////////////////////

void setup() {
  watchdogEnable(watchdogTime_ms); //prende el WD
  //wdt_disable();
  //Inicializa la comunicación Serial con Pixhawk
  sermav.begin(57600);

  //Configura el buffer triestado
  serant.begin(57600);
  pinMode(PXEnablePin, OUTPUT);
  pinMode(ArdEnablePin, OUTPUT);
  digitalWrite(ArdEnablePin, HIGH); // Se desconecta On-board
  digitalWrite(PXEnablePin, LOW); // Se conecta Pixhawk
  //initialize Serial communication
#ifdef debugging
  sermon.begin(115200);
#endif

  for (int i = 0; i < 4; i++) {
    pinMode(sensor_trig[i], OUTPUT);
    digitalWrite(sensor_trig[i], LOW);
    pinMode(sensor_echo[i], INPUT);
    digitalWrite(sensor_echo[i], LOW);
  }

  //Inicializa la comunicación i2c con la Flir Lepton
  Wire.begin();

  // Configuración de la comunicación SPI con la Flir Lepton
  pinMode(csFLIR, OUTPUT);
  digitalWrite(csFLIR, HIGH);
  SPI.begin(csFLIR);
  //SPISettings SPIFLIRsett = SPISettings(10000000, MSBFIRST, SPI_MODE3);
  SPISettings SPIFLIRsett = SPISettings(20000000, MSBFIRST, SPI_MODE3);
  SPI.beginTransaction(csFLIR, SPIFLIRsett);
  SPI.endTransaction();
  //SPI.setDataMode(csFLIR, SPI_MODE3);
  //SPI.setBitOrder(csFLIR, MSBFIRST);
  //SPI.setClockDivider(csFLIR, 5);

  // Configuración e inicialización de la tarjeta SD
#ifdef SD_present
  SD_init();
#endif

  //Inicializacion para que reserve la memoria
  for (int i = 0; i < 60; i++) {
    for (int j = 0; j < 80; j++) {
      image[i][j] = 8000;
      Celsius_thermal_image[i][j] = 8000;
      checked[i][j] = 120;
      A[0].a = 0;
      A[0].b = 0;
    }
  }

}

////////////////////Loop Principal////////////////////
void loop() {


  if (runonce == 0) {

    shutup();         //Se callan todos los mensajes provenientes de la Pixhawk

    runner.init();    //Se inicializa el scheduler

    runner.addTask(patWatchDog);
    patWatchDog.enable();

    runner.addTask(consolaReader);
    consolaReader.enable();

    runner.addTask(heartBeat);  // Se le agrega la tarea heartBeat al scheduler
    heartBeat.enable();

    sendHeartBeat();        //Se manda un HB suelto para presentarse


    requestMSG( per_mav_msg, MAVLINK_MSG_ID_MISSION_CURRENT);         //
    requestMSG( per_mav_msg, MAVLINK_MSG_ID_BATTERY_STATUS);          //
    requestMSG( per_mav_msg, MAVLINK_MSG_ID_STATUSTEXT);              //
    requestMSG( per_mav_msg, MAVLINK_MSG_ID_SYSTEM_TIME);             //
    requestMSG( per_mav_msg, MAVLINK_MSG_ID_FLIGHT_INFORMATION);      //
    requestMSG( per_mav_msg, MAVLINK_MSG_ID_GLOBAL_POSITION_INT);     //pedido de posicion actual
    requestMSG( per_mav_msg, MAVLINK_MSG_ID_ATTITUDE);                //
    requestMSG( per_mav_msg, MAVLINK_MSG_ID_EXTENDED_SYS_STATE);      //
    requestMSG( per_mav_msg * 3, MAVLINK_MSG_ID_SCALED_PRESSURE);     //

    runner.addTask(up_date_task);  // Se le agrega la tarea up_date al scheduler
    up_date_task.enable();

    runner.addTask(states_task);  // Se le agrega la tarea states al scheduler
    states_task.enable();

    runner.addTask(senddatatoGCS_task);  // Se le agrega la tarea senddatatoGCS al scheduler
    //senddatatoGCS_task.enable();

    //lepton_set_conf();  //Configuración de la Flir Lepton

    //lepton_get_conf();  //Checkeo de la configuración de la Flir Lepton

    #ifdef SD_present
    sd_log_init();    //Se crea el archivo log.txt
    #endif

    runonce = 1;

  }

  runner.execute();

  /*

    String userAction = Serial.readStringUntil('\n');


    //depend on the user input do a ceratin action
    if(userAction == "foto"){
      take_picture();
      senddatatoGCS_task.enable();
      picture_print();
    }
  */

}

extern char _end;
extern "C" char *sbrk(int i);
char *ramstart = (char *)0x20070000;
char *ramend = (char *)0x20088000;

void memoriadisp() {
  char *heapend = sbrk(0);
  register char * stack_ptr asm ("sp");
  struct mallinfo mi = mallinfo();
  printf("\nDynamic ram used: %d\n", mi.uordblks);
  printf("Program static ram used %d\n", &_end - ramstart);
  printf("Stack ram used %d\n\n", ramend - stack_ptr);
  printf("My guess at free mem: %d\n", stack_ptr - heapend + mi.fordblks);
}




#ifdef debugging
void picture_print() {
  for (int i = 0; i < 60; i++) {
    for (int j = 0; j < 80; j++) {
      sermon.print((int)image[i][j], DEC);
      sermon.print("\t");
    }
    sermon.print("\n");
  }
}
#endif
