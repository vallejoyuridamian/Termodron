

const char smtp_server[ ] = "smtp.montevideo.com.uy";          // SMTP server
const char pop3_server[ ] = "mail.montevideo.com.uy";          // POP3 server
const char smtp_user_name[ ] = "termodron1@montevideo.com.uy"; // smtp user name
const char pop3_user_name[ ] = "termodron1";                   // pop3 user name
const char mail_password[ ] = "Termo1";                        // e-mail password
const char smtp_port[ ] = "25";                                // SMTP server port (25, 465 or 587)
const char pop3_port[ ] = "110";                               // POP3 server port (110 or 995)

//Información de número de teléfono destino

const char phone_number[]="+59898065912";

//Información de la tarjeta SIM
const char sim_pin[] = "4725";
const char sim_apn[] = "prepago.ancel";             //Servicio prepago
const char sim_user[] = "bam";
const char sim_password[] = "bam";
//const char apn[] = "antel.lte";                //Servicio contrato

//Información de direcciones
const char sender_address[ ] = "termodron1@montevideo.com.uy";    // Sender address
const char sender_name[ ] = "termodron1@montevideo.com.uy";       // Sender name

const char to_address[ ] = "agusbarriola@gmail.com";        // Recipient address
const char to_name[ ] = "agusbarriola@gmail.com";           // Recipient name


char data[64];
String subject;
String code_subject;
String body;
char path[50];
String totalemails;
int totalemailsint;
String lasttotalemails = "0";
int8_t answer;
int onModulePin= 8;
int rstModulePin= 9;
int counter;
unsigned long previous;
char SMS[20];
char aux_str[128];
String aux_strg;
int code;
int NPuntos;
/*struct coord{
  double latitud;
  double longitud;
  double altitud;
};
coord coordenadas[10];
*/
const int MISIONMAXPUNTOS = 1;
Punto Mision[MISIONMAXPUNTOS];


Punto coordenadas[1];

bool fly_start = 0;
bool fly_report = 0;
bool fly_end = 0;
Scheduler runner; // se crea el scheduler
uint8_t GPSLock=0;
uint8_t ARM=1;
uint8_t DISARM=0;
uint8_t system_type = MAV_TYPE_QUADROTOR; // 2
uint8_t autopilot_type = MAV_AUTOPILOT_PX4; // 12
uint8_t system_id=254; // Identifica al sistema, 255 es standard para CGS
uint8_t component_id=254; // Identifica al sistema, 255 es standard para CGS
uint8_t base_mode=0;
uint32_t custom_mode=0;
uint8_t system_status=0;
uint8_t stream_id=0;
uint16_t message_rate=0;
uint8_t stop_str = 0;
uint8_t start_str=1;
uint8_t target_system=1;
uint8_t target_component=1;
uint8_t req_stream_id=0;
uint16_t req_message_rate=0;
uint64_t time_usec;
int disable=-1;
uint8_t frame = MAV_FRAME_GLOBAL_RELATIVE_ALT; // 3, x = latitud, y = longitud, z= altitud sobre home
uint16_t command;
uint8_t current=1;
uint8_t autocontinue=1;
int param1;
int param2=-1;
int param3; 
int param4;
float xf;
float yf;
float zf;
uint32_t xi;
uint32_t yi;
uint32_t zi;
uint8_t confirmation=0;
//float periodoGPS=1e6; // Distancia en microsegundos entre dos mensajes de GPS
int timeout =5; // tiempo maximo de espera para los waypoints
uint16_t count = 0; // cantidad de waypoints que tiene una mision
uint16_t seq = 0;
//int chan = 0;
uint32_t tmax = 2000; // timeout para los mensajes


// Punteros

Punto* Trayectoria;

// Callback de las tareas
//void recieveSMSCallback();
//void recieveEMailCallBack();
void heartBeatCallback();
void missionRequestListCallBack();

//Tareas
//Task recieveSMS(60000, TASK_FOREVER, &recieveSMSCallback); // Chequea nuevos sms
//Task recieveEMail(300000, TASK_FOREVER, &recieveEMailCallBack); // Chequea nuevos correos
Task heartBeat(1000, TASK_FOREVER, &heartBeatCallback); // Manda un heartbeat cada un segundo para siempre
Task missionRequestList(500, TASK_FOREVER, &missionRequestListCallBack); // Pide la lista de misiones






#define Consola Serial
#define Shield3G Serial3
#define px4 Serial2
#define sermon Serial


#include "/home/damian/Dropbox/Termodron_Proyecto/Software/GCS_Code/sendATcommand.ino"
#include "/home/damian/Dropbox/Termodron_Proyecto/Software/GCS_Code/borradoEMail.ino"
#include "/home/damian/Dropbox/Termodron_Proyecto/Software/GCS_Code/Shield3G_lib.ino"
//Información del servidor SMTP y cuenta de correo electrónico




void setup() {

  pinMode(onModulePin, OUTPUT);
  pinMode(rstModulePin, OUTPUT);
  Consola.begin(115200);
  Shield3G.begin(19200);   
  Consola.println("Starting...");
  // Se prende el Shield
  power_on();
  // Se setea la SIM
  setsim();

  borradoEMail();
}

void loop() {

}
