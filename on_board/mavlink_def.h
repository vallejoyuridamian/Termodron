uint8_t ARM = 1;
uint8_t DISARM = 0;
uint8_t system_type = MAV_TYPE_QUADROTOR; // 2
uint8_t autopilot_type = MAV_AUTOPILOT_PX4; // 12
uint8_t system_id = 253; // Identifica al sistema, 255 es standard para GCS
uint8_t component_id = 253; // Identifica al sistema, 255 es standard para GCS
uint8_t base_mode = 0;
uint32_t custom_mode = 0;
uint8_t system_status = 0;
uint8_t stream_id = 0;
uint8_t target_system = 1;
uint8_t target_component = 1;
uint8_t req_stream_id = 0;
uint8_t confirmation = 0;
uint8_t frame = MAV_FRAME_GLOBAL_RELATIVE_ALT; // 3, x = latitud, y = longitud, z= altitud sobre home
uint8_t autocontinue = 1;
float param1;
float param2 = -1;
float param3; 
float param4;
uint8_t current = 1;
uint16_t command;

uint16_t count = 0; // cantidad de waypoints que tiene una mision
uint16_t seq = 0;
uint16_t auxseq = 0;

int timeout = 5; // tiempo maximo de espera para los waypoints
uint32_t timeout_mav_msg = 2000; // timeout para los mensajes
float per_mav_msg = 1000;   //periodo de solicitud de mensajes

boolean received = 0;
bool start_log = false;

uint8_t mode_flag;
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

//uint8_t serial_control_device;
//uint8_t serial_control_flags;
//uint16_t serial_control_timeout;
//uint32_t serial_control_baudrate;
uint8_t serial_control_count;
uint8_t serial_control_data[70];

//Variables para comunicacion de datos MAVLink
struct WayPoint {
  double lat;
  double lon;
  double alt;
};
WayPoint home_wp, current_wp, actual_wp, new_wp;  //coord de el punto 0, del lugar actual, del way point siguente y del nuevo
uint16_t actual_yaw, prior_yaw;
WayPoint mission_wps[200];


int mission_mode = 0;//default 0:Follow; 1:RTL; 2:Cont

