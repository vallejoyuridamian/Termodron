#define Consola Serial
#define PX4 Serial2
#define Shield3G Serial3
#define powerPin 5
#define pinShieldVivo 3
#define pinAntenaViva 4

// Constantes de pruebas
bool unavex=true;
bool unavexx=true;
int periodoGPSmsj = 500;

// Banderas Opciones Email

bool flg_LogMail=false;
bool flg_Img=false;
bool flg_Alertas=false;
bool flg_ReporteFinal=false;
bool flg_Img_available=false;
bool haveToSetHome =false;
bool bsendNACK=false;
bool apurarEstadoExtendido=true;
// Constantes de los leds

int bytesLeidosDeAntena = 0;
int bytesLeidosDeShield = 0;
const int BYTESCONMUTAR = 5;

// Constantes de la imagen enviada por la on-board

int columnavieja=0;
int contadorColumna=0;
const int IMG_ACK=1;
const int IMG_NACK=2;
const int IMG_INDEFINIDO=0;
int IMG_ACK_STATUS = IMG_INDEFINIDO;
const int DATAONBOARDSIZE = 251;
const int FOTO_REQUEST_NO = 0;
const int FOTO_REQUEST_SI = 1;
int FOTO_REQUEST = FOTO_REQUEST_SI;

const int TIPO_MISION_FOLLOW = 0;
const int TIPO_MISION_RTL = 1;
const int TIPO_MISION_CONT = 2;
int TIPO_MISION = TIPO_MISION_FOLLOW;
const int VminCell=3200;
int LOG_ENABLE = 0;

uint8_t dataToOnBoard[DATAONBOARDSIZE];
// Transferencia de archivos entre el la placa on-board y la GCS

const int DATANOVALID  = 9; // son los bytes que sobran al mandar la imagen 
uint8_t data_on_board[DATAONBOARDSIZE];

// constantes de los pines
bool PX4AVAILABLE = false;
bool SHIELD3G = false;
 
//Información del servidor SMTP y cuenta de correo electrónico
const char smtp_server[ ] = "smtp.montevideo.com.uy";          // SMTP server
const char pop3_server[ ] = "mail.montevideo.com.uy";          // POP3 server
const char smtp_user_name[ ] = "termodron1@montevideo.com.uy"; // smtp user name
const char pop3_user_name[ ] = "termodron1";                   // pop3 user name
const char mail_password[ ] = "Termo1";                        // e-mail password
const char smtp_port[ ] = "25";                                // SMTP server port (25, 465 or 587)
const char pop3_port[ ] = "110";                               // POP3 server port (110 or 995)

const int MAXMENSLEN = 100;
#ifdef RTCMdef
const int MAXMENSLENRTCM = 500;
byte MensajeRTCM[MAXMENSLENRTCM];
#endif
//Información de número de teléfono destino
const char phone_number[]="+59898065912";

//Información de la tarjeta SIM
const char sim_pin[] ="4725"; //"5476";
const char sim_apn[] = "prepago.ancel";             //Servicio prepago
const char sim_user[] = "bam";
const char sim_password[] = "bam";
//const char sim_apn[] = "antel.lte";                //Servicio contrato

//Información de direcciones
const int MAILLENGTH = 50;
char sender_address[MAILLENGTH] = "termodron1@montevideo.com.uy";    // Sender address
char sender_name[MAILLENGTH] = "termodron1@montevideo.com.uy";       // Sender name

const float ALTITUDADISTANCIAENTREPER = 1.1;
char MensajeShield3G[MAXMENSLEN];
int iMENS3G = 0;
int iMENSRTCM = 0;
int data_size_RTCM = 0;
float distEntrePer = 3; // metros entre las paralelas del zigz-ag
int RTCMLength=0;
#ifdef agustin
char to_address[MAILLENGTH] = "agusbarriola@gmail.com";        // Recipient address
char to_name[MAILLENGTH] = "agusbarriola@gmail.com";           // Recipient name
#endif
#ifdef nacho
char to_address[MAILLENGTH] = "reyesjoseignacio@gmail.com";        // Recipient address
char to_name[MAILLENGTH] = "reyesjoseignacio@gmail.com";           // Recipient name
#endif
#ifdef damian
char to_address[MAILLENGTH] = "vallejoyuridamian@gmail.com";        // Recipient address
char to_name[MAILLENGTH] = "vallejoyuridamian@gmail.com";           // Recipient name
#endif
char data[64];
const int TCPPack = 1460;
char path[50];
char totalemails[4];
int totalemailsint;
int lasttotalemailsint=0;
int8_t answer;
const int onModulePin= 8;
const int rstModulePin= 9;
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
const int SubjectSize = 50;
char SubjectSend[50];
const int BodySize = 4000;
char BodySend[BodySize];

const float alcance3DR = 1000; //Nacho
const float tray2bat = 10000; //Nacho


uint8_t tiempo_hora = 0;
uint8_t tiempo_minuto = 0;
uint8_t tiempo_segundo = 0;
uint8_t tiempo_AM = 0; // si es 1 es AM
uint8_t tiempo_dia = 0;
uint8_t tiempo_mes = 0;
uint8_t tiempo_anio = 0;


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

// ReqMax
const int RTCMReqMax = 3;
const int HTTPACTReqMax = 3;
const int TCPCloseReqMax = 3;
const int TCPOpenReqMax = 3;
const int SMTPFILEReqMax = 3;

bool RCTM_NA= false;

// Counters
long TCPOpenCounter = 0;
long HTTPACTCounter = 0;
long RTCMReqCounter = 0;
long RTCM_NACounter = 0;
long TCPCloseCounter= 0;
long SMTPFILECounter= 0;
// Timeouts
const PROGMEM int TIMEOUTRTCM=10e3;
const PROGMEM int TCPOpenTimeout=10e3;
const PROGMEM int TCPCloseTimeout=5e3;
const PROGMEM int HTTPACTTimeout=5e3;
const PROGMEM int RTCMReqTimeout=5e3;
const PROGMEM int RTCM_NATimeout = 120e3;
const PROGMEM int SMTPFILETimeout = 5e3;
// Estados RTCMREADER
const int RTCMREADER_INIT=1;
int RTCMREADERSTATUS=RTCMREADER_INIT;

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

int POP3try=1;
int SMTPtry=1;

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
Punto Mision[MISIONMAXPUNTOS];
Punto coordenadas[AREAMAXPUNTOS];

// Timeouts
const int GPSTIMEOUT = 20e3;
const int MISSIONREQUESTTIMEOUT=10e3;
const int MISSIONACKTIMEOUT=10e3;


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

const int ARMED_STATUS_ARMED= 0;
const int ARMED_STATUS_DISARMED= 1;
const int ARMED_STATUS_UNKNOWN= 2;

int ARMED_STATUS = ARMED_STATUS_UNKNOWN;

float PERIODO_MAVLINK_MSG_ID_EXTENDED_SYS_STATE = 1e5;

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

int8_t carga_bateria = -8; 
int sendwaypointreq = 0;
int sendwaypointmax = 3;

float yaw=-888;
float pitch=-888;
float roll=-888;

boolean received = false;
boolean configured = false;

int32_t latitud_int_1E7 = -349170931;
int32_t longitud_int_1E7 = -561668561;
int32_t altitud_int = -8;

//int32_t latitud_int_1E7_foto = -888888888;
//int32_t longitud_int_1E7_foto = -888888888;

int32_t latitud_int_1E7_foto = -349170931;
int32_t longitud_int_1E7_foto = -561668561;

int32_t latitud_home_int_1E7 = -349170931;
int32_t longitud_home_int_1E7 = -561668561;
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
const PROGMEM uint8_t system_id=254; // Identifica al sistema, 255 es standard para CGS
const PROGMEM uint8_t component_id=254; // Identifica al sistema, 255 es standard para CGS
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


const int INPUT_STATUS_MISSION = 0;
const int INPUT_STATUS_MANUAL = 1;
const int INPUT_STATUS_UNKNOWN = 2;  
int INPUT_STATUS = INPUT_STATUS_UNKNOWN;          

Punto homepos[1];

//variables relativas a la imagen 
const int image_x  = 60;              
const int image_y = 80;              
int thermal_image[image_x][image_y]=
{{8018,  8013,  8013,  8013,  8012,  8011,  8008,  8007,  8010,  8019,  8011,  8022,  8095,  8152,  8174,  8185,  8186,  8186,  8181,  8175,  8153,  8139,  8146,  8184,  8191,  8191,  8199,  8194,  8198,  8200,  8203,  8220,  8240,  8267,  8274,  8249,  8258,  8246,  8236,  8200,  8144,  8121,  8079,  8049,  8033,  8017,  8004,  8005,  8002,  8002,  7999,  7997,  7999,  7995,  7994,  7998,  8001,  8013,  8019,  8018,  8017,  8014,  8017,  8023,  8024,  8019,  8014,  8011,  8004,  7998,  8295,  7991,  7999,  8001,  8003,  8002,  7999,  8002,  7995,  7995},
{8017,  8012,  8020,  8023,  8020,  8017,  8013,  8009,  8013,  8015,  8015,  8025,  8112,  8152,  8171,  8183,  8181,  8188,  8188,  8190,  8176,  8156,  8154,  8186,  8186,  8188,  8198,  8199,  8198,  8200,  8208,  8217,  8248,  8238,  8283,  8268,  8224,  8171,  8159,  8146,  8142,  8110,  8075,  8052,  8032,  8017,  8002,  8000,  7999,  8001,  8001,  7998,  7996,  7995,  7996,  7994,  7997,  8009,  8017,  8014,  8013,  8013,  8016,  8022,  8022,  8017,  8015,  8008,  8004,  8016,  8303,  7996,  7996,  7999,  7998,  7998,  7990,  7991,  7988,  7994},
{8029,  8036,  8072,  8087,  8086,  8062,  8045,  8019,  8017,  8023,  8017,  8038,  8129,  8163,  8177,  8185,  8183,  8190,  8187,  8192,  8196,  8161,  8168,  8190,  8198,  8194,  8193,  8200,  8194,  8206,  8221,  8218,  8244,  8229,  8206,  8247,  8250,  8180,  8149,  8170,  8188,  8128,  8071,  8051,  8028,  8012,  8006,  8005,  8002,  8003,  8000,  7998,  7999,  7998,  7997,  7997,  8001,  8015,  8021,  8016,  8013,  8014,  8015,  8026,  8025,  8019,  8012,  8009,  8000,  7993,  8292,  7998,  7995,  7993,  7995,  7994,  7989,  7990,  7990,  7995},
{8105,  8168,  8207,  8208,  8171,  8142,  8119,  8071,  8024,  8026,  8021,  8039,  8130,  8167,  8180,  8192,  8184,  8172,  8181,  8205,  8211,  8184,  8178,  8201,  8211,  8199,  8196,  8197,  8199,  8218,  8227,  8214,  8247,  8241,  8199,  8205,  8222,  8210,  8160,  8149,  8179,  8141,  8065,  8047,  8028,  8006,  8004,  8002,  8000,  7998,  8003,  8000,  8003,  8001,  8001,  8000,  8003,  8015,  8022,  8017,  8013,  8015,  8021,  8027,  8025,  8017,  8009,  8008,  8004,  7986,  8287,  7992,  7994,  7995,  8001,  7996,  8005,  8004,  8000,  8010},
{8190,  8252,  8281,  8280,  8269,  8212,  8168,  8130,  8081,  8037,  8022,  8038,  8130,  8175,  8188,  8188,  8168,  8173,  8189,  8222,  8222,  8205,  8182,  8197,  8215,  8208,  8198,  8202,  8205,  8218,  8234,  8216,  8237,  8239,  8209,  8234,  8249,  8257,  8215,  8193,  8131,  8107,  8053,  8032,  8017,  8007,  8006,  8006,  8005,  8004,  8005,  8004,  8008,  8005,  8004,  8003,  8004,  8022,  8022,  8019,  8021,  8017,  8021,  8027,  8027,  8014,  8008,  8005,  8001,  8020,  8295,  8007,  8008,  8005,  8012,  8015,  8021,  8027,  8023,  8026},
{8234,  8271,  8295,  8294,  8298,  8276,  8220,  8169,  8126,  8064,  8029,  8034,  8122,  8175,  8184,  8169,  8168,  8187,  8204,  8229,  8234,  8227,  8189,  8195,  8213,  8207,  8202,  8203,  8199,  8214,  8232,  8223,  8222,  8239,  8217,  8227,  8225,  8223,  8211,  8200,  8155,  8088,  8038,  8016,  8009,  8005,  8005,  8005,  8006,  8005,  8006,  8007,  8008,  8005,  8001,  8004,  8006,  8022,  8022,  8019,  8019,  8015,  8022,  8028,  8025,  8015,  8013,  8010,  8013,  8020,  8303,  8020,  8024,  8025,  8033,  8032,  8037,  8036,  8026,  8034},
{8258,  8291,  8305,  8303,  8308,  8303,  8279,  8208,  8159,  8104,  8041,  8034,  8107,  8170,  8170,  8167,  8176,  8190,  8208,  8238,  8242,  8241,  8206,  8211,  8210,  8212,  8206,  8201,  8194,  8203,  8232,  8231,  8239,  8272,  8228,  8209,  8206,  8215,  8199,  8174,  8151,  8105,  8027,  8014,  8009,  8007,  8007,  8007,  8003,  8004,  8010,  8008,  8011,  8009,  8005,  8002,  8008,  8022,  8016,  8015,  8015,  8016,  8028,  8030,  8026,  8019,  8018,  8020,  8025,  8036,  8309,  8027,  8040,  8032,  8038,  8033,  8035,  8043,  8037,  8037},
{8275,  8300,  8309,  8306,  8320,  8326,  8305,  8239,  8189,  8137,  8063,  8045,  8104,  8172,  8167,  8156,  8177,  8189,  8213,  8245,  8253,  8234,  8220,  8217,  8243,  8233,  8207,  8199,  8195,  8192,  8225,  8238,  8235,  8301,  8293,  8235,  8201,  8190,  8181,  8172,  8203,  8132,  8024,  8014,  8012,  8008,  8009,  8011,  8003,  8007,  8008,  8010,  8009,  8009,  8007,  8002,  8011,  8020,  8017,  8012,  8014,  8014,  8025,  8034,  8032,  8027,  8031,  8038,  8043,  8040,  8308,  8041,  8042,  8033,  8040,  8042,  8042,  8048,  8044,  8046},
{8277,  8309,  8311,  8311,  8319,  8330,  8326,  8262,  8198,  8175,  8156,  8093,  8114,  8170,  8164,  8152,  8187,  8202,  8224,  8263,  8252,  8240,  8230,  8224,  8253,  8282,  8228,  8200,  8192,  8187,  8221,  8251,  8223,  8282,  8269,  8289,  8305,  8238,  8188,  8164,  8176,  8115,  8023,  8017,  8013,  8012,  8009,  8010,  8007,  8007,  8008,  8006,  8005,  8013,  8008,  8008,  8010,  8023,  8019,  8014,  8017,  8019,  8030,  8036,  8038,  8039,  8038,  8039,  8034,  8050,  8313,  8040,  8035,  8034,  8039,  8042,  8044,  8053,  8044,  8051},
{8259,  8311,  8312,  8309,  8315,  8333,  8333,  8279,  8203,  8197,  8215,  8174,  8134,  8185,  8156,  8157,  8202,  8228,  8253,  8265,  8237,  8257,  8241,  8215,  8256,  8307,  8282,  8216,  8198,  8189,  8218,  8272,  8235,  8266,  8278,  8261,  8271,  8259,  8245,  8234,  8150,  8087,  8017,  8013,  8008,  8008,  8008,  8010,  8005,  8007,  8006,  8006,  8008,  8009,  8005,  8008,  8013,  8025,  8024,  8021,  8023,  8031,  8034,  8036,  8032,  8034,  8036,  8030,  8031,  8028,  8312,  8030,  8036,  8034,  8036,  8037,  8044,  8050,  8048,  8043},
{8248,  8308,  8316,  8315,  8317,  8334,  8334,  8305,  8225,  8211,  8227,  8211,  8264,  8224,  8178,  8210,  8226,  8232,  8255,  8239,  8250,  8288,  8234,  8219,  8244,  8269,  8267,  8253,  8205,  8196,  8221,  8294,  8266,  8272,  8313,  8338,  8282,  8295,  8273,  8264,  8245,  8096,  8018,  8009,  8010,  8006,  8011,  8011,  8007,  8006,  8006,  8006,  8007,  8010,  8007,  8007,  8013,  8023,  8024,  8024,  8029,  8031,  8030,  8030,  8031,  8028,  8031,  8031,  8027,  8019,  8307,  8037,  8031,  8031,  8035,  8028,  8037,  8042,  8042,  8044},
{8225,  8309,  8318,  8314,  8311,  8330,  8337,  8328,  8249,  8235,  8219,  8258,  8324,  8262,  8205,  8238,  8235,  8233,  8242,  8234,  8272,  8296,  8272,  8218,  8225,  8237,  8260,  8288,  8231,  8203,  8220,  8296,  8290,  8262,  8296,  8342,  8342,  8285,  8289,  8275,  8261,  8125,  8024,  8021,  8015,  8014,  8013,  8013,  8008,  8003,  8002,  8005,  8004,  8007,  8009,  8010,  8012,  8025,  8023,  8018,  8024,  8027,  8030,  8030,  8031,  8029,  8031,  8032,  8027,  8033,  8298,  8031,  8031,  8027,  8033,  8028,  8031,  8035,  8031,  8038},
{8186,  8300,  8321,  8313,  8307,  8318,  8332,  8324,  8257,  8245,  8218,  8289,  8332,  8265,  8221,  8243,  8237,  8231,  8229,  8267,  8297,  8300,  8255,  8216,  8215,  8225,  8255,  8304,  8289,  8223,  8229,  8305,  8312,  8253,  8269,  8309,  8331,  8304,  8249,  8265,  8230,  8069,  8021,  8016,  8016,  8012,  8011,  8012,  8000,  7999,  8002,  8000,  8002,  8004,  8010,  8010,  8009,  8022,  8017,  8009,  8014,  8027,  8039,  8038,  8035,  8036,  8036,  8035,  8032,  8038,  8301,  8023,  8027,  8027,  8031,  8027,  8026,  8027,  8023,  8033},
{8133,  8277,  8314,  8316,  8305,  8304,  8319,  8314,  8277,  8239,  8262,  8312,  8309,  8246,  8217,  8245,  8237,  8226,  8253,  8284,  8271,  8255,  8233,  8220,  8227,  8223,  8231,  8281,  8299,  8254,  8253,  8275,  8309,  8246,  8300,  8258,  8320,  8326,  8262,  8252,  8171,  8034,  8020,  8019,  8014,  8013,  8010,  8010,  8005,  8000,  7998,  7996,  8000,  8000,  8010,  8012,  8014,  8020,  8013,  8009,  8005,  8014,  8042,  8047,  8045,  8040,  8043,  8041,  8029,  8028,  8302,  8031,  8027,  8023,  8026,  8028,  8023,  8026,  8022,  8027},
{8077,  8216,  8302,  8309,  8308,  8297,  8305,  8306,  8289,  8246,  8293,  8303,  8267,  8230,  8210,  8268,  8251,  8231,  8286,  8286,  8292,  8245,  8226,  8219,  8237,  8235,  8248,  8252,  8278,  8277,  8258,  8260,  8313,  8242,  8275,  8278,  8278,  8326,  8283,  8246,  8111,  8024,  8018,  8013,  8015,  8014,  8010,  8011,  8004,  8003,  7999,  7997,  7998,  7999,  8008,  8012,  8016,  8018,  8011,  8006,  8005,  8003,  8028,  8048,  8050,  8049,  8051,  8043,  8035,  8040,  8298,  8019,  8024,  8024,  8028,  8022,  8023,  8024,  8018,  8021},
{8044,  8123,  8263,  8298,  8302,  8299,  8291,  8294,  8295,  8298,  8312,  8293,  8248,  8220,  8214,  8290,  8282,  8262,  8301,  8295,  8281,  8251,  8208,  8283,  8241,  8225,  8251,  8270,  8262,  8273,  8253,  8282,  8285,  8281,  8254,  8268,  8217,  8288,  8278,  8229,  8061,  8017,  8012,  8009,  8011,  8010,  8005,  8010,  8008,  8005,  8006,  7999,  7997,  8000,  8006,  8011,  8014,  8012,  8005,  8004,  8003,  8000,  8009,  8040,  8050,  8053,  8054,  8048,  8037,  8031,  8305,  8017,  8027,  8023,  8027,  8022,  8021,  8021,  8010,  8012},
{8033,  8055,  8169,  8270,  8293,  8290,  8283,  8285,  8294,  8305,  8296,  8247,  8236,  8217,  8225,  8307,  8294,  8285,  8305,  8312,  8287,  8228,  8296,  8382,  8254,  8198,  8217,  8269,  8270,  8277,  8251,  8314,  8274,  8284,  8243,  8247,  8209,  8205,  8259,  8166,  8025,  8014,  8012,  8010,  8011,  8012,  8009,  8006,  8007,  8005,  8000,  7996,  7998,  7998,  8007,  8012,  8015,  8012,  8001,  8003,  8002,  8001,  8006,  8015,  8038,  8051,  8054,  8050,  8042,  8039,  8302,  8039,  8026,  8021,  8021,  8016,  8007,  8005,  7995,  8002},
{8032,  8036,  8084,  8196,  8271,  8282,  8277,  8275,  8278,  8293,  8274,  8260,  8253,  8221,  8254,  8319,  8296,  8311,  8335,  8301,  8282,  8253,  8403,  8407,  8314,  8193,  8194,  8264,  8271,  8305,  8257,  8308,  8323,  8270,  8226,  8229,  8213,  8168,  8168,  8071,  8018,  8014,  8010,  8010,  8010,  8009,  8009,  8006,  8001,  7999,  8001,  7999,  8000,  8000,  8004,  8016,  8013,  8008,  8002,  8001,  7999,  8001,  8000,  7999,  8013,  8036,  8043,  8045,  8037,  8037,  8293,  8022,  8019,  8007,  8009,  8001,  8000,  8001,  7997,  8001},
{8029,  8028,  8045,  8095,  8222,  8264,  8273,  8264,  8269,  8281,  8311,  8288,  8246,  8233,  8261,  8321,  8272,  8328,  8347,  8302,  8262,  8365,  8415,  8417,  8411,  8301,  8177,  8229,  8277,  8290,  8306,  8278,  8348,  8261,  8207,  8205,  8193,  8169,  8095,  8020,  8008,  8010,  8005,  8007,  8006,  8007,  8004,  7999,  7996,  7997,  8001,  8002,  7998,  7996,  8000,  8012,  8011,  8008,  8001,  7999,  7999,  8001,  8001,  8002,  8001,  8014,  8030,  8029,  8020,  8017,  8269,  8002,  8003,  8001,  8005,  8000,  7997,  8003,  7995,  8002},
{8027,  8025,  8029,  8045,  8132,  8233,  8260,  8262,  8266,  8283,  8289,  8295,  8270,  8264,  8263,  8311,  8271,  8331,  8325,  8313,  8286,  8411,  8399,  8416,  8419,  8405,  8301,  8201,  8253,  8291,  8330,  8267,  8325,  8284,  8188,  8196,  8166,  8063,  8013,  8008,  8004,  8005,  8001,  8004,  8002,  7999,  8000,  8001,  7996,  7999,  7999,  7999,  7999,  7998,  8001,  8011,  8010,  8005,  8002,  8000,  8001,  8002,  8001,  8005,  7998,  8002,  8009,  8006,  8002,  8010,  8264,  7995,  8001,  7999,  8007,  7999,  7997,  7999,  7997,  7995},
{8023,  8017,  8023,  8025,  8061,  8176,  8248,  8261,  8269,  8278,  8283,  8302,  8274,  8287,  8272,  8309,  8281,  8337,  8312,  8317,  8335,  8406,  8400,  8424,  8419,  8407,  8399,  8267,  8213,  8268,  8321,  8279,  8260,  8308,  8186,  8124,  8050,  8007,  8000,  8003,  8002,  8002,  8002,  8001,  8001,  8000,  8002,  8001,  8003,  8001,  7999,  8000,  7997,  8001,  8000,  8011,  8014,  8004,  8002,  8002,  8001,  8004,  8004,  8005,  7999,  8001,  8002,  8005,  7998,  8007,  8275,  7999,  7999,  7999,  8003,  8000,  7998,  7998,  7997,  7998},
{8016,  8018,  8024,  8026,  8045,  8101,  8221,  8260,  8272,  8279,  8278,  8276,  8274,  8280,  8264,  8285,  8283,  8320,  8296,  8310,  8369,  8402,  8410,  8421,  8406,  8393,  8393,  8369,  8219,  8215,  8298,  8287,  8203,  8228,  8128,  8019,  8009,  8000,  8003,  8001,  8001,  8002,  7999,  8000,  8001,  8001,  7999,  7997,  7999,  8001,  8001,  8000,  8000,  8004,  8000,  8008,  8011,  8006,  8003,  8003,  8005,  8001,  8002,  8000,  8000,  8001,  8003,  8003,  7998,  7995,  8256,  8005,  7996,  7996,  8006,  8003,  8001,  7997,  7999,  8000},
{8021,  8024,  8031,  8033,  8039,  8061,  8164,  8255,  8272,  8280,  8278,  8273,  8276,  8272,  8259,  8265,  8265,  8300,  8263,  8297,  8378,  8399,  8416,  8410,  8391,  8383,  8389,  8382,  8290,  8214,  8260,  8236,  8080,  8040,  8021,  8005,  8001,  7999,  8000,  8000,  8002,  7997,  7997,  7998,  8001,  8000,  8002,  7998,  8002,  8003,  8004,  8000,  8004,  8005,  8001,  8006,  8009,  8007,  8002,  8003,  8002,  8002,  8006,  8001,  7997,  8001,  8003,  8004,  8002,  8013,  8243,  8002,  8000,  7998,  8004,  7998,  7997,  7998,  7993,  7996},
{8033,  8027,  8034,  8034,  8035,  8045,  8094,  8231,  8270,  8283,  8282,  8275,  8271,  8264,  8263,  8243,  8222,  8228,  8236,  8271,  8363,  8383,  8388,  8380,  8374,  8376,  8380,  8376,  8334,  8172,  8133,  8052,  8021,  8007,  8002,  8006,  8002,  8000,  8001,  8001,  8001,  7997,  7996,  7999,  8001,  8000,  8001,  8001,  8000,  8002,  8004,  8005,  8003,  8006,  8001,  8009,  8007,  8008,  8004,  8003,  8004,  8004,  8002,  8002,  8003,  8003,  8002,  8006,  8002,  8009,  8255,  8009,  8004,  7999,  8001,  8000,  7997,  7999,  7997,  8001},
{8032,  8028,  8032,  8032,  8036,  8039,  8051,  8159,  8261,  8284,  8283,  8273,  8268,  8267,  8265,  8237,  8180,  8147,  8161,  8169,  8342,  8376,  8398,  8397,  8387,  8383,  8376,  8368,  8329,  8094,  8029,  8014,  8010,  8003,  8006,  8006,  8000,  7996,  8000,  7999,  8000,  7997,  7996,  7999,  7998,  8001,  8001,  8003,  7999,  8000,  8004,  8001,  8002,  8004,  8004,  8007,  8008,  8005,  8000,  8003,  8004,  8002,  8002,  8002,  8000,  8003,  8003,  8005,  7999,  8006,  8257,  8010,  8003,  8003,  8004,  7998,  7999,  8002,  7996,  7999},
{8035,  8032,  8034,  8036,  8038,  8036,  8036,  8075,  8217,  8282,  8287,  8282,  8272,  8269,  8271,  8264,  8251,  8250,  8242,  8248,  8259,  8280,  8331,  8406,  8401,  8382,  8357,  8343,  8327,  8121,  8023,  8011,  8011,  8003,  8002,  8003,  8002,  7998,  7999,  8001,  8000,  7999,  7999,  7998,  8001,  8000,  8001,  8003,  8002,  8002,  8002,  8003,  8005,  8005,  8005,  8009,  8010,  8005,  8001,  8003,  8004,  8005,  8006,  8005,  8001,  8001,  8002,  8003,  8001,  8008,  8244,  8007,  8006,  7997,  8003,  8000,  8001,  7998,  7995,  7999},
{8035,  8032,  8037,  8035,  8037,  8035,  8034,  8047,  8154,  8273,  8293,  8290,  8283,  8276,  8275,  8266,  8267,  8266,  8263,  8306,  8318,  8309,  8333,  8393,  8399,  8389,  8346,  8319,  8320,  8238,  8051,  8010,  8009,  8003,  8002,  8003,  8000,  7999,  8001,  7999,  8001,  8002,  7999,  7999,  8001,  7999,  8001,  8004,  8001,  8003,  8005,  8007,  8010,  8010,  8010,  8011,  8014,  8011,  8006,  8004,  8003,  8005,  8005,  8004,  8001,  8001,  8003,  8003,  7999,  8001,  8258,  8010,  7999,  7996,  8001,  8000,  8000,  7998,  7995,  7999},
{8033,  8031,  8038,  8031,  8033,  8031,  8029,  8036,  8090,  8249,  8284,  8285,  8284,  8283,  8280,  8273,  8266,  8259,  8258,  8339,  8389,  8392,  8392,  8399,  8395,  8384,  8345,  8318,  8315,  8276,  8079,  8018,  8009,  7999,  8000,  8000,  8000,  8000,  8000,  8000,  8004,  7998,  8000,  8001,  8004,  8003,  8003,  8008,  8011,  8015,  8019,  8019,  8016,  8017,  8018,  8018,  8009,  8007,  8005,  8006,  8005,  8005,  8006,  8007,  8003,  8002,  8005,  8005,  8000,  8004,  8256,  8011,  7999,  7993,  8003,  8002,  8001,  7999,  7991,  7992},
{8029,  8027,  8030,  8031,  8029,  8033,  8032,  8034,  8063,  8208,  8272,  8280,  8281,  8277,  8274,  8275,  8267,  8256,  8254,  8341,  8381,  8391,  8395,  8386,  8392,  8383,  8332,  8317,  8319,  8305,  8186,  8043,  8013,  8003,  8004,  8006,  8000,  8001,  8003,  8006,  8010,  8014,  8013,  8016,  8018,  8018,  8013,  8017,  8017,  8016,  8021,  8023,  8021,  8022,  8020,  8016,  8013,  8010,  8007,  8009,  8012,  8011,  8009,  8007,  8008,  8004,  8007,  8012,  8011,  8012,  8266,  8017,  8003,  8000,  8006,  8003,  8005,  7996,  7982,  7989},
{8033,  8028,  8031,  8030,  8031,  8032,  8031,  8034,  8050,  8166,  8260,  8272,  8271,  8267,  8267,  8267,  8260,  8251,  8258,  8340,  8372,  8379,  8353,  8352,  8383,  8384,  8340,  8320,  8325,  8309,  8257,  8126,  8021,  8008,  8011,  8014,  8008,  8011,  8014,  8016,  8018,  8020,  8019,  8019,  8019,  8019,  8019,  8019,  8019,  8022,  8023,  8023,  8024,  8021,  8021,  8019,  8017,  8015,  8012,  8013,  8016,  8016,  8017,  8013,  8012,  8007,  8014,  8014,  8009,  8012,  8265,  8005,  8007,  8003,  8008,  8002,  8003,  7996,  7983,  7989},
{8029,  8027,  8029,  8029,  8031,  8031,  8031,  8031,  8045,  8139,  8252,  8263,  8254,  8253,  8253,  8244,  8216,  8177,  8229,  8329,  8353,  8362,  8304,  8296,  8355,  8382,  8368,  8349,  8331,  8313,  8257,  8205,  8063,  8022,  8022,  8021,  8018,  8013,  8019,  8019,  8020,  8022,  8021,  8024,  8021,  8023,  8022,  8023,  8021,  8022,  8025,  8026,  8026,  8026,  8020,  8021,  8019,  8016,  8011,  8013,  8016,  8013,  8012,  8008,  8011,  8008,  8011,  8012,  8010,  8022,  8261,  8009,  8005,  8000,  8004,  8000,  8000,  7997,  7988,  7990},
{8028,  8022,  8029,  8028,  8028,  8029,  8027,  8032,  8041,  8113,  8232,  8204,  8142,  8154,  8138,  8109,  8075,  8080,  8221,  8350,  8373,  8374,  8311,  8318,  8368,  8410,  8404,  8385,  8364,  8326,  8257,  8213,  8118,  8029,  8024,  8023,  8021,  8014,  8019,  8018,  8019,  8019,  8018,  8023,  8023,  8019,  8023,  8024,  8025,  8025,  8025,  8025,  8022,  8024,  8021,  8018,  8018,  8013,  8013,  8013,  8016,  8015,  8012,  8013,  8013,  8009,  8010,  8012,  8014,  8014,  8261,  8010,  8008,  8002,  8000,  7998,  7997,  7998,  7983,  7992},
{8023,  8023,  8021,  8021,  8021,  8016,  8027,  8027,  8035,  8089,  8205,  8149,  8046,  8036,  8035,  8034,  8036,  8059,  8221,  8367,  8402,  8411,  8359,  8314,  8385,  8422,  8406,  8388,  8378,  8360,  8268,  8203,  8129,  8027,  8023,  8021,  8021,  8018,  8018,  8020,  8020,  8020,  8021,  8019,  8022,  8020,  8022,  8022,  8022,  8020,  8023,  8018,  8021,  8022,  8019,  8018,  8020,  8015,  8015,  8017,  8015,  8015,  8016,  8014,  8014,  8011,  8015,  8015,  8011,  8015,  8267,  8011,  8009,  8002,  8005,  7995,  8001,  8002,  7985,  7986},
{8016,  8013,  8019,  8015,  8020,  8019,  8021,  8021,  8031,  8075,  8168,  8078,  8033,  8026,  8031,  8033,  8034,  8057,  8177,  8364,  8396,  8416,  8419,  8386,  8402,  8430,  8393,  8373,  8346,  8386,  8254,  8197,  8128,  8029,  8019,  8020,  8018,  8017,  8017,  8019,  8019,  8018,  8019,  8015,  8020,  8017,  8020,  8020,  8018,  8017,  8019,  8020,  8018,  8022,  8019,  8018,  8018,  8016,  8012,  8016,  8016,  8019,  8016,  8021,  8016,  8014,  8017,  8015,  8012,  8009,  8266,  8012,  8006,  8001,  8005,  7997,  7999,  8002,  7991,  7989},
{8016,  8016,  8020,  8021,  8022,  8023,  8022,  8023,  8026,  8051,  8072,  8033,  8025,  8024,  8034,  8036,  8036,  8051,  8160,  8345,  8388,  8397,  8404,  8408,  8398,  8389,  8364,  8354,  8361,  8383,  8246,  8186,  8119,  8030,  8020,  8020,  8016,  8011,  8011,  8012,  8011,  8013,  8015,  8019,  8019,  8017,  8018,  8017,  8020,  8017,  8022,  8019,  8020,  8021,  8020,  8020,  8022,  8017,  8015,  8014,  8019,  8020,  8021,  8022,  8018,  8016,  8017,  8012,  8008,  8015,  8257,  8000,  7998,  7998,  8003,  7997,  7998,  8007,  7995,  7990},
{8018,  8022,  8025,  8025,  8022,  8024,  8028,  8023,  8025,  8032,  8027,  8021,  8028,  8026,  8033,  8034,  8034,  8049,  8134,  8311,  8354,  8380,  8401,  8411,  8394,  8388,  8383,  8381,  8382,  8342,  8233,  8166,  8108,  8024,  8013,  8015,  8014,  8009,  8008,  8010,  8013,  8015,  8016,  8019,  8019,  8019,  8018,  8022,  8021,  8015,  8017,  8012,  8019,  8022,  8022,  8025,  8023,  8019,  8016,  8016,  8019,  8019,  8023,  8024,  8017,  8014,  8014,  8009,  8005,  8006,  8264,  8002,  7993,  7995,  7996,  7995,  7997,  8007,  7997,  7992},
{8022,  8020,  8027,  8023,  8025,  8026,  8025,  8023,  8025,  8029,  8022,  8019,  8026,  8027,  8031,  8036,  8035,  8046,  8101,  8275,  8366,  8389,  8405,  8403,  8394,  8370,  8326,  8289,  8278,  8262,  8199,  8149,  8097,  8021,  8013,  8013,  8012,  8009,  8009,  8010,  8013,  8014,  8018,  8014,  8020,  8014,  8015,  8017,  8013,  8011,  8013,  8010,  8017,  8025,  8027,  8027,  8027,  8023,  8016,  8020,  8019,  8023,  8021,  8021,  8017,  8011,  8009,  8008,  8006,  7997,  8263,  7998,  7993,  7994,  7998,  7997,  8001,  8007,  8000,  7992},
{8021,  8022,  8028,  8029,  8026,  8028,  8023,  8024,  8022,  8029,  8020,  8022,  8025,  8026,  8030,  8034,  8034,  8041,  8072,  8206,  8294,  8337,  8373,  8347,  8314,  8259,  8226,  8219,  8215,  8209,  8167,  8134,  8077,  8018,  8016,  8016,  8014,  8007,  8010,  8009,  8014,  8012,  8013,  8011,  8012,  8007,  8011,  8012,  8007,  8007,  8006,  8005,  8015,  8020,  8028,  8026,  8023,  8021,  8013,  8018,  8014,  8015,  8016,  8013,  8010,  8008,  8007,  8008,  8006,  8004,  8259,  7994,  7994,  7994,  7997,  7996,  7996,  8005,  8004,  7999},
{8020,  8019,  8026,  8027,  8029,  8026,  8026,  8022,  8024,  8029,  8022,  8021,  8026,  8028,  8031,  8034,  8033,  8035,  8047,  8119,  8225,  8265,  8278,  8269,  8242,  8213,  8195,  8186,  8180,  8172,  8138,  8117,  8045,  8017,  8017,  8018,  8013,  8008,  8006,  8008,  8016,  8015,  8014,  8011,  8015,  8014,  8014,  8017,  8009,  8004,  8007,  8005,  8006,  8015,  8020,  8016,  8018,  8013,  8013,  8012,  8009,  8006,  8011,  8009,  8008,  8006,  8007,  8008,  8004,  8008,  8261,  7991,  7995,  7994,  7998,  7995,  7998,  8000,  8004,  8001},
{8018,  8016,  8021,  8026,  8028,  8027,  8025,  8025,  8020,  8025,  8023,  8022,  8026,  8027,  8031,  8033,  8030,  8034,  8041,  8063,  8142,  8211,  8224,  8223,  8209,  8185,  8171,  8160,  8155,  8142,  8121,  8065,  8022,  8015,  8017,  8014,  8014,  8006,  8009,  8013,  8022,  8016,  8015,  8015,  8018,  8015,  8017,  8018,  8012,  8003,  8007,  8005,  8012,  8011,  8010,  8009,  8012,  8010,  8009,  8009,  8007,  8007,  8010,  8009,  8006,  8006,  8007,  8008,  8007,  8002,  8252,  8003,  7995,  7996,  8000,  7997,  7997,  8001,  8003,  8002},
{8016,  8018,  8023,  8025,  8024,  8025,  8024,  8022,  8023,  8031,  8025,  8022,  8029,  8028,  8030,  8034,  8031,  8029,  8036,  8038,  8055,  8140,  8179,  8187,  8184,  8187,  8162,  8147,  8138,  8117,  8054,  8022,  8019,  8009,  8013,  8013,  8012,  8010,  8011,  8015,  8016,  8015,  8014,  8015,  8019,  8017,  8020,  8021,  8019,  8007,  8002,  8007,  8013,  8012,  8009,  8009,  8010,  8006,  8006,  8007,  8007,  8006,  8008,  8008,  8007,  8006,  8007,  8009,  8006,  8014,  8261,  8010,  8003,  7998,  8002,  8000,  8000,  8000,  8003,  8007},
{8018,  8021,  8021,  8020,  8017,  8018,  8017,  8020,  8021,  8028,  8021,  8020,  8023,  8026,  8031,  8031,  8030,  8026,  8033,  8031,  8029,  8053,  8106,  8155,  8162,  8163,  8145,  8135,  8096,  8043,  8012,  8012,  8014,  8006,  8010,  8014,  8011,  8008,  8010,  8014,  8015,  8016,  8014,  8019,  8021,  8017,  8019,  8022,  8017,  8009,  8009,  8008,  8012,  8012,  8009,  8009,  8007,  8004,  8005,  8006,  8006,  8008,  8008,  8010,  8009,  8006,  8008,  8010,  8007,  8001,  8262,  8011,  8008,  8001,  8006,  8006,  8006,  8006,  7999,  8000},
{8026,  8026,  8021,  8022,  8016,  8016,  8017,  8017,  8020,  8028,  8023,  8022,  8023,  8027,  8030,  8032,  8029,  8025,  8028,  8027,  8021,  8025,  8029,  8045,  8056,  8063,  8059,  8035,  8015,  8008,  8006,  8008,  8013,  8009,  8012,  8018,  8012,  8008,  8011,  8013,  8018,  8018,  8017,  8020,  8020,  8019,  8021,  8021,  8017,  8010,  8009,  8012,  8009,  8010,  8008,  8007,  8007,  8006,  8004,  8008,  8011,  8006,  8008,  8007,  8010,  8006,  8010,  8012,  8009,  8015,  8268,  7998,  8011,  8010,  8006,  8010,  8013,  8009,  8001,  8001},
{8025,  8027,  8025,  8020,  8015,  8021,  8019,  8017,  8021,  8027,  8017,  8017,  8023,  8026,  8031,  8030,  8029,  8026,  8025,  8027,  8018,  8020,  8021,  8018,  8011,  8010,  8005,  7999,  7999,  8003,  8006,  8003,  8010,  8008,  8012,  8015,  8012,  8008,  8009,  8012,  8016,  8015,  8017,  8018,  8018,  8016,  8022,  8024,  8022,  8007,  8010,  8008,  8009,  8010,  8008,  8010,  8007,  8007,  8007,  8009,  8012,  8009,  8011,  8013,  8010,  8011,  8011,  8012,  8009,  8015,  8274,  8003,  8007,  8007,  8005,  8013,  8011,  8008,  7999,  8001},
{8026,  8024,  8026,  8023,  8022,  8017,  8019,  8017,  8020,  8025,  8013,  8013,  8019,  8021,  8030,  8029,  8027,  8024,  8023,  8029,  8021,  8021,  8020,  8017,  8007,  8003,  8004,  8000,  7996,  8001,  8007,  8002,  8005,  8001,  8006,  8011,  8011,  8007,  8011,  8016,  8017,  8019,  8015,  8020,  8021,  8016,  8020,  8020,  8021,  8012,  8010,  8009,  8011,  8012,  8011,  8005,  8010,  8008,  8010,  8010,  8013,  8011,  8009,  8012,  8012,  8008,  8009,  8008,  8006,  8007,  8259,  8000,  8001,  7999,  8004,  8011,  8009,  8004,  7996,  7996},
{8024,  8023,  8025,  8026,  8020,  8019,  8019,  8021,  8024,  8027,  8019,  8019,  8021,  8024,  8030,  8030,  8027,  8025,  8025,  8026,  8018,  8018,  8015,  8016,  8007,  8001,  8003,  8000,  7996,  8001,  8003,  8000,  8000,  8001,  8002,  8008,  8009,  8007,  8009,  8014,  8015,  8015,  8014,  8019,  8018,  8017,  8017,  8020,  8020,  8014,  8012,  8014,  8014,  8011,  8012,  8010,  8011,  8006,  8007,  8010,  8011,  8010,  8010,  8008,  8009,  8006,  8010,  8008,  8007,  8017,  8266,  7998,  7998,  7995,  7999,  8008,  8004,  8002,  7995,  8001},
{8026,  8026,  8028,  8026,  8024,  8024,  8022,  8022,  8022,  8028,  8023,  8021,  8023,  8025,  8034,  8032,  8029,  8024,  8028,  8028,  8016,  8018,  8019,  8018,  8010,  7999,  8002,  8001,  7996,  8000,  8001,  8001,  8000,  7998,  8001,  8006,  8011,  8008,  8009,  8011,  8014,  8016,  8014,  8015,  8018,  8012,  8008,  8016,  8009,  8010,  8013,  8012,  8010,  8010,  8007,  8009,  8007,  8007,  8007,  8010,  8011,  8013,  8013,  8009,  8007,  8008,  8011,  8012,  8010,  8019,  8265,  8006,  8000,  7999,  7997,  8008,  8008,  8009,  8002,  8005},
{8026,  8021,  8026,  8022,  8022,  8025,  8022,  8021,  8023,  8027,  8022,  8022,  8025,  8026,  8031,  8032,  8026,  8028,  8028,  8028,  8020,  8017,  8015,  8017,  8008,  7998,  7999,  8000,  7996,  7999,  8001,  8000,  8002,  7998,  8004,  8006,  8007,  8005,  8004,  8007,  8007,  8008,  8008,  8008,  8008,  8008,  8006,  8007,  8005,  8009,  8013,  8011,  8011,  8010,  8009,  8008,  8007,  8002,  8006,  8009,  8013,  8013,  8012,  8011,  8007,  8010,  8009,  8009,  8008,  8015,  8278,  7999,  8001,  8001,  8003,  8011,  8010,  8005,  7998,  7998},
{8023,  8022,  8024,  8019,  8022,  8019,  8018,  8020,  8022,  8028,  8027,  8022,  8023,  8026,  8033,  8032,  8029,  8031,  8028,  8028,  8017,  8018,  8017,  8015,  8005,  8001,  7999,  8001,  7996,  8002,  8003,  8005,  8007,  8004,  8002,  8002,  8000,  8001,  8002,  8004,  8005,  8008,  8006,  8007,  8009,  8008,  8005,  8005,  8007,  8011,  8011,  8011,  8013,  8013,  8012,  8010,  8008,  8007,  8005,  8009,  8010,  8009,  8010,  8013,  8008,  8010,  8011,  8011,  8005,  8001,  8279,  7993,  8001,  8004,  8002,  8008,  8009,  8006,  7993,  7999},
{8017,  8018,  8021,  8015,  8013,  8016,  8018,  8015,  8016,  8027,  8023,  8019,  8025,  8024,  8030,  8030,  8027,  8027,  8027,  8023,  8018,  8016,  8017,  8016,  8006,  7999,  8000,  8002,  8001,  8005,  8008,  8006,  8006,  8003,  7999,  8003,  8000,  7997,  8003,  8005,  8008,  8008,  8006,  8009,  8009,  8008,  8005,  8008,  8011,  8012,  8013,  8015,  8012,  8014,  8010,  8010,  8010,  8005,  8010,  8008,  8006,  8008,  8010,  8010,  8006,  8008,  8008,  8011,  8010,  8011,  8282,  8015,  8010,  8007,  8006,  8007,  8010,  8006,  7996,  7999},
{8021,  8018,  8020,  8012,  8011,  8012,  8017,  8016,  8016,  8023,  8023,  8020,  8021,  8023,  8028,  8031,  8027,  8025,  8029,  8024,  8017,  8012,  8011,  8015,  8006,  8004,  8004,  8005,  8002,  8004,  8007,  8006,  8006,  8004,  8001,  8005,  8002,  8001,  8003,  8004,  8008,  8010,  8006,  8010,  8008,  8010,  8010,  8010,  8012,  8012,  8013,  8015,  8014,  8018,  8014,  8018,  8014,  8010,  8007,  8008,  8011,  8010,  8011,  8010,  8009,  8009,  8011,  8013,  8012,  8015,  8284,  8012,  8012,  8008,  8007,  8007,  8012,  8008,  7999,  8003},
{8019,  8012,  8018,  8010,  8011,  8016,  8017,  8017,  8015,  8020,  8023,  8020,  8020,  8025,  8025,  8029,  8027,  8027,  8024,  8020,  8013,  8010,  8006,  8010,  8006,  8005,  8008,  8005,  7998,  8000,  8004,  8006,  8005,  7998,  8002,  8003,  8005,  8002,  8004,  8006,  8009,  8008,  8007,  8010,  8011,  8008,  8011,  8011,  8010,  8011,  8014,  8014,  8015,  8016,  8014,  8017,  8017,  8012,  8012,  8008,  8011,  8011,  8014,  8012,  8010,  8009,  8014,  8015,  8014,  8023,  8274,  8014,  8011,  8008,  8007,  8004,  8012,  8008,  7995,  8001},
{8016,  8016,  8020,  8012,  8009,  8016,  8017,  8016,  8016,  8023,  8021,  8020,  8020,  8023,  8027,  8028,  8025,  8026,  8026,  8022,  8013,  8010,  8007,  8012,  8012,  8010,  8009,  8006,  7998,  7999,  8003,  8002,  8004,  8000,  8005,  8005,  8005,  8004,  8006,  8009,  8010,  8008,  8008,  8009,  8012,  8010,  8012,  8014,  8013,  8010,  8015,  8017,  8017,  8016,  8017,  8017,  8017,  8014,  8013,  8016,  8011,  8012,  8014,  8015,  8013,  8011,  8013,  8017,  8012,  8020,  8275,  8013,  8013,  8007,  8008,  8007,  8013,  8011,  7999,  8000},
{8016,  8017,  8018,  8013,  8008,  8012,  8014,  8016,  8017,  8020,  8015,  8016,  8017,  8018,  8023,  8029,  8025,  8028,  8025,  8020,  8012,  8006,  8007,  8012,  8006,  8009,  8009,  8007,  8002,  7999,  8002,  8003,  8005,  7998,  8004,  8005,  8006,  8003,  8007,  8009,  8008,  8009,  8010,  8010,  8012,  8012,  8010,  8015,  8013,  8016,  8017,  8020,  8014,  8016,  8017,  8017,  8016,  8016,  8012,  8013,  8013,  8013,  8012,  8016,  8010,  8013,  8014,  8016,  8012,  8014,  8287,  8010,  8008,  8004,  8009,  8006,  8011,  8011,  8000,  8002},
{8016,  8013,  8018,  8014,  8008,  8014,  8016,  8015,  8012,  8019,  8017,  8014,  8018,  8020,  8023,  8024,  8027,  8024,  8027,  8023,  8011,  8009,  8008,  8011,  8007,  8008,  8006,  8007,  8000,  8000,  8004,  8006,  8004,  8003,  8004,  8008,  8007,  8004,  8004,  8008,  8010,  8012,  8011,  8011,  8014,  8011,  8010,  8014,  8013,  8016,  8017,  8017,  8018,  8018,  8015,  8016,  8016,  8017,  8012,  8010,  8012,  8012,  8012,  8017,  8011,  8011,  8016,  8018,  8011,  8017,  8291,  8011,  8007,  8005,  8010,  8004,  8009,  8015,  8005,  8005},
{8011,  8014,  8013,  8010,  8008,  8014,  8018,  8015,  8016,  8015,  8017,  8015,  8017,  8016,  8020,  8022,  8024,  8025,  8025,  8021,  8012,  8007,  8007,  8011,  8006,  8002,  8000,  8002,  8002,  8000,  8004,  8004,  8003,  8001,  8003,  8006,  8005,  8002,  8005,  8008,  8009,  8012,  8013,  8013,  8010,  8014,  8009,  8015,  8013,  8017,  8017,  8017,  8015,  8013,  8016,  8020,  8018,  8016,  8009,  8011,  8009,  8009,  8013,  8014,  8011,  8009,  8013,  8017,  8008,  8012,  8300,  8016,  8003,  8008,  8005,  8001,  8008,  8018,  8008,  8009},
{8006,  8007,  8006,  8004,  8006,  8009,  8013,  8012,  8014,  8012,  8019,  8015,  8012,  8013,  8019,  8022,  8025,  8022,  8022,  8020,  8010,  8008,  8007,  8008,  7998,  8001,  7994,  7996,  7999,  8000,  8002,  8002,  8002,  8001,  8007,  8008,  8004,  8002,  8007,  8006,  8011,  8012,  8012,  8016,  8013,  8011,  8010,  8015,  8013,  8015,  8017,  8017,  8017,  8020,  8015,  8018,  8015,  8013,  8010,  8012,  8010,  8011,  8012,  8013,  8012,  8013,  8012,  8016,  8010,  8010,  8289,  7996,  8010,  8006,  8007,  8004,  8011,  8016,  8011,  8010},
{8009,  8006,  8006,  8004,  8003,  8006,  8011,  8009,  8014,  8015,  8012,  8012,  8014,  8013,  8021,  8020,  8019,  8021,  8021,  8021,  8009,  8007,  8007,  8007,  8001,  8001,  7996,  7998,  8000,  8000,  8006,  8003,  8001,  7999,  8005,  8005,  8004,  8003,  8008,  8007,  8010,  8013,  8012,  8014,  8014,  8014,  8013,  8018,  8015,  8014,  8015,  8017,  8019,  8017,  8016,  8017,  8014,  8013,  8007,  8010,  8012,  8008,  8010,  8013,  8011,  8010,  8011,  8015,  8007,  8015,  8296,  7999,  8006,  8005,  8006,  8003,  8008,  8020,  8013,  8009},
{8010,  8003,  8007,  8003,  8007,  8006,  8009,  8008,  8014,  8015,  8011,  8010,  8010,  8012,  8018,  8017,  8015,  8018,  8017,  8014,  8009,  8004,  8007,  8007,  8003,  8002,  7997,  7997,  7996,  8001,  8009,  8005,  8005,  8001,  8006,  8009,  8007,  8006,  8008,  8009,  8015,  8010,  8009,  8012,  8016,  8012,  8016,  8016,  8015,  8020,  8019,  8019,  8019,  8021,  8018,  8016,  8014,  8012,  8013,  8015,  8013,  8008,  8011,  8013,  8017,  8012,  8013,  8011,  8008,  8006,  8296,  8009,  8008,  8004,  8008,  8008,  8007,  8021,  8014,  8012},
{8009,  8004,  8006,  8005,  8007,  8007,  8012,  8010,  8013,  8018,  8009,  8009,  8011,  8012,  8018,  8021,  8017,  8015,  8014,  8014,  8005,  8003,  8006,  8009,  8006,  8008,  8002,  8001,  8003,  8006,  8013,  8009,  8011,  8004,  8011,  8010,  8012,  8013,  8012,  8013,  8014,  8019,  8012,  8017,  8021,  8019,  8022,  8023,  8020,  8019,  8026,  8025,  8025,  8024,  8022,  8021,  8018,  8015,  8015,  8019,  8017,  8015,  8012,  8012,  8013,  8018,  8016,  8016,  8017,  8015,  8302,  8014,  8018,  8010,  8014,  8013,  8011,  8024,  8015,  8014},};

          //Nacho
int cursorRawThermal_fila = 0;
int cursorRawThermal_columna = 0;

char Celsius_thermal_image[image_x][image_y];  
int r = 0; //cantidad de objetos            //Nacho
int s = 0; //cantidad de elementos en el objeto mayor         //Nacho
int sensor_dist[4] = {0, 0, 0, 0};         //Nacho
bool sensor_act[4] = {false, false, false, false};         //Nacho
char reset_onboard_error[15];         //Nacho

uint8_t serial_control_count;         //Nacho
//uint8_t data_on_board[70];         //Nacho


/////////// MODOS MISION ///////////////

byte MISSION_MODE_FOLLOW = 0x00;
byte MISSION_MODE_RTL = 0x01;
byte MISSION_MODE_CONT = 0x02;
byte MISSION_MODE = MISSION_MODE_FOLLOW;

const byte ONBOARDHEADER = 0x80;


// Punteros
Punto* Trayectoria;




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
void checkEmailCallBack();
void sendEmailCallBack();
void emailReaderCallBack();
void RTCMCallBack();
void Shield3GReaderCallBack();
void RTCMReaderCallBack();
void PX4ReaderCallBack();
void enviarReportesCallBack();
void XmodemCallBack();
void XmodemReaderCallBack();
void conmutarPinesCallback();
void consolaReaderCallback();

// Periodos de las tareas
const int PERIODOHEARTBEAT = 1e3;
const int PERIODOEMAIL = 10e3;
const int PERIODOEMAILREADER = 1e3;
const int PERIODORTCM = 10e3;
const int PERIODO3GREADER=1;
const int PERIODORTCMREADER=1;
const int PERIODOPX4READER=1;
const int PERIODOENVIARREPORTES=120e3;
const int PERIODOXMODEM=1;
const int PERIODOXMODEMREADER=1;
const int PERIODOCONMUTARPINES= 10;
const int PERIODOCONSOLAREADER= 1;

//Tareas
Task heartBeat(PERIODOHEARTBEAT, TASK_FOREVER, &heartBeatCallback); // Manda un heartbeat cada un segundo para siempre
Task checkEmail(PERIODOEMAIL,TASK_FOREVER, &checkEmailCallBack);
Task sendEmail(PERIODOEMAILREADER,TASK_FOREVER, &sendEmailCallBack);
Task emailReader(PERIODOEMAILREADER,TASK_FOREVER, &emailReaderCallBack);
#ifdef RTCMdef
Task RTCM(PERIODORTCM,TASK_FOREVER,&RTCMCallBack);
Task RTCMReader(PERIODORTCMREADER,TASK_FOREVER,&RTCMReaderCallBack);
#endif
Task Shield3GReader(PERIODO3GREADER,TASK_FOREVER,&Shield3GReaderCallBack);
Task PX4Reader(PERIODOPX4READER,TASK_FOREVER,&PX4ReaderCallBack);
Task enviarReportes(PERIODOENVIARREPORTES,TASK_FOREVER,&enviarReportesCallBack);
Task Xmodem(PERIODOXMODEM,TASK_FOREVER,&XmodemCallBack);
Task XmodemReader(PERIODOXMODEMREADER, TASK_FOREVER, &XmodemReaderCallBack);
Task conmutarPines(PERIODOCONMUTARPINES, TASK_FOREVER, &conmutarPinesCallback);
Task consolaReader(PERIODOCONSOLAREADER, TASK_FOREVER, &consolaReaderCallback);

////////////////////////////////Weather///////////////////////
#define DHTPIN 52     // Digital Pin of Arduino Board
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

#define RainPIN 53
#define AnRainPIN 3



void AnularArray(char* data,char zero,int dataLen){

  for(int i = 0; i < dataLen;i++){
   data[i]= zero;
  }
  
}
