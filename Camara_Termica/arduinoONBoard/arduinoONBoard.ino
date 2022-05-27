//initialize the libraries used for the communications
#include "C:/Users/nacho15/Dropbox/Termodron_Proyecto/Software/MAVLink/c_library_v2/common/mavlink.h" // Libreria para MAVLink   
#include "C:/Users/nacho15/Dropbox/Termodron_Proyecto/Software/MAVLink/TaskScheduler/TaskScheduler.h" // Libreria para multithreading
#include "C:/Users/nacho15/Desktop/PROGRAMS/arduino-nightly/libraries/SPI/spi.h"
#include <Wire.h>
#include <TimeLib.h>
#include <math.h>


#define SD_present
#define debugging

#ifdef SD_present
  #include <SD.h>
  //#include "SdFat.h"
  //SdFat SD;
  #define csSD 52
#endif
#define csFLIR 10



#ifdef debugging
  #define sermon Serial
#endif

#define sermav Serial2



byte x = 0;

/* Inicio solo para envio de comandos a Lepton Flir */
//para los errores Wire.endTransmission
byte error;

//Direcciones de acceso a memoria
#define ADDRESS  (0x2A)
#define COMMANDID_REG (0x04)
#define DATALEN_REG (0x06)
#define DATA0 (0x08)

//modulos ID de control de communicacion i2c
#define AGC (0x01)
#define SYS (0x02)
#define VID (0x03)
#define OEM (0x08)
#define RAD (0x0E)

//comandos ID de control de comunicación I2C
#define GET (0x00)
#define SET (0x01)
#define RUN (0x02)

//tamaño VoSPI packet, 2bytes for ID, 2bytes for CRC and 160bytes for the frame
#define VOSPI_FRAME_SIZE (164)
byte lepton_frame_packet[VOSPI_FRAME_SIZE];

//variables relativas a la imagen
#define image_x (60)
#define image_y (80)
int image[image_x][image_y];//word???
int r = 0; //cantidad de objetos
int s = 0; //cantidad de elementos en el objeto mayor
struct Indice {
  int a;
  int b;
};
Indice D;  //indices del punto medio del objeto mayor

//variables relativas a los sensores
int sensor_echo[4] = {30, 32, 34, 36};
int sensor_trig[4] = {31, 33, 35, 37};
int sensor_dist[4] = {0, 0, 0, 0};
bool sensor_act[4] = {false, false, false, false};

//Variables relativas al sketch
bool runonce = 0;

//Variables para comunicacion de datos MAVLink
struct WayPoint {
  double lat;
  double lon;
  double alt;
};
WayPoint home_wp, current_wp, actual_wp, new_wp;  //coord de el punto 0, del lugar actual, del way point siguente y del nuevo
uint16_t actual_yaw, prior_yaw;
WayPoint mission_wps[200];


// Callback de las tareas
void heartBeatCallback();
void up_date();
void states();
Task heartBeat(1000, TASK_FOREVER, &heartBeatCallback); // Manda un heartbeat cada un segundo para siempre
Task up_date_task(1000, TASK_FOREVER, &up_date); // 
Task states_task(100, TASK_FOREVER, &states); // 
Scheduler runner; // se crea el scheduler

uint8_t system_type = MAV_TYPE_QUADROTOR; // 2
uint8_t autopilot_type = MAV_AUTOPILOT_PX4; // 12
uint8_t system_id = 253; // Identifica al sistema, 255 es standard para CGS
uint8_t component_id = 253; // Identifica al sistema, 255 es standard para CGS
uint8_t base_mode=0;
uint32_t custom_mode=0;
uint8_t system_status=0;
uint8_t stream_id=0;
uint8_t target_system=1;
uint8_t target_component=1;
uint8_t req_stream_id=0;
uint8_t confirmation =0;
uint8_t frame = MAV_FRAME_GLOBAL_RELATIVE_ALT; // 3, x = latitud, y = longitud, z= altitud sobre home
uint16_t command;
uint8_t current=1;
uint8_t autocontinue=1;
float param1;
float param2=-1;
float param3; 
float param4;
int timeout =5; // tiempo maximo de espera para los waypoints
uint16_t count = 0; // cantidad de waypoints que tiene una mision
uint16_t seq = 0;
uint16_t auxseq = 0;
uint32_t tmax = 2000; // timeout para los mensajes
float t = 1000;
boolean received = 0;

int32_t bat;
int16_t bat_temp;
uint16_t bat_volt[10];
int16_t bat_curr;
int32_t bat_ener;
char statusText[50];
uint8_t severity;
uint64_t time_unix_usec;   //Timestamp of the master clock in microseconds since UNIX epoch.
uint64_t arming_time_utc;
uint64_t takeoff_time_utc;
uint32_t time_boot_ms;
float att_pitch;
float att_roll;
float att_yaw;
float att_pitch_speed;
float att_roll_speed;
float att_yaw_speed;

bool time_ref= false;
uint64_t boot_hour_us;
uint64_t actual_time_us;

#ifdef SD_present
  //Variables relativas al modulo SD
  File myFile;
  int image_number = 0;
  char image_file[15];
#endif


void setup(){
  //initialize Serial communication
  sermav.begin(115200);
  
  //initialize Serial communication
  #ifdef debugging
    sermon.begin(115200);
  #endif
 
  for (int i=0; i<4; i++){
    pinMode(sensor_trig[i], OUTPUT);
    digitalWrite(sensor_trig[i], LOW);
    pinMode(sensor_echo[i], INPUT);
    digitalWrite(sensor_echo[i], LOW);
  }
  
  //initialize i2c communication
  Wire.begin();

//  configure spi communication

  pinMode(csFLIR, OUTPUT);
  digitalWrite(csFLIR, HIGH);
  SPI.begin(csFLIR);
  SPISettings SPIFLIRsett = SPISettings(10000000, MSBFIRST, SPI_MODE3);
  //SPISettings SPIFLIRsett = SPISettings(20000000, MSBFIRST, SPI_MODE3);
  SPI.beginTransaction(csFLIR, SPIFLIRsett);
  SPI.endTransaction(); 
//  SPI.setDataMode(csFLIR, SPI_MODE3);
//  SPI.setBitOrder(csFLIR, MSBFIRST);
//  SPI.setClockDivider(csFLIR, 5);

#ifdef SD_present
  pinMode(csSD, OUTPUT);
  digitalWrite(csSD, HIGH);  
//  SPI.begin(csSD);
//  SPI.setDataMode(csSD, SPI_MODE0);
//  SPI.setBitOrder(csSD, MSBFIRST);
//  SPI.setClockDivider(csSD, 128);
  if (!SD.begin(20000000, csSD)){
    #ifdef debugging
      sermon.println("Se ha producido un fallo modulo_SD");
    #endif
  }else{
    #ifdef debugging
      sermon.println("modulo_SD OK");
    #endif
  }
#endif
  
}



void loop(){  
  
  if (runonce == 0) {
    #ifdef SD_present
      sd_log_init();
    #endif
    //lepton_conf();
    shutup();
    //Se inicializa el scheduler
    runner.init();
    // Se le agrega el HB
    runner.addTask(heartBeat);
    heartBeat.enable();
    //Se manda un HB suelto para presentarse
    sendHeartBeat();
    request_all_mission();

    t = 1000;
    requestMSG( t, MAVLINK_MSG_ID_GLOBAL_POSITION_INT);//pedido de posicion actual 
    requestMSG( t, MAVLINK_MSG_ID_MISSION_CURRENT);//
    requestMSG( t, MAVLINK_MSG_ID_BATTERY_STATUS);//
    requestMSG( t, MAVLINK_MSG_ID_STATUSTEXT);//
    requestMSG( t, MAVLINK_MSG_ID_SYSTEM_TIME);//
    requestMSG( t, MAVLINK_MSG_ID_FLIGHT_INFORMATION);//
    requestMSG( 0, MAVLINK_MSG_ID_HOME_POSITION);//Dejar de pedirlo luego de request_all_mission()
    requestMSG( t, MAVLINK_MSG_ID_ATTITUDE);//
  
    runner.addTask(up_date_task);
    up_date_task.enable();
    runner.addTask(states_task);
    states_task.enable();
    
    runonce = 1;
    delay(200);
    float time_proc;
    time_proc = millis();
    take_picture();
    time_proc = millis() - time_proc;
    sermon.print("\n");
    sermon.print("takepicture: ");
    sermon.println(time_proc);
    
    time_proc = millis();
    search_body();
    time_proc = millis() - time_proc;
    sermon.print("searchbody: ");
    sermon.println(time_proc);
    
    time_proc = millis();
    sensors_read();
    time_proc = millis() - time_proc;
    sermon.print("sensorsread: ");
    sermon.println(time_proc);

  }

  //runner.execute();
  
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
