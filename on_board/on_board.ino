//Include de Librerías
#define xnacho
#define damian

#ifdef nacho
#include "C:/Users/nacho15/Dropbox/Termodron_Proyecto/Software/MAVLink/c_library_v2/common/mavlink.h" // Libreria para MAVLink   
#include "C:/Users/nacho15/Dropbox/Termodron_Proyecto/Software/MAVLink/TaskScheduler/TaskScheduler.h" // Libreria para multithreading
#include "C:/Users/nacho15/Desktop/PROGRAMS/arduino-nightly/libraries/SPI/spi.h"
#endif

#ifdef damian
#include "/home/damian/Dropbox/Termodron_Proyecto/Software/MAVLink/c_library_v2/common/mavlink.h" // Libreria para MAVLink   
#include "/home/damian/Dropbox/Termodron_Proyecto/Software/MAVLink/TaskScheduler/TaskScheduler.h" // Libreria para multithreading
#include "/home/damian/Dropbox/Termodron_Proyecto/Software/Camara_Termica/arduinoONBoard/SPI-nueva/SPI.h"
//#include "/home/damian/Dropbox/Termodron_Proyecto/Software/on_board/Librerias/Time.h"
#endif

#include <TimeLib.h>
#include <Wire.h>
#include <math.h>

// Include con definiciones
#include "lepton_def.h"
#include "mavlink_def.h"

//Si hay tarjeta SD, si no la hay comentar la siguiente linea
#define SD_present
//Si se está debugueando
#define debugging

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

//Serial para conectarse al transceptor
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

//variables relativas a la imagen
#define image_x (60)
#define image_y (80)
#define image_size (4800)
int image[image_x][image_y];  //word
byte Celsius_thermal_image[image_x][image_y];
int r = 0; //cantidad de objetos
int s = 0; //cantidad de elementos en el objeto mayor
struct Indice {
  int a;
  int b;
};
Indice D;  //indices del punto medio del objeto mayor

//valores a configurar para la deteccion de cuerpo caliente
const int T_max = 9000;//45;   //valor máximo a detectar
const int T_min = 8070;//25;   //valor mínimo a detectar
const int pixels_min = 1;//(int) 800 / (h_n * 0.011 * h_n * 0.011);
float cam_temp;
float fpa_temp;

//valores para configurar archivo .bmp
const int tmin_bmp = 10;
const int tmax_bmp = 40;
int tp_bmp = (tmax_bmp + tmin_bmp)/2;

//Constantes de los estados
long int wait_time = 1000;
int detour = 5;

//Batería constantes
int bat_to_home = 25;
int bat_to_land = 8;

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

// Periodos de las tareas
const int PERIODOheartBeat = 1e3;
const int PERIODOup_date = 1e3;
const int PERIODOstates = 1e3;

// Tareas
Task heartBeat(PERIODOheartBeat, TASK_FOREVER, &heartBeatCallback); // Manda un heartbeat cada un segundo para siempre
Task up_date_task(PERIODOup_date, TASK_FOREVER, &up_dateCallback); // 
Task states_task(PERIODOstates, TASK_FOREVER, &statesCallback); // 

Scheduler runner; // se crea el scheduler



////////////////////Setup////////////////////

void setup(){
  //Inicializa la comunicación Serial con Pixhawk
  sermav.begin(57600);
  
  //Configura el buffer triestado
  serant.begin(57600);
  pinMode(PXEnablePin,OUTPUT);
  pinMode(ArdEnablePin,OUTPUT);
  digitalWrite(ArdEnablePin,HIGH);// Desconectamos a Arduino
  digitalWrite(PXEnablePin,LOW); // Conectamos a Pixhawk
  
  //initialize Serial communication
  #ifdef debugging
    sermon.begin(115200);
  #endif
 
  for (int i=0; i< 4; i++){
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
}

////////////////////Loop Principal////////////////////
void loop(){  


  if (runonce == 0) {
    #ifdef SD_present
    sd_log_init();    //Se crea el archivo log.txt
    #endif

    shutup();         //Se callan todos los mensajes provenientes de la Pixhawk
    
    runner.init();    //Se inicializa el scheduler
    
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
    
    
    runner.addTask(up_date_task);  // Se le agrega la tarea up_date al scheduler
    up_date_task.enable();

    runner.addTask(states_task);  // Se le agrega la tarea states al scheduler
    states_task.enable();

    lepton_set_conf();  //Configuración de la Flir Lepton
    
    lepton_get_conf();  //Checkeo de la configuración de la Flir Lepton
    
    runonce = 1;

  }

  //sermon.println(millis());
  up_date();
  //sermon.println(millis());
  runner.execute();
  
}


#ifdef debugging
  void picture_print(){      
        for(int i=0;i<60;i++){
          for(int j=0;j<80;j++){
            sermon.print((int)image[i][j], DEC);
            sermon.print("\t");    
          }
          sermon.print("\n");
        }
  }
#endif
