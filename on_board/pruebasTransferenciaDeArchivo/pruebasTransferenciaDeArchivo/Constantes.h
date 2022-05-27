#define Consola Serial
#define PX4 Serial3

// Constantes de pruebas
bool unavex=true;
bool unavexx=true;
int periodoGPSmsj = 500;

// Banderas Opciones Email

bool flg_LogMail=false;
bool flg_Img=false;
bool flg_Alertas=false;
bool flg_ReporteFinal=false;
bool flg_Img_available=true;

// Constantes 

// Constantes de pruebas del buffer triestado

const int PXEnablePin = 6;
const int ArdEnablePin = 7;
const int  DATATOGCSSIZE = 251;
uint8_t dataToGCS[DATATOGCSSIZE];



 
//Información del servidor SMTP y cuenta de correo electrónico
const char smtp_server[ ] = "smtp.montevideo.com.uy";          // SMTP server
const char pop3_server[ ] = "mail.montevideo.com.uy";          // POP3 server
const char smtp_user_name[ ] = "termodron1@montevideo.com.uy"; // smtp user name
const char pop3_user_name[ ] = "termodron1";                   // pop3 user name
const char mail_password[ ] = "Termo1";                        // e-mail password
const char smtp_port[ ] = "25";                                // SMTP server port (25, 465 or 587)
const char pop3_port[ ] = "110";                               // POP3 server port (110 or 995)
const int MAXMENSLEN = 100;
const int MAXMENSLENRTCM = 500;
//Información de número de teléfono destino
const char phone_number[]="+59898065912";

//Información de la tarjeta SIM
const char sim_pin[] ="4725"; //"5476";
const char sim_apn[] = "prepago.ancel";             //Servicio prepago
const char sim_user[] = "bam";
const char sim_password[] = "bam";
//const char sim_apn[] = "antel.lte";                //Servicio contrato

//Información de direcciones
const char sender_address[ ] = "termodron1@montevideo.com.uy";    // Sender address
const char sender_name[ ] = "termodron1@montevideo.com.uy";       // Sender name

char MensajeShield3G[MAXMENSLEN];
byte MensajeRTCM[MAXMENSLENRTCM];
int iMENS3G = 0;
int iMENSRTCM = 0;
int data_size_RTCM = 0;
const float distEntrePer = 1e3;
int RTCMLength=0;
#ifdef agustin
const char to_address[ ] = "agusbarriola@gmail.com";        // Recipient address
const char to_name[ ] = "agusbarriola@gmail.com";           // Recipient name
#endif
#ifdef nacho
const char to_address[ ] = "reyesjoseignacio@gmail.com";        // Recipient address
const char to_name[ ] = "reyesjoseignacio@gmail.com";           // Recipient name
#endif
#ifdef damian
const char to_address[ ] = "vallejoyuridamian@gmail.com";        // Recipient address
const char to_name[ ] = "vallejoyuridamian@gmail.com";           // Recipient name
#endif
char data[64];
const PROGMEM int TCPPack = 1460;
char path[50];
char totalemails[4];
int totalemailsint;
int lasttotalemailsint=0;
int8_t answer;
const PROGMEM int onModulePin= 8;
const PROGMEM int rstModulePin= 9;
int counter;
unsigned long previous;
char SMS[20];
const int aux_str_len = 128;
char aux_str[aux_str_len];
String aux_strg;
int Code;
int NPuntos;
int SubjectFlag;
char Subject[50];
char Code_subject[2];
char Body[300];
int iBody;
char SubjectSend[50];   //Nacho

char BodySend[5000];

const float alcance3DR = 1000; //Nacho
const float tray2bat = 10000; //Nacho


const int RESET_PIN = 7;
int aux;
int end_file = 0;
char aux_char;
int http_x;
int http_status; // 0: end, 1 waiting data with header, 2 waiting data without header, 3 timeout waiting data, 4 unknow response
int x = 0;
long espera; 
bool icy = false;

//////////////////////////////// RTCM ///////////////////////////////////////////
// Estados RTCM
const int RTCM_INIT = 0;
const int RTCM_OK = 1;
const int RTCM_CLOSE = 2;
const int RTCM_OPENED = 3;
const int HTTP_OPENED = 4;
int RTCMSTATUS=RTCM_INIT;

// Banderas RTCM
int TCPOpenReqNum = 0;
int TCPCloseReqNum = 0;
int HTTPACTReqNum = 0;
int RTCMReqNum = 0;
int SMTPFILEReqNum = 0;


//////////////////////////////// EMAIL ///////////////////////////////////////////
// Estados POP3
const int POP3_INIT = 0;
const int POP3_OPENED = 1;
const int POP3_GETTING = 2;
const int POP3_CD = 3;
const int POP3_READING = 4;
const int POP3_CLOSE = 5;
const int POP3_STBY = 6;
int POP3STATUS = POP3_STBY; //Debería ser STBY para esperar 1 minuto al principio


int POP3try=0;
int POP3tryMAX=3;


// Estados SMTP
const int SMTP_INIT = 0;
const int SMTP_AUTH = 1;
const int SMTP_FROM = 2;
const int SMTP_RCPT = 3;
const int SMTP_SUB = 4;
const int SMTP_BODY = 5;
const int SMTP_SEND = 6;
const int SMTP_STBY = 7;
const int SMTP_FILE = 8;
int SMTPSTATUS = SMTP_INIT;

// Banderas E-Mail
int POP3INReqNum = 0;
int POP3NUMReqNum = 0;
int POP3CDReqNum = 0;
int POP3GETReqNum = 0;
int POP3READReqNum = 0;
int POP3OUTReqNum = 0;
int SMTPINReqNum = 0;
int SMTPAUTHReqNum = 0;
int SMTPFROMReqNum = 0;
int SMTPRCPTReqNum = 0;
int SMTPSUBReqNum = 0;
int SMTPBODYReqNum = 0;
int SMTPSENDReqNum = 0;

// ReqMax
const int POP3NUMReqMax = 3;
const int POP3CDReqMax = 3;
const int POP3GETReqMax = 3;
const int POP3READReqMax = 3;
const int POP3INReqMax = 3;
const int POP3OUTReqMax = 3;
const int SMTPINReqMax = 3;
const int SMTPAUTHReqMax = 3;
const int SMTPFROMReqMax = 3;
const int SMTPRCPTReqMax = 3;
const int SMTPSUBReqMax = 3;
const int SMTPBODYReqMax = 3;
const int SMTPSENDReqMax = 3;

// Counters
long POP3NUMCounter = 0;
long POP3CDCounter = 0;
long POP3GETCounter = 0;
long POP3READCounter = 0;
long POP3INCounter = 0;
long POP3OUTCounter = 0;
long SMTPINCounter = 0;
long SMTPAUTHCounter = 0;
long SMTPFROMCounter = 0;
long SMTPRCPTCounter = 0;
long SMTPSUBCounter = 0;
long SMTPBODYCounter = 0;
long SMTPSENDCounter = 0;


// Timeouts
const PROGMEM int POP3NUMTimeout=5e3;
const PROGMEM int POP3CDTimeout=5e3;
const PROGMEM int POP3GETTimeout=5e3;
const PROGMEM int POP3READTimeout=5e3;
const PROGMEM int POP3INTimeout=15e3;
const PROGMEM int POP3OUTTimeout=5e3;
const PROGMEM int SMTPINTimeout=5e3;
const PROGMEM int SMTPAUTHTimeout=5e3;
const PROGMEM int SMTPFROMTimeout=5e3;
const PROGMEM int SMTPRCPTTimeout=5e3;
const PROGMEM int SMTPSUBTimeout=5e3;
const PROGMEM int SMTPBODYTimeout=5e3;
const PROGMEM int SMTPSENDTimeout=20e3;


char FileName[]= "imagen.bmp";

const int MISIONMAXPUNTOS = 50;
const int AREAMAXPUNTOS = 10;

// Timeouts
const int GPSTIMEOUT = 10e3;
const int MISSIONREQUESTTIMEOUT=10e3;
const int MISSIONACKTIMEOUT=5e3;


/////////////// XMODEM ////////////////

// Estados XMODEM
const int XMODEM_INIT = 0;
const int XMODEM_DEL= 1;
const int XMODEM_SET_FILE= 2;
const int XMODEM_TRANS= 3;
const int XMODEM_EOT= 4;
const int XMODEM_ETB= 5;
int XMODEMSTATUS = XMODEM_INIT;


// Conatdores XMODEM

int XMODEMINITReqNum=0;
int XMODEMDELReqNum=0;
int XMODEMSETFILEReqNum=0;
int XMODEMTRANSReqNum=0;
int XMODEMEOTReqNum=0;
int XMODEMETBReqNum=0;


// ReqMax XMODEM
const int XMODEMINITReqMax = 3;
const int XMODEMDELReqMax = 3;
const int XMODEMSETFILEReqMax = 3;
const int XMODEMTRANSReqMax = 3;
const int XMODEMEOTReqMax = 3;
const int XMODEMETBReqMax = 3;

// Contadores XMODEM

long XMODEMINITCounter=0;
long XMODEMDELCounter=0;
long XMODEMSETFILECounter=0;
long XMODEMTRANSCounter=0;
long XMODEMEOTCounter=0;
long XMODEMETBCounter=0;

// Timeouts de XMODEM
const PROGMEM int XMODEMINITTimeout=5e3;
const PROGMEM int XMODEMDELTimeout=5e3;
const PROGMEM int XMODEMSETFILETimeout=5e3;
const PROGMEM int XMODEMTRANSTimeout=5e3;
const PROGMEM int XMODEMEOTTimeout=5e3;
const PROGMEM int XMODEMETBTimeout=5e3;


// Constantes y variables Xmodem

char SOH = 0x01;
char EOT = 0x04;
char ETB = 0x17;
char ACK = 0x06;
char NACK = 0x15;
char C_XMOD = 0x43;
char Xmodem_count=0x01;
char Xmodem_count_A1=0xFE;
const int XmodemDataSize=128;
char XmodemData[XmodemDataSize];
const int XmodemCRCSize=2;
char XmodemCRC[XmodemCRCSize];
int Xmodem_CRC;
char ShieldByte;
int CursorXmodem=0;
char PADDING_CHAR= 0x00;//0x1A;
bool flg_Xmodem_ultimodato=false;
bool flg_Xmodem_enviando=false;
///////////////////////////// PX4 //////////////////////////


//////// ESTADOS DEL DRON //////////////

const int PIXHAWK_STANDBY = 0;
const int PIXHAWK_MISION = 1;
const int PIXHAWK_LOITERING = 2;
const int PIXHAWK_SIGUIENDO = 3;
const int PIXHAWK_VOLVIENDO = 4;
const int PIXHAWK_ATERRIZADO = 5;
const int PIXHAWK_ESQUIVANDO = 6;

int PIXHAWK_STATE= PIXHAWK_STANDBY; 

// Estado extendido de MAVLINK //


uint8_t Pixhawk_landed_state = MAV_LANDED_STATE_UNDEFINED;
uint8_t Pixhawk_landed_state_previous = MAV_LANDED_STATE_UNDEFINED;
bool flg_TakeOffDetected=false;
bool flg_LandingDetected=false;

//////// FLAGS DE ALARMAS ////////////

bool inicio_mision_alarm = false;
bool cuerpo_encontrado_alarm= false;
bool aterrizado_alarm= false;
bool bateria_baja_alarm= false;
bool bateria_critica_alarm= false;
bool fin_mision_alarm= false;
bool obstaculo_encontrado_alarm= false;


//// Otras banderas para enviar mails ////
bool sendAlerta=false;
bool sendReporteFinal=false;
bool flg_has_att=false;


// variables del estado del dron y constantes MAVLINK PIXHAWK//

int8_t carga_bateria; 
int sendwaypointreq = 0;
int sendwaypointmax = 3;

float yaw=-888;
float pitch=-888;
float roll=-888;

boolean received = false;
boolean configured = false;

int32_t latitud_int_1E7 = -888888888;
int32_t longitud_int_1E7 = -888888888;
int32_t altitud_int = 50;


int32_t latitud_home_int_1E7 = -888888888;
int32_t longitud_home_int_1E7 = -888888888;
int32_t altitud_home_int_mm = 5000;

uint32_t Timestamp_boot_PX=-888;
uint64_t time_usec = 0;
uint8_t statusTextSeverity = 0;
const int statusTextTextLenMax = 50;
int statusTextTextLen = 50;
char statusTextText[statusTextTextLenMax];

uint8_t result_command_ack;
bool fly_start = 0;
bool fly_report = 0;
bool fly_end = 0;
Scheduler runner; // se crea el scheduler
uint8_t GPSLock=0;
uint8_t ARM=1;
uint8_t DISARM=0;
const PROGMEM uint8_t system_type = MAV_TYPE_QUADROTOR; // 2
const PROGMEM uint8_t autopilot_type = MAV_AUTOPILOT_PX4; // 12
const PROGMEM uint8_t system_id=69; // Identifica al sistema, 255 es standard para CGS
const PROGMEM uint8_t component_id=69; // Identifica al sistema, 255 es standard para CGS
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
int disable=-1;
int mission_send_timeout=0;
int mission_send_max=5;
int seqprevious=-1;
int seqactual=0;
#ifdef CoordenadasEnFloat
uint8_t frame = MAV_FRAME_GLOBAL_RELATIVE_ALT; // 3, x = latitud, y = longitud, z= altitud sobre home
#else
uint8_t frame = MAV_FRAME_GLOBAL_RELATIVE_ALT_INT; // 6, x = latitud, y = longitud, z= altitud sobre home
#endif
uint16_t command;
uint8_t current=1;
uint8_t autocontinue=1;
int param1;
int param2=-1;
int param3; 
int param4;
int param5;
int param6;
int param7;
float xf;
float yf;
float zf;
int32_t xi;
int32_t yi;
int32_t zi;
uint8_t confirmation=0;
int timeout =5; // tiempo maximo de espera para los waypoints
uint16_t count = 0; // cantidad de waypoints que tiene una mision
uint16_t seq = 0;
uint8_t flagRTCM = 0; 


//variables relativas a la imagen //Nacho
//Nacho
int cursorRawThermal_fila = 0;
int cursorRawThermal_columna = 0;

int r = 0; //cantidad de objetos            //Nacho
int s = 0; //cantidad de elementos en el objeto mayor         //Nacho
int sensor_dist[4] = {0, 0, 0, 0};         //Nacho
bool sensor_act[4] = {false, false, false, false};         //Nacho
char reset_onboard_error[15];         //Nacho

uint8_t serial_control_count;         //Nacho
uint8_t serial_control_data[70];         //Nacho


/////////// MODOS MISION ///////////////

byte MISSION_MODE_FOLLOW = 0x00;
byte MISSION_MODE_RTL = 0x01;
byte MISSION_MODE_CONT = 0x02;
byte MISSION_MODE = MISSION_MODE_FOLLOW;

const byte ONBOARDHEADER = 0x80;




////////////////////////////////Constantes para BMP (Imagen) //////////////////////////////

int tmax = 40;
int tmin = 0;
int tp = (tmax+tmin)/2;
int temp;
const int ImagenBMPSize=14454;
char ImagenBMP[ImagenBMPSize];


////////////////////////////////TAREAS///////////////////////

// Callback de las tareas
void heartBeatCallback();
void checkEMailCallBack();
void sendEMailCallBack();
void EMailReaderCallBack();
void RTCMCallBack();
void Shield3GReaderCallBack();
void RTCMReaderCallBack();
void PX4ReaderCallBack();
void EnviarReportesCallBack();
void XmodemCallBack();
void XmodemReaderCallBack();

// Periodos de las tareas
const int PERIODOHEARTBEAT = 1e3;
const int PERIODOEMAIL = 10e3;
const int PERIODOEMAILREADER = 1e3;
const int PERIODORTCM = 10e3;
const int PERIODO3GREADER=1;
const int PERIODORTCMREADER=1;
const int PERIODOPX4READER=1;
const int PERIODOXMODEM=1;
const int PERIODOXMODEMREADER=1;

//Tareas
Task heartBeat(PERIODOHEARTBEAT, TASK_FOREVER, &heartBeatCallback); // Manda un heartbeat cada un segundo para siempre
Task PX4Reader(PERIODOPX4READER,TASK_FOREVER,&PX4ReaderCallBack);
