//#include "/home/damian/Dropbox/Termodron_Proyecto/Software/MAVLink/c_library_v2/common/mavlink.h" // Libreria para MAVLink     
//#include "/home/damian/Dropbox/Termodron_Proyecto/Software/MAVLink/TaskScheduler/TaskScheduler.h" // Libreria para multithreading
#include "C:/Users/nacho15/Dropbox/Termodron_Proyecto/Software/MAVLink/c_library_v2/common/mavlink.h" // Libreria para MAVLink   
#include "C:/Users/nacho15/Dropbox/Termodron_Proyecto/Software/MAVLink/TaskScheduler/TaskScheduler.h" // Libreria para multithreading

#define px4 Serial1
#define sermon Serial

// Callback de las tareas
void heartBeatCallback();
void missionRequestListCallBack();

//Tareas
Task heartBeat(1000, TASK_FOREVER, &heartBeatCallback); // Manda un heartbeat cada un segundo para siempre
Task missionRequestList(500, TASK_FOREVER, &missionRequestListCallBack); // Pide la lista de misiones

Scheduler runner; // se crea el scheduler

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
  float disable=-1;
  uint8_t frame = MAV_FRAME_GLOBAL_RELATIVE_ALT; // 3, x = latitud, y = longitud, z= altitud sobre home
  uint16_t command;
  uint8_t current=1;
  uint8_t autocontinue=1;
  float param2=-1;
  float param3=0; 
  float param4=0;
  int32_t x=0;
  int32_t y=0;
  float z=0;
  uint8_t confirmation=0;
  float periodoGPS=1e6; // Distancia en microsegundos entre dos mensajes de GPS
  int timeout =5; // tiempo maximo de espera para los waypoints
  uint16_t count = 0; // cantidad de waypoints que tiene una mision
  uint16_t seq = 0;
  int chan = 0;
  uint32_t tmax = 2000; // timeout para los mensajes

void setup() {
  
	px4.begin(57600);
  sermon.begin(115200);
  // Se inicializa el scheduler y se le agrega el HB
  runner.init();
  runner.addTask(heartBeat);
  //runner.addTask(missionRequestList);
  heartBeat.enable();
  //missionRequestList.enable();
  // Se manda un HB suelto para presentarse
  sendHeartBeat();
  // Ahora la PX ya sabe quienes somos
  
/*
  
// Cargar una mision

clearall();
sendwaypointcount(2);
// Punto 1
waitfor(MAVLINK_MSG_ID_MISSION_REQUEST, 2000);

seq = 0;
uint16_t  command  =  MAV_CMD_NAV_WAYPOINT; // CMD_ID = 16
uint8_t current = 0; // Punto actual
uint8_t autocontinue  = 1; // True 
float param1  = 0; // Tiempo de hold  
float param2  = 0; // Radio de aceptacion en metros
float param3  = 0; // 0 para pasar por el WP
float param4  = 0; // YAW deseado
float x = -348671840*1E-7; // latitud grados * 1E7
float y = -561811200*1E-7; // longitud grados * 1E7
float z = 25.00; // altitud metros relativa a home (suponiendo frame = 3)
sermon.println(x);
sendwaypoint(seq,frame,command,current,autocontinue,param1,param2,param3,param4,x,y,z);

seq = 1;
x = -348671869*1E-7; // latitud grados * 1E7
y = -561811269*1E-7; // longitud grados * 1E7
z = 69.00; // altitud metros relativa a home (suponiendo frame = 3)

waitfor(MAVLINK_MSG_ID_MISSION_REQUEST, tmax);

sendwaypoint(seq,frame,command,current,autocontinue,param1,param2,param3,param4,x,y,z);

waitfor(MAVLINK_MSG_ID_MISSION_ACK, tmax);

// Fin mision

// Se deshabilitan los mensajes que manda por defecto
  shutup(); 

// Pedir los elementos de la mision

missionrequest();
waitfor(MAVLINK_MSG_ID_MISSION_COUNT, tmax);
for(uint16_t i=0; i < count; i++){
waypointrequest(i);
waitfor(MAVLINK_MSG_ID_MISSION_ITEM, tmax);
}//for
missionack();
*/
}//setup

void loop() { 
  runner.execute();
  comm_receive();
}//loop

void comm_receive() { 
        
	mavlink_message_t msg; 
	mavlink_status_t stat;
  while(px4.available()>0) { 
	  uint8_t c = px4.read();
		if(mavlink_parse_char(MAVLINK_COMM_0, c, &msg, &stat)) { 
 			sermon.print("Mensaje recibido, ID= ");
 			sermon.println(msg.msgid);
 			switch(msg.msgid) {
			        case MAVLINK_MSG_ID_ATTITUDE: {  //ID del mensaje 30
    	       /* sermon.println("Actitud recibida");
              float yaw =mavlink_msg_attitude_get_yaw(&msg)*180/PI;
              sermon.print("YAW= ");
              sermon.print(yaw);
              sermon.println(" °");
             

              float pitch =mavlink_msg_attitude_get_pitch(&msg)*180/PI;
              sermon.print("PITCH= ");
              sermon.print(pitch);
              sermon.println(" °");

              float roll =mavlink_msg_attitude_get_roll(&msg)*180/PI;
              sermon.print("ROLL= ");
              sermon.print(roll);
              sermon.println(" °");
               */
               break;
			        }
             case MAVLINK_MSG_ID_SYS_STATUS: { // ID del mensaje 1
              /*sermon.println("Estado del sistema recibido");

             */ break;
             }

             case MAVLINK_MSG_ID_VFR_HUD: {   // ID del mensaje 74
              /*sermon.println("VFR (Visual Flight rules) HUD (Heads Up Display) recibido");

              */break;
              }

              case MAVLINK_MSG_ID_ATTITUDE_TARGET:{ //ID del mensaje 83
              /*sermon.println("Actitud objetivo recibida");
              

                */break;
              }
              case MAVLINK_MSG_ID_HEARTBEAT :{ //ID del mensaje 0
           /*   sermon.print("Heartbeat recibido, t= ");
              sermon.print(millis()/1000);
              sermon.println(" s");
              sermon.print("Target ID: ");
              sermon.println(msg.sysid);
              sermon.print("Component ID: ");
              sermon.println(msg.compid);
            
             uint8_t ver=mavlink_msg_heartbeat_get_mavlink_version(&msg);
             sermon.print("Version: ");
             sermon.println(ver);
             */
             break;
              }
              
              case MAVLINK_MSG_ID_PING :{ //ID del mensaje 4
              sermon.print("Ping recibido, t= ");
              sermon.println(millis()/1000);
              

              break;

              }


            case MAVLINK_MSG_ID_GLOBAL_POSITION_INT :{ //ID del mensaje 33
  
            uint32_t pos_time_boot_ms= mavlink_msg_global_position_int_get_time_boot_ms(&msg); // Timestamp (milisegundos desde el booteo del sistema
            int32_t pos_lat=mavlink_msg_global_position_int_get_lat(&msg); // Latitud, expresada en grados * 1E7
            int32_t pos_lon=mavlink_msg_global_position_int_get_lon(&msg); // Longitud, expresada en grados * 1E7
            int32_t pos_alt=mavlink_msg_global_position_int_get_alt(&msg); // Altitud en metros, expresada en milimetros, AMSL
            int32_t pos_relative_alt=mavlink_msg_global_position_int_get_relative_alt(&msg); // Altitud sobre el suelo en metros, expresada como * 1000 (milimetros)
            int16_t pos_vx=mavlink_msg_global_position_int_get_vx(&msg); // Velocidad en tierra X (Latitud, norte positivo), expresada en m/s * 100
            int16_t pos_vy=mavlink_msg_global_position_int_get_vy(&msg); // Velocidad en tierra Y (Longitud, este positivo), expresada en m/s * 100
            int16_t pos_vz=mavlink_msg_global_position_int_get_vz(&msg); // Velocidad en tierra Z (Altitud, abajo positivo), expresada en m/s * 100
            uint16_t pos_hdg; // Direccion del vehiculo (angulo yaw) en grados * 100, 0.0..359.99 degrees. su no se sabe se setea en UINT16_MAX

            sermon.print("Latitud: ");
            sermon.print(pos_lat);
            sermon.println(" grados * 1E7");

            
            sermon.print("Longitud: ");
            sermon.print(pos_lon);
            sermon.println(" grados * 1E7");

            sermon.print("Altitud: ");
            sermon.print(pos_alt);
            sermon.println(" milimetros");
            
            break;

            }

              
              case MAVLINK_MSG_ID_COMMAND_ACK :{ //ID del mensaje 77
              sermon.print("Command Acknowledge recibido, t= ");
              sermon.println(millis()/1000);
              uint8_t result= mavlink_msg_command_ack_get_result(&msg);
              switch(result) {
                case MAV_RESULT_ACCEPTED:{
                  sermon.println("MAV_RESULT_ACCEPTED"); //Command ACCEPTED and EXECUTED
                  break;
                  }
                case MAV_RESULT_TEMPORARILY_REJECTED:{
                  sermon.println("MAV_RESULT_TEMPORARILY_REJECTED"); //Command TEMPORARY REJECTED/DENIED
                  break;
                  }
                case MAV_RESULT_DENIED:{
                  sermon.println("MAV_RESULT_DENIED"); //Command PERMANENTLY DENIED
                  break;
                  }
                case MAV_RESULT_UNSUPPORTED:{
                  sermon.println("MAV_RESULT_UNSUPPORTED"); //Command UNKNOWN/UNSUPPORTED
                  break;
                  }
                  case MAV_RESULT_FAILED:{
                  sermon.println("MAV_RESULT_FAILED"); //Command executed, but failed
                  break;
                  }
                  default:
                  sermon.println("wat");
                  break;
                             
              } // result
              break;
              } // case del 77
              
              case MAVLINK_MSG_ID_MISSION_COUNT :{ // ID del mensaje 44
               count = mavlink_msg_mission_count_get_count(&msg);
               sermon.print("La mision tiene ");
               sermon.print(count);
               sermon.println(" elementos");
               break;
              } // case del 44

               
               case MAVLINK_MSG_ID_MISSION_ITEM :{ // ID del mensaje 39

                         // Mision recibida
                         
                         uint16_t seq = mavlink_msg_mission_item_get_seq(&msg);
                         sermon.print("Item ");
                         sermon.print(seq);
                         sermon.println(" recibido");
                         // latitud
                         float lat = mavlink_msg_mission_item_get_x(&msg);
                         // longitud
                         float lon = mavlink_msg_mission_item_get_y(&msg);
                         // altitud
                         float alt = mavlink_msg_mission_item_get_z(&msg);
                         sermon.print("Latitud ");
                         sermon.print(lat);
                         sermon.println(" grados * 1E7");
                         sermon.print("Longitud ");
                         sermon.print(lon);
                         sermon.println(" grados * 1E7"); 
                         sermon.print("Altitud ");
                         sermon.print(alt);
                         sermon.println(" metros");
                         break;
    
                  } // case 39

              
				default:
					//sermon.println("nada");
				break;
			}
		} 
    
	}

}

    void sendHeartBeat() { 
    mavlink_message_t msg; // crea un mensaje
    uint8_t buf[MAVLINK_MAX_PACKET_LEN]; // crea un buffer para guardarlo
    mavlink_msg_heartbeat_pack(system_id, component_id, &msg, system_type, autopilot_type, base_mode, custom_mode, system_status); // Arma el mensaje
    uint16_t len = mavlink_msg_to_send_buffer(buf, &msg); //guarda el mensaje en el buffer y devuelve el largo del mensaje (payload+no payload) 
    px4.write(buf, len); // manda el mensaje 
    sermon.println("Heartbeat enviado"); // Imprime que lo mandó
    buf[MAVLINK_MAX_PACKET_LEN];
    mavlink_msg_command_long_pack(system_id, component_id, &msg,target_system, target_component, MAV_CMD_SET_MESSAGE_INTERVAL, confirmation, MAVLINK_MSG_ID_GLOBAL_POSITION_INT, periodoGPS, param3, param4, x, y, z);
    len = mavlink_msg_to_send_buffer(buf, &msg);
    px4.write(buf, len);
    }

// Callbacks

    // Heartbeat
    void heartBeatCallback() {
    sendHeartBeat();
    }

    void shutup() {
     for(int i=1; i < 290; i++){
      mavlink_message_t msg;
      uint8_t buf[MAVLINK_MAX_PACKET_LEN];
      mavlink_msg_command_long_pack(system_id, component_id, &msg,target_system, target_component, MAV_CMD_SET_MESSAGE_INTERVAL, confirmation, i, disable, param3, param4, x, y, z);
      uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
      px4.write(buf, len);
     }//for
    }//shutup
 
    void requestGPS(float t){
    mavlink_message_t msg;
    uint8_t buf[MAVLINK_MAX_PACKET_LEN];
    mavlink_msg_command_long_pack(system_id, component_id, &msg,target_system, target_component, MAV_CMD_SET_MESSAGE_INTERVAL, confirmation, MAVLINK_MSG_ID_GLOBAL_POSITION_INT, t, param3, param4, x, y, z);
    uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
    px4.write(buf, len);
    }//requestGPS

    void missionRequestListCallBack(){
    mavlink_message_t msg;
    uint8_t buf[MAVLINK_MAX_PACKET_LEN];
    mavlink_msg_mission_request_list_pack(system_id, component_id, &msg, target_system, target_component);
    uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
    px4.write(buf, len);
    }

    void waypointrequest(uint16_t seq){
    mavlink_message_t msg;
    uint8_t buf[MAVLINK_MAX_PACKET_LEN];
    mavlink_msg_mission_request_pack(system_id, component_id, &msg, target_system, target_component,seq);
    uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
    px4.write(buf, len);
    }

    void sendwaypointack(){
    mavlink_message_t msg;
    uint8_t buf[MAVLINK_MAX_PACKET_LEN];
    mavlink_msg_mission_ack_pack(system_id, component_id, &msg, target_system, target_component,MAV_RESULT_ACCEPTED);
    uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
    px4.write(buf, len);
    }

    void sendwaypointcount(uint16_t count){
    mavlink_message_t msg;
    uint8_t buf[MAVLINK_MAX_PACKET_LEN];
    mavlink_msg_mission_count_pack(system_id, component_id, &msg, target_system, target_component,count);
    uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
    px4.write(buf, len);
    }
    
    void sendwaypoint(uint16_t seq, uint8_t frame, uint16_t command,uint8_t current, uint8_t autocontinue, float param1, float param2, float param3, float param4, float x, float y, float z){
    mavlink_message_t msg;
    uint8_t buf[MAVLINK_MAX_PACKET_LEN];
    mavlink_msg_mission_item_pack(system_id, component_id, &msg, target_system, target_component,seq,frame,command,current,autocontinue,param1,param2,param3,param4,x,y,z);
    uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
    px4.write(buf, len);
    }
    
    void clearall(){
    mavlink_message_t msg;
    uint8_t buf[MAVLINK_MAX_PACKET_LEN];
    mavlink_msg_mission_clear_all_pack(system_id, component_id, &msg, target_system, target_component);
    uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
    px4.write(buf, len);
    }

    void waitfor(uint8_t MSG_ID, uint32_t timeout){
      uint32_t timestamp = millis();
      boolean received = 0;
      mavlink_message_t msg; 
      mavlink_status_t stat;
      while(received == 0 && millis() < timestamp+timeout){
        while(px4.available()>0) { 
           uint8_t c = px4.read();
           if(mavlink_parse_char(MAVLINK_COMM_0, c, &msg, &stat)) { 
            switch(msg.msgid) {
                  case MAVLINK_MSG_ID_MISSION_REQUEST: {
                    if(MSG_ID==MAVLINK_MSG_ID_MISSION_REQUEST){
                    received = 1;
                    uint16_t sequ = mavlink_msg_mission_request_get_seq(&msg);
                    sermon.print("mensaje esperado recibido, ID =");
                    sermon.println(msg.msgid);
                    sermon.println(sequ);
                    }//if
                    else{
                      sermon.print("mensaje diferente, ID =");
                      sermon.println(msg.msgid);
                    }
                    break;            
                  }//case
                  case MAVLINK_MSG_ID_MISSION_ACK: {
                    if(MSG_ID==MAVLINK_MSG_ID_MISSION_ACK){
                    received = 1;
                    sermon.print("mensaje esperado recibido, ID =");
                    sermon.println(msg.msgid);
                    }//if
                    else{
                      sermon.print("mensaje diferente, ID =");
                      sermon.println(msg.msgid);
                      }//else
                    break;            
                  }//case
                  case MAVLINK_MSG_ID_MISSION_COUNT:{
                  if(MSG_ID==MAVLINK_MSG_ID_MISSION_COUNT){
                    received = 1;
                    sermon.print("mensaje esperado recibido, ID =");
                    sermon.println(msg.msgid);
                    count = mavlink_msg_mission_count_get_count(&msg);
                  }//if
                  else{
                      sermon.print("mensaje diferente, ID =");
                      sermon.println(msg.msgid);
                  }//else
                  break;            
                  }//case
                  case MAVLINK_MSG_ID_MISSION_ITEM:{
                    if(MSG_ID==MAVLINK_MSG_ID_MISSION_ITEM){
                      received = 1;
                      sermon.print("mensaje esperado recibido, ID =");
                      sermon.println(msg.msgid);
                      uint16_t seq = mavlink_msg_mission_item_get_seq(&msg);
                      sermon.print("Item ");
                      sermon.print(seq);
                      sermon.println(" recibido");
                      // latitud
                      float lat = mavlink_msg_mission_item_get_x(&msg);
                      // longitud
                      float lon = mavlink_msg_mission_item_get_y(&msg);
                      // altitud
                      float alt = mavlink_msg_mission_item_get_z(&msg);
                      sermon.print("Latitud ");
                      sermon.print(lat*1E7);
                      sermon.println(" grados * 1E7");
                      sermon.print("Longitud ");
                      sermon.print(lon*1E7);
                      sermon.println(" grados * 1E7"); 
                      sermon.print("Altitud ");
                      sermon.print(alt);
                      sermon.println(" metros");
                    }//if
                    else{
                        sermon.print("mensaje diferente, ID =");
                        sermon.println(msg.msgid);
                    }//else
                    break;            
                    }//case
                  default:
                  sermon.print("mensaje diferente, ID =");
                  sermon.println(msg.msgid);
                  break;
            } //switch
          } //if
        } //while
      } //while
      if(millis() >= timestamp+timeout){
        sermon.print("Salio por timeout esperando mensaje #");
        sermon.println(MSG_ID);
      } //if
    } //waitfor


    void missionrequest(){
      mavlink_message_t msg;
      uint8_t buf[MAVLINK_MAX_PACKET_LEN];
      mavlink_msg_mission_request_list_pack(system_id, component_id, &msg, target_system, target_component);
      uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
      px4.write(buf, len);
    }//missionrequest

  void missionack(){
   mavlink_message_t msg;
   uint8_t type =MAV_MISSION_ACCEPTED; // 0 significa OK
   uint8_t buf[MAVLINK_MAX_PACKET_LEN];
   mavlink_msg_mission_ack_pack(system_id, component_id, &msg, target_system, target_component, type);
   uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
   px4.write(buf, len);
   sermon.println("Mision recibida OK");
   }//waypointack

