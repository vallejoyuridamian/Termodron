//initialize the libraries used for the communications
#include <Wire.h>
#include <SPI.h>

#include <math.h>

//Si lo saco se rompe
byte x = 0;

//para los errores Wire.endTransmission
byte error;

//define memory adresses to acces them
#define ADDRESS  (0x2A)
#define COMMANDID_REG (0x04)
#define DATALEN_REG (0x06)
#define DATA0 (0x08)

//define the modules ID to control the communication i2c (used for confguring the module)
#define AGC (0x01)
#define SYS (0x02)
#define VID (0x03)
#define OEM (0x08)
#define RAD (0x0E)

//define the command ID to control the communication i2c (used for confguring the module)
#define GET (0x00)
#define SET (0x01)
#define RUN (0x02)

//define de size of a VoSPI packet, 2bytes for ID, 2bytes for CRC and 160bytes for the frame
#define VOSPI_FRAME_SIZE (164)
byte lepton_frame_packet[VOSPI_FRAME_SIZE];

//defining variables related with the image
#define image_x (60)
#define image_y (80)
word image[image_x][image_y];
int r = 0; //cantidad de objetos
int s = 0; //cantidad de elementos en el objeto mayor
struct Indice {
  int a;
  int b;
};
Indice D;  //indices del punto medio del objeto mayor

//defining variables related with the sensors
int sensor_echo[5] = {30, 32, 34, 36, 38};
int sensor_trig[5] = {31, 33, 35, 37, 39};
long sensor_dist[5] = {0, 0, 0, 0, 0};

//Initialize the variables used in this sketch
int i,j;
bool runonce = 0;
bool donecapturing = 0; 
boolean captureImage = false;

//Initialize the variables inteface Pixhawk
float chpt_lon[4],chpt_lat[4]; //coord de el punto 0, del check point pasado, del lugar actual y del check point siguente
int h_0, h_n;

int h_max = 20;   //20m equivalen a 22*22 cm^2 cada pixel
int h_min = 10;   //10m equivalen a un área por foto de 9.5*6.8 m^2

int orientation;  //orientación respecto al Norte en grados

void setup(){

  for (i=0; i<5; i++){
    pinMode(sensor_trig[i], OUTPUT);
    digitalWrite(sensor_trig[i], LOW);
  }
  for (i=0; i<5; i++){
    pinMode(sensor_echo[i], INPUT);
    digitalWrite(sensor_echo[i], LOW);
  }
 
  //initialize i2c communication
  Wire.begin();

  //initialize Serial communication
  Serial.begin(115200);
  //set the time out of the serial input
  Serial.setTimeout(90000UL);

  //configure spi communication
  pinMode(10, OUTPUT);
  SPI.setDataMode(SPI_MODE3);
  SPI.setClockDivider(5);

  //initialize SPI communication
  SPI.begin();
}

void loop(){  
  if (runonce == 0) {
    conf_lepton();

    //This functions enables the agc mode
    //agc_enable();

    runonce = 1;
    delay(200);
  }
    
  //datos que por MAVLINK se solicitan al PX4
  data_requested = Serial.readStringUntil('\n'); 

  take_picture();
  donecapturing = false;
  search_body();

  //Vericación----------------------------------------
  for(i=0;i<image_x;i++){
      for(j=0;j<image_y;j++){
        Serial.print((int)image[i][j], DEC);
        Serial.print("\t");
      }
      Serial.print("\n");
  }
  Serial.println(OK);
  Serial.print(s, DEC);
  Serial.print("\n");
  Serial.print(r, DEC);
  Serial.print("\n");
  Serial.print(D.a, DEC);
  Serial.print("\t");
  Serial.print(D.b, DEC);
  Serial.print("\n");
  //-------------------------------------------------

  sens_read();
  if (change_chpt()){
     //rutina para escribir el nuevo o los nuevos CHPT al PX4  
    
  }      
  



  
  
  Serial.flush();//Wait end transmition
  
}
