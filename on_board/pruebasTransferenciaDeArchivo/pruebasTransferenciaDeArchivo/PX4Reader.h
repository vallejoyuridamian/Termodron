void stabilizeGimbal();
void configureGimbal();
void datarecive_error();      
void shutup(int MSG_ID_SHUTUP);
void requestMSG(int MSG_ID_REQUEST,float timeus);
void pingOnBoard();
void dataSend();

void PX4ReaderCallBack() { 

  /*
  while(PX4.available()>0){
    
  uint8_t c = PX4.read();
  
  Consola.println(c,HEX);  
    
  }
  */
  
  
  mavlink_message_t msg; 
  mavlink_status_t stat;
  while(PX4.available()>0) { 
    uint8_t c = PX4.read();
    if(mavlink_parse_char(MAVLINK_COMM_0, c, &msg, &stat)) { 
      
      #ifdef  ConsolaMon
      Consola.print("Mensaje recibido, ID= ");
      Consola.println(msg.msgid);
      #endif
      
      switch(msg.msgid) {
        case MAVLINK_MSG_ID_ATTITUDE: {  //ID del mensaje 30
                  
          yaw =mavlink_msg_attitude_get_yaw(&msg)*180/PI;
          pitch =mavlink_msg_attitude_get_pitch(&msg)*180/PI;
          roll =mavlink_msg_attitude_get_roll(&msg)*180/PI;
          
          #ifdef  ConsolaMon
          Consola.println(F("Estabilizando gimbal"));
          #endif
          if(!configured){
            configureGimbal();
            configured = true;
          }
          stabilizeGimbal();
          
          #ifdef  ConsolaMon
          Consola.println(F("Actitud recibida"));
          Consola.print(F("YAW= "));
          Consola.print(yaw);
          Consola.println(" °");
                    
          Consola.print(F("PITCH= "));
          Consola.print(pitch);
          Consola.println(F(" °"));
            
          Consola.print(F("ROLL= "));
          Consola.print(roll);
          Consola.println(F(" °"));
          #endif
          
          break;
        }
        case MAVLINK_MSG_ID_BATTERY_STATUS: {  //ID del mensaje 147
          
          carga_bateria= mavlink_msg_battery_status_get_battery_remaining(&msg);
          #ifdef  ConsolaMon
          Consola.print(F("Carga remanente= "));
          Consola.print(carga_bateria);
          Consola.println(F(" %"));
          #endif
          
          break;
        }
        case MAVLINK_MSG_ID_MISSION_CURRENT: {  //ID del mensaje #42
          
          //requestMSG(MAVLINK_MSG_ID_MISSION_CURRENT,1e6);

          break;
        }

       
        case MAVLINK_MSG_ID_SYS_STATUS: { // ID del mensaje 1
        
          //Consola.println("Estado del sistema recibido");

          break;
        }
        
        case MAVLINK_MSG_ID_ATTITUDE_TARGET:{ //ID del mensaje 83
        

          break;
        }
        case MAVLINK_MSG_ID_HEARTBEAT :{ //ID del mensaje 0
          uint8_t PX4_STATUS = mavlink_msg_heartbeat_get_system_status(&msg);
          

          #ifdef  ConsolaMon
          Consola.println(F("Heartbeat recibido, estado "));
          #endif
          
          
          if(PX4_STATUS==MAV_STATE_ACTIVE){
          PIXHAWK_STATE = PIXHAWK_MISION;
          #ifdef  ConsolaMon
          Consola.println(F("ACTIVO"));
          #endif
           
          }

          if(PX4_STATUS==MAV_STATE_STANDBY){
          PIXHAWK_STATE = PIXHAWK_STANDBY;  
          #ifdef  ConsolaMon
          Consola.println(F("STANDBY"));
          #endif            
          }
          
          
          break;
        }
        
        case MAVLINK_MSG_ID_PING :{ //ID del mensaje 4
        
          break;

        }


        case MAVLINK_MSG_ID_GLOBAL_POSITION_INT :{ //ID del mensaje #33

          Timestamp_boot_PX= mavlink_msg_global_position_int_get_time_boot_ms(&msg); // Timestamp (milisegundos desde el booteo del sistema)
                              
          latitud_int_1E7 = mavlink_msg_global_position_int_get_lat(&msg); // Latitud, expresada en grados * 1E7
          longitud_int_1E7 = mavlink_msg_global_position_int_get_lon(&msg); // Longitud, expresada en grados * 1E7
          altitud_int = mavlink_msg_global_position_int_get_alt(&msg); // Altitud en metros
          
          #ifdef GPSprueba
          //Consola.print(F("Latitud: "));
          Consola.print("GLOBAL_POSITION_INT (lat_lon_alt_tiempo: ");
          
          Consola.print(latitud_int_1E7);
          Consola.print("\t");
          //Consola.println(F(" grados * 1E7"));
              
          //Consola.print(F("Longitud: "));
          Consola.print(longitud_int_1E7);
          Consola.print("\t");
          //Consola.println(F(" grados * 1E7"));
    
          //Consola.print(F("Altitud: "));
          Consola.print(altitud_int);
          Consola.println();

          #endif
          //44444444444444444
          break;
    
        }


        case MAVLINK_MSG_ID_GPS_RAW_INT :{ //ID del mensaje #24

          Timestamp_boot_PX= mavlink_msg_global_position_int_get_time_boot_ms(&msg); // Timestamp (milisegundos desde el booteo del sistema)
                              
          latitud_int_1E7 = mavlink_msg_gps_raw_int_get_lat(&msg); // Latitud, expresada en grados * 1E7
          longitud_int_1E7 = mavlink_msg_gps_raw_int_get_lon(&msg); // Longitud, expresada en grados * 1E7
          altitud_int = mavlink_msg_gps_raw_int_get_alt(&msg); // Altitud en metros
          time_usec=mavlink_msg_gps_raw_int_get_time_usec(&msg); // tiempo en us UNIX o desde el booteo
          #ifdef GPSprueba
          //Consola.print(F("Latitud: "));
          Consola.print("GPS_RAW_INT (lat_lon_alt_tiempo: ");
          
          Consola.print(latitud_int_1E7);
          Consola.print("\t");
          //Consola.println(F(" grados * 1E7"));
              
          //Consola.print(F("Longitud: "));
          Consola.print(longitud_int_1E7);
          Consola.print("\t");
          //Consola.println(F(" grados * 1E7"));
    
          //Consola.print(F("Altitud: "));
          Consola.print(altitud_int);
          Consola.println();
         
          #endif
          //44444444444444444
          break;
    
        }


       
        case MAVLINK_MSG_ID_COMMAND_ACK :{ //ID del mensaje 77
          #ifdef  ConsolaMon
          Consola.print(F("Command Acknowledge recibido, t= "));
          Consola.println(millis()/1000);
          #endif
          result_command_ack= mavlink_msg_command_ack_get_result(&msg);
          switch(result_command_ack) {
            case MAV_RESULT_ACCEPTED:{
              #ifdef  ConsolaMon
              Consola.println(F("MAV_RESULT_ACCEPTED")); //Command ACCEPTED and EXECUTED
              #endif
              break;
            }
            case MAV_RESULT_TEMPORARILY_REJECTED:{
              #ifdef  ConsolaMon
              Consola.println(F("MAV_RESULT_TEMPORARILY_REJECTED")); //Command TEMPORARY REJECTED/DENIED
              #endif
              break;
            }
            case MAV_RESULT_DENIED:{
              #ifdef  ConsolaMon
              Consola.println(F("MAV_RESULT_DENIED")); //Command PERMANENTLY DENIED
              #endif
              break;
            }
            case MAV_RESULT_UNSUPPORTED:{
              #ifdef  ConsolaMon
              Consola.println(F("MAV_RESULT_UNSUPPORTED")); //Command UNKNOWN/UNSUPPORTED
              #endif
              break;
            }
            case MAV_RESULT_FAILED:{
              #ifdef  ConsolaMon
              Consola.println(F("MAV_RESULT_FAILED")); //Command executed, but failed
              #endif
              break;
            }
            default:
              #ifdef  ConsolaMon
              Consola.println(F("Resultado no valido"));
              #endif
              break;
                         
          } // result
        break;
        } // case del 77
        
        case MAVLINK_MSG_ID_MISSION_COUNT :{ // #44
         count = mavlink_msg_mission_count_get_count(&msg);
         #ifdef  ConsolaMon
         Consola.print(F("La mision tiene "));
         Consola.print(count);
         Consola.println(F(" elementos"));
         #endif
         break;
        } // case del 44

         
        case MAVLINK_MSG_ID_MISSION_ITEM :{ // #39

          // Mision recibida
          
          uint16_t seq = mavlink_msg_mission_item_get_seq(&msg);
          #ifdef  ConsolaMon
          Consola.print("Item ");
          Consola.print(seq);
          Consola.println(F(" recibido"));
          #endif
          // latitud
          double lat = mavlink_msg_mission_item_get_x(&msg);
          // longitud
          double lon = mavlink_msg_mission_item_get_y(&msg);
          // altitud
          double alt = mavlink_msg_mission_item_get_z(&msg);
          #ifdef  ConsolaMon
          Consola.print(F("Latitud "));
          Consola.print(lat);
          Consola.println(F(" grados * 1E7"));
          Consola.print(F("Longitud "));
          Consola.print(lon);
          Consola.println(F(" grados * 1E7")); 
          Consola.print(F("Altitud "));
          Consola.print(alt);
          Consola.println(F(" metros"));
          #endif
          break;

        }

        case MAVLINK_MSG_ID_EXTENDED_SYS_STATE:{ // ID del mensaje 126
          
          Pixhawk_landed_state_previous; 
          Pixhawk_landed_state = mavlink_msg_extended_sys_state_get_landed_state(&msg);

          #ifdef ConsolaMon
          Consola.print(F("Estado extendido del dron: "));
          #endif
          switch(Pixhawk_landed_state) {
            
            case MAV_LANDED_STATE_UNDEFINED:{
              #ifdef ConsolaMon
              Consola.println(F("Indefinido"));
              #endif
              break;  
            }
            case MAV_LANDED_STATE_ON_GROUND:{
              #ifdef ConsolaMon
              Consola.println(F("En tierra"));
              #endif
              if(Pixhawk_landed_state_previous==MAV_LANDED_STATE_IN_AIR){
                flg_LandingDetected=true;
              }
              break;  
            }
            case MAV_LANDED_STATE_IN_AIR:{
              #ifdef ConsolaMon
              Consola.println(F("En aire"));
              #endif
              if(Pixhawk_landed_state_previous==MAV_LANDED_STATE_IN_AIR){
                flg_TakeOffDetected=true;
              }
              break;  
            }
          Pixhawk_landed_state_previous=Pixhawk_landed_state;
          
          }
          
          
          break;            
        }


        case MAVLINK_MSG_ID_HOME_POSITION:{ // ID del mensaje 242
          
          latitud_home_int_1E7 = mavlink_msg_home_position_get_latitude(&msg);
          longitud_home_int_1E7 = mavlink_msg_home_position_get_longitude (&msg);
          altitud_home_int_mm = mavlink_msg_home_position_get_altitude(&msg);

          #ifdef  ConsolaMon
            Consola.println(F("Posicion Home recibida"));
            Consola.print(F("LATITUD = "));
            Consola.print(latitud_home_int_1E7);
            Consola.println(F("*1E-7 °"));
            Consola.print(F("LONGITUD = "));
            Consola.print(longitud_home_int_1E7);
            Consola.println(F("*1E-7 °"));
            Consola.print(F("ALTITUD = "));
            Consola.print(altitud_home_int_mm);
            Consola.println(F("mm"));          
           #endif
          
          break;            
        }


        case MAVLINK_MSG_ID_STATUSTEXT:{ // ID del mensaje 253
          
          statusTextSeverity = mavlink_msg_statustext_get_severity(&msg);
          statusTextTextLen = mavlink_msg_statustext_get_text(&msg,&statusTextText[0]);
          
          #ifdef  ConsolaMon
            Consola.println(F("STATUSTEXT RECIBIDO"));
            Consola.print(F("SEVERITY: "));
            Consola.println(statusTextSeverity);
            Consola.print(F("TEXT: "));
            for(int i = 0;i<statusTextTextLen;i++){
              Consola.print(statusTextText);
            }
            Consola.println();
           #endif
          
          break;            
        }

        case MAVLINK_MSG_ID_HIL_ACTUATOR_CONTROLS:{ // ID del mensaje 93
          shutup(MAVLINK_MSG_ID_HIL_ACTUATOR_CONTROLS);
          break;            
        }

        case MAVLINK_MSG_ID_HIL_CONTROLS:{ // ID del mensaje 91
          shutup(MAVLINK_MSG_ID_HIL_CONTROLS);
          break;            
        }

        case MAVLINK_MSG_ID_VFR_HUD:{ // ID del mensaje 74
          shutup(MAVLINK_MSG_ID_VFR_HUD);
          break;            
        }
        
        default:
          #ifdef  ConsolaMon
          //Consola.println(F("nada"));
          #endif
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
  PX4.write(buf, len); // manda el mensaje 
  #ifdef  ConsolaMon
  Consola.print(F("Heartbeat enviado, tiempo t = ")); // Imprime que lo mandó
  Consola.println(millis());
  #endif
 
  dataSend();
  
}

void pingOnBoard() { 
  mavlink_message_t msg; // crea un mensaje
  uint8_t buf[MAVLINK_MAX_PACKET_LEN]; // crea un buffer para guardarlo
  mavlink_msg_ping_pack(system_id, component_id, &msg,0,0,1,1); // Arma el mensaje
  uint16_t len = mavlink_msg_to_send_buffer(buf, &msg); //guarda el mensaje en el buffer y devuelve el largo del mensaje (payload+no payload) 
  PX4.write(buf, len); // manda el mensaje 
  #ifdef  ConsolaMon
  Consola.print(F("PING ENVIADO ")); // Imprime que lo mandó
  Consola.println(millis());
  #endif

}

void serialControl(){
  mavlink_message_t msg; // crea un mensaje
  uint8_t buf[MAVLINK_MAX_PACKET_LEN]; // crea un buffer para guardarlo

  mavlink_message_t msg_ob; // crea un mensaje
  uint8_t data_sc[70]; // crea un buffer para guardarlo
  mavlink_msg_ping_pack(system_id, component_id, &msg_ob,0,0,253,253); // Arma el mensaje
  uint8_t count_sc = mavlink_msg_to_send_buffer(data_sc, &msg_ob); //guarda el mensaje en el buffer y devuelve el largo del mensaje (payload+no payload) 

 
  mavlink_msg_serial_control_pack(system_id, component_id, &msg,SERIAL_CONTROL_DEV_TELEM1,0,1e3,0,count_sc,data_sc); // Arma el mensaje
  uint16_t len = mavlink_msg_to_send_buffer(buf, &msg); //guarda el mensaje en el buffer y devuelve el largo del mensaje (payload+no payload) 
  PX4.write(buf, len); // manda el mensaje 
  #ifdef  ConsolaMon
  Consola.println(F("Serial Control")); // Imprime que lo mandó
  #endif
}




// Callbacks

// Heartbeat
void heartBeatCallback() {
  sendHeartBeat();
  //serialControl();
}
void shutup() {
  for(int i=1; i < 290; i++){
    mavlink_message_t msg;
    uint8_t buf[MAVLINK_MAX_PACKET_LEN];
    mavlink_msg_command_long_pack(system_id, component_id, &msg,target_system, target_component, MAV_CMD_SET_MESSAGE_INTERVAL, confirmation, i, disable, param3, param4, xf, yf, zf);
    uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
    PX4.write(buf, len);
  }//for
}//shutup

void shutup(int MSG_ID_SHUTUP) {
  mavlink_message_t msg;
  uint8_t buf[MAVLINK_MAX_PACKET_LEN];
  mavlink_msg_command_long_pack(system_id, component_id, &msg,target_system, target_component, MAV_CMD_SET_MESSAGE_INTERVAL, confirmation, MSG_ID_SHUTUP, disable, param3, param4, xf, yf, zf);
  uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
  PX4.write(buf, len);
}//shutup


 
void requestGPS(double t){
  mavlink_message_t msg;
  uint8_t buf[MAVLINK_MAX_PACKET_LEN];
  mavlink_msg_command_long_pack(system_id, component_id, &msg,target_system, target_component, MAV_CMD_SET_MESSAGE_INTERVAL, confirmation, MAVLINK_MSG_ID_GLOBAL_POSITION_INT, t, param3, param4, xf, yf, zf);
  uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
  PX4.write(buf, len);
}//requestGPS

/*void missionRequestListCallBack(){
  mavlink_message_t msg;
  uint8_t buf[MAVLINK_MAX_PACKET_LEN];
  mavlink_msg_mission_request_list_pack(system_id, component_id, &msg, target_system, target_component);
  uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
  PX4.write(buf, len);
}*/

void waypointrequest(uint16_t seq){
  mavlink_message_t msg;
  uint8_t buf[MAVLINK_MAX_PACKET_LEN];
  mavlink_msg_mission_request_pack(system_id, component_id, &msg, target_system, target_component,seq);
  uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
  PX4.write(buf, len);
}

void sendwaypointack(){
  mavlink_message_t msg;
  uint8_t buf[MAVLINK_MAX_PACKET_LEN];
  mavlink_msg_mission_ack_pack(system_id, component_id, &msg, target_system, target_component,MAV_RESULT_ACCEPTED);
  uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
  PX4.write(buf, len);
}

void sendwaypointcount(uint16_t count){
  mavlink_message_t msg;
  uint8_t buf[MAVLINK_MAX_PACKET_LEN];
  mavlink_msg_mission_count_pack(system_id, component_id, &msg, target_system, target_component,count);
  uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
  PX4.write(buf, len);
}


    
void sendwaypointInt(uint16_t seq, uint8_t frame, uint16_t command,uint8_t current, uint8_t autocontinue, double param1, double param2, double param3, double param4, int32_t x, int32_t y, float z){
  mavlink_message_t msg;
  uint8_t buf[MAVLINK_MAX_PACKET_LEN];
  mavlink_msg_mission_item_int_pack(system_id, component_id, &msg, target_system, target_component,seq,frame,command,current,autocontinue,param1,param2,param3,param4,x,y,z);
  uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
  PX4.write(buf, len);
}

void sendwaypointFloat(uint16_t seq, uint8_t frame, uint16_t command,uint8_t current, uint8_t autocontinue, double param1, double param2, double param3, double param4, float x, float y, float z){
  mavlink_message_t msg;
  uint8_t buf[MAVLINK_MAX_PACKET_LEN];
  mavlink_msg_mission_item_int_pack(system_id, component_id, &msg, target_system, target_component,seq,frame,command,current,autocontinue,param1,param2,param3,param4,x,y,z);
  uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
  PX4.write(buf, len);
}

/*  Encontrar el comando correcto para Landing
void landnow(uint16_t seq, uint8_t frame, uint16_t command,uint8_t current, uint8_t autocontinue, double param1, double param2, double param3, double param4, float x, float y, float z){
  mavlink_message_t msg;
  uint8_t buf[MAVLINK_MAX_PACKET_LEN];
  mavlink_msg_mission_item_int_pack(system_id, component_id, &msg, target_system, target_component,seq,frame,command,current,autocontinue,param1,param2,param3,param4,x,y,z);
  uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
  PX4.write(buf, len);
}*/


   
void clearall(){
  mavlink_message_t msg;
  uint8_t buf[MAVLINK_MAX_PACKET_LEN];
  mavlink_msg_mission_clear_all_pack(system_id, component_id, &msg, target_system, target_component);
  uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
  PX4.write(buf, len);
}

void waitfor(uint8_t MSG_ID, uint32_t timeout){
  uint32_t timestamp = millis();
  received=false;
  mavlink_message_t msg; 
  mavlink_status_t stat;
  while((received == false) && (millis() < timestamp+timeout)){ // TODO este while esta mal
    while(PX4.available()>0) { 
      uint8_t c = PX4.read();
      if(mavlink_parse_char(MAVLINK_COMM_0, c, &msg, &stat)) { 
        switch(msg.msgid) {

        case MAVLINK_MSG_ID_STATUSTEXT:{ // ID del mensaje #253
          
          statusTextSeverity = mavlink_msg_statustext_get_severity(&msg);
          statusTextTextLen = mavlink_msg_statustext_get_text(&msg,&statusTextText[0]);
          
          #ifdef  ConsolaMon
            Consola.println(F("STATUSTEXT RECIBIDO"));
            Consola.print(F("SEVERITY: "));
            Consola.println(statusTextSeverity);
            Consola.print(F("TEXT: "));
            for(int i = 0;i<statusTextTextLen;i++){
              Consola.print(statusTextText);
            }
            Consola.println();
           #endif
          
          break;            
        }

        case MAVLINK_MSG_ID_MISSION_REQUEST_INT: { // #51
          if(MSG_ID==MAVLINK_MSG_ID_MISSION_REQUEST_INT){
            received = true;
            uint16_t sequ = mavlink_msg_mission_request_get_seq(&msg);
            #ifdef  ConsolaMon
            Consola.print(F("mensaje esperado recibido, ID ="));
            Consola.println(msg.msgid);
            Consola.println(sequ);
            #endif
            seq=sequ;
          }
          break;
        }
          
          case MAVLINK_MSG_ID_MISSION_REQUEST: {  // #40
            if(MSG_ID==MAVLINK_MSG_ID_MISSION_REQUEST){
              received = true;
              uint16_t sequ = mavlink_msg_mission_request_get_seq(&msg);
              seq=sequ;
              #ifdef ConsolaMon
              Consola.print(F("mensaje esperado recibido, ID ="));
              Consola.println(msg.msgid);
              Consola.println(sequ);
              #endif
            }
            else{
              #ifdef ConsolaMon
              Consola.print(F("mensaje diferente, ID ="));
              Consola.println(msg.msgid);
              #endif
            }
            break;            
          }//case
          case MAVLINK_MSG_ID_MISSION_ACK: {
            if(MSG_ID==MAVLINK_MSG_ID_MISSION_ACK){
              received = true;
              #ifdef  ConsolaMon
              Consola.print(F("mensaje esperado recibido, ID ="));
              Consola.println(msg.msgid);
              #endif
            }//if
            else{
              #ifdef  ConsolaMon
              Consola.print(F("mensaje diferente, ID ="));
              Consola.println(msg.msgid);
              #endif
            }//else
            break;            
          }//case
          case MAVLINK_MSG_ID_MISSION_COUNT:{
            if(MSG_ID==MAVLINK_MSG_ID_MISSION_COUNT){
              received = true;
              #ifdef  ConsolaMon
              Consola.print(F("mensaje esperado recibido, ID ="));
              Consola.println(msg.msgid);
              count = mavlink_msg_mission_count_get_count(&msg);
              #endif
            }//if
            else{
              #ifdef  ConsolaMon
              Consola.print(F("mensaje diferente, ID ="));
              Consola.println(msg.msgid);
              #endif
            }//else
            break;            
          }//case
          case MAVLINK_MSG_ID_MISSION_CURRENT:{   //#42
            if(MSG_ID==MAVLINK_MSG_ID_MISSION_CURRENT){
              received = true;
              #ifdef  ConsolaMon
              Consola.print(F("mensaje esperado recibido, ID ="));
              Consola.println(msg.msgid);
              count = mavlink_msg_mission_count_get_count(&msg);
              #endif
            }//if
            else{
              #ifdef  ConsolaMon
              Consola.print(F("mensaje diferente, ID ="));
              Consola.println(msg.msgid);
              #endif
            }//else
            break;            
          }//case
          case MAVLINK_MSG_ID_GLOBAL_POSITION_INT :{ // #33
            if(MSG_ID==MAVLINK_MSG_ID_GLOBAL_POSITION_INT){
              GPSLock = 1;
              received = true;
              #ifdef  ConsolaMon
              Consola.print(F("mensaje esperado recibido, ID ="));
              Consola.println(msg.msgid);
              #endif
              uint32_t pos_time_boot_ms= mavlink_msg_global_position_int_get_time_boot_ms(&msg); // Timestamp (milisegundos desde el booteo del sistema
              int32_t pos_lat=mavlink_msg_global_position_int_get_lat(&msg); // Latitud, expresada en grados * 1E7
              int32_t pos_lon=mavlink_msg_global_position_int_get_lon(&msg); // Longitud, expresada en grados * 1E7
              int32_t pos_alt=mavlink_msg_global_position_int_get_alt(&msg); // Altitud en metros, expresada en milimetros, AMSL
              int32_t pos_relative_alt=mavlink_msg_global_position_int_get_relative_alt(&msg); // Altitud sobre el suelo en metros, expresada como * 1000 (milimetros)
              int16_t pos_vx=mavlink_msg_global_position_int_get_vx(&msg); // Velocidad en tierra X (Latitud, norte positivo), expresada en m/s * 100
              int16_t pos_vy=mavlink_msg_global_position_int_get_vy(&msg); // Velocidad en tierra Y (Longitud, este positivo), expresada en m/s * 100
              int16_t pos_vz=mavlink_msg_global_position_int_get_vz(&msg); // Velocidad en tierra Z (Altitud, abajo positivo), expresada en m/s * 100
              uint16_t pos_hdg; // Direccion del vehiculo (angulo yaw) en grados * 100, 0.0..359.99 degrees. su no se sabe se setea en UINT16_MAX
              xi=pos_lat;
              yi=pos_lon;
              zi=pos_alt;
              #ifdef  ConsolaMon
              Consola.print(F("Latitud: "));
              Consola.print(xi);
              Consola.println(F(" grados * 1E7"));
              Consola.print(F("Longitud: "));
              Consola.print(yi);
              Consola.println(F(" grados * 1E7"));
              Consola.print(F("Altitud: "));
              Consola.print(zi);
              Consola.println(F(" milimetros"));
              #endif
            }//if
            else{
              #ifdef  ConsolaMon
              Consola.print(F("mensaje diferente, ID ="));
              Consola.println(msg.msgid);
              #endif  
            }//else
            break;
          }//case
          case MAVLINK_MSG_ID_MISSION_ITEM:{
            if(MSG_ID==MAVLINK_MSG_ID_MISSION_ITEM){
              received = true;
              #ifdef  ConsolaMon
              Consola.print(F("mensaje esperado recibido, ID ="));
              Consola.println(msg.msgid);
              #endif
              uint16_t seq = mavlink_msg_mission_item_get_seq(&msg);
              #ifdef  ConsolaMon
              Consola.print(F("Item "));
              Consola.print(seq);
              Consola.println(F(" recibido"));
              #endif
              // latitud
              double lat = mavlink_msg_mission_item_get_x(&msg);
              // longitud
              double lon = mavlink_msg_mission_item_get_y(&msg);
              // altitud
              double alt = mavlink_msg_mission_item_get_z(&msg);
              #ifdef  ConsolaMon
              Consola.print(F("Latitud "));
              Consola.print(lat*1E7);
              Consola.println(F(" grados * 1E7"));
              Consola.print(F("Longitud "));
              Consola.print(lon*1E7);
              Consola.println(F(" grados * 1E7")); 
              Consola.print(F("Altitud "));
              Consola.print(alt);
              Consola.println(F(" metros"));
              #endif
            }//if
            else{
              #ifdef  ConsolaMon
              Consola.print(F("mensaje diferente, ID ="));
              Consola.println(msg.msgid);
              #endif
            }//else
            break;            
          }//case
          default:
          #ifdef  ConsolaMon
          Consola.print(F("mensaje diferente, ID ="));
          Consola.println(msg.msgid);
          #endif
          break;
        } //switch
      } //if
    } //while
  } //while
  if(millis() >= timestamp+timeout){
    #ifdef  ConsolaMon
    Consola.print(F("Salio por timeout esperando mensaje #"));
    Consola.println(MSG_ID);
    #endif
  } //if
} //waitfor


void missionrequest(){
  mavlink_message_t msg;
  uint8_t buf[MAVLINK_MAX_PACKET_LEN];
  mavlink_msg_mission_request_list_pack(system_id, component_id, &msg, target_system, target_component);
  uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
  PX4.write(buf, len);
}//missionrequest

void missionack(){
  mavlink_message_t msg;
  uint8_t type =MAV_MISSION_ACCEPTED; // 0 significa OK
  uint8_t buf[MAVLINK_MAX_PACKET_LEN];
  mavlink_msg_mission_ack_pack(system_id, component_id, &msg, target_system, target_component, type);
  uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
  PX4.write(buf, len);
  Consola.println(F("Mision recibida OK"));
}//waypointack

void setHome(){
  mavlink_message_t msg;
  uint8_t buf[MAVLINK_MAX_PACKET_LEN];
  mavlink_msg_set_home_position_pack(system_id, component_id, &msg,target_system,xi,yi,zi,0,0,0,0,0,0,0);
  uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
  PX4.write(buf, len);
}

void arm(){
  mavlink_message_t msg;
  uint8_t buf[MAVLINK_MAX_PACKET_LEN];
  mavlink_msg_command_long_pack(system_id, component_id, &msg,target_system,target_component,MAV_CMD_COMPONENT_ARM_DISARM,confirmation,ARM,0,0,0,0,0,0);
  uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
  PX4.write(buf, len);
}

void disarm(){
  mavlink_message_t msg;
  uint8_t buf[MAVLINK_MAX_PACKET_LEN];
  mavlink_msg_command_long_pack(system_id, component_id, &msg,target_system,target_component,MAV_CMD_COMPONENT_ARM_DISARM,confirmation,DISARM,0,0,0,0,0,0);
  uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
  PX4.write(buf, len);
}


void setMode(uint16_t mode){//
  mavlink_message_t msg;
  uint8_t buf[MAVLINK_MAX_PACKET_LEN];
  mavlink_msg_command_long_pack(system_id, component_id, &msg,target_system,target_component,MAV_CMD_DO_SET_MODE,confirmation,mode,0,0,0,0,0,0);
  uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
  PX4.write(buf, len);
}

boolean EnviarLandNow(){
  
  
  
  command=MAV_CMD_DO_LAND_START; // CMD_ID = 189
  param1=0;   
  param2=0; 
  param3=0;
  param4=0;
  param5=0;
  param6=0; 
  param7=0;

  // Borro la mision actual
  //setMode(MAV_MODE_AUTO_ARMED);
  //disarm();
  // Espero un lock del GPS
  if(PIXHAWK_STATE==PIXHAWK_STANDBY){
    #ifdef  ConsolaMon
    Consola.println(F("Dron ya esta aterrizado"));
    #endif  
    return false;
  }

  // Comienza el intercamibo de mensajes (se carga la mision)

  received=false;
  sendwaypointreq=0;
  
  while((!received)and(sendwaypointreq<sendwaypointmax)){
    
    #ifdef ConsolaMon
    Consola.println();
    Consola.print("Enviando comando de aterrizar: ");
    Consola.print(1);
    Consola.print("por vez numero ");
    Consola.println(sendwaypointreq+1);
    #endif
        
    sendwaypointcount(1);
    
    #ifdef CoordenadasEnFloat
    waitfor(MAVLINK_MSG_ID_MISSION_REQUEST, MISSIONREQUESTTIMEOUT);
    #else
    waitfor(MAVLINK_MSG_ID_MISSION_REQUEST_INT, MISSIONREQUESTTIMEOUT); 
    #endif

    // Punto 0
    
    current=0; // Punto actual
    autocontinue  = 1; // True 
    param1=0; // Tiempo de hold  
    param2=0.5; // Radio de aceptacion en metros
    param3=0; // 0 para pasar por el WP
    param4=0; // YAW deseado
    #ifdef CoordenadasEnFloat
    sendwaypointFloat(seq,frame,command,current,autocontinue,param1,param2,param3,param4,param5,param6);
    #else
    sendwaypointInt(seq,frame,command,current,autocontinue,param1,param2,param3,param4,param4,param5,param6);
    #endif
    sendwaypointreq++;
  }

  if(!received){
    #ifdef ConsolaMon
    Consola.println();
    Consola.println("No fue posible enviar el comando de aterrizaje.");
    #endif    
    return false;
  }

  waitfor(MAVLINK_MSG_ID_MISSION_ACK, MISSIONACKTIMEOUT);
  // Pide que se ponga en modo mision
  return true;

}




void datarecive_sensor(){
  int ii = 1;
  r = (serial_control_data[ii] << 8) + serial_control_data[ii + 1];
  ii = ii + 2;
  s = (serial_control_data[ii] << 8) + serial_control_data[ii + 1];
  ii = ii + 2;
  sensor_act[0] = (serial_control_data[ii] & 0x01);
  sensor_act[1] = (serial_control_data[ii] & 0x02);
  sensor_act[2] = (serial_control_data[ii] & 0x04);
  sensor_act[3] = (serial_control_data[ii] & 0x08);
  ii++;
  sensor_dist[0] = (serial_control_data[ii] << 8) + serial_control_data[ii + 1];
  ii = ii + 2;
  sensor_dist[1] = (serial_control_data[ii] << 8) + serial_control_data[ii + 1];
  ii = ii + 2;
  sensor_dist[2] = (serial_control_data[ii] << 8) + serial_control_data[ii + 1];
  ii = ii + 2;
  sensor_dist[3] = (serial_control_data[ii] << 8) + serial_control_data[ii + 1];
}


void datarecive_error(){    //1:Resync; 2:SD; 3:Mavlink;
    switch(serial_control_data[1]){
        case 1:{
        memset(reset_onboard_error,'\0', 15);
        sprintf(reset_onboard_error, "Resync error");
        break;
        }
        case 2:{
        memset(reset_onboard_error,'\0', 15);
        sprintf(reset_onboard_error, "SD error");
        break;
        }
        case 3:{
        memset(reset_onboard_error,'\0', 15);
        sprintf(reset_onboard_error, "Mavlink error");
        break;
        }
    }
}




void requestMSG(int MSG_ID_REQUEST,float timeus) {
  mavlink_message_t msg;
  uint8_t buf[MAVLINK_MAX_PACKET_LEN];
  mavlink_msg_command_long_pack(system_id, component_id, &msg,target_system, target_component, MAV_CMD_SET_MESSAGE_INTERVAL, confirmation, MSG_ID_REQUEST, timeus, param3, param4, xf, yf, zf);
  uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
  PX4.write(buf, len);
}



void configureGimbal(){
  mavlink_message_t msg;
  uint8_t buf[MAVLINK_MAX_PACKET_LEN];
  mavlink_msg_command_long_pack(system_id, component_id, &msg,target_system, target_component, MAV_CMD_DO_MOUNT_CONFIGURE, confirmation, MAV_MOUNT_MODE_MAVLINK_TARGETING, 1, 1,0,0,0, 0);
  uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
  //PX4.write(buf, len);
}
         
void stabilizeGimbal(){
  mavlink_message_t msg;
  uint8_t buf[MAVLINK_MAX_PACKET_LEN];
  mavlink_msg_command_long_pack(system_id, component_id, &msg,target_system, target_component, MAV_CMD_DO_MOUNT_CONTROL, confirmation, pitch*PI*1.5, -roll*PI*0.9, 0,0,0,0,MAV_MOUNT_MODE_MAVLINK_TARGETING);
  uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
  PX4.write(buf, len);
}
          
          
void dataSend(){

  digitalWrite(PXEnablePin,HIGH); // Se desconecta PX
  delay(10);
  digitalWrite(ArdEnablePin,LOW); // Se conecta Arduino

 
  mavlink_message_t msg; // crea un mensaje
  uint8_t buf[MAVLINK_MAX_PACKET_LEN]; // crea un buffer para guardarlo

  mavlink_msg_file_transfer_protocol_pack(system_id, component_id, &msg, 0, 0, 0, dataToGCS);
  uint16_t len = mavlink_msg_to_send_buffer(buf, &msg); //guarda el mensaje en el buffer y devuelve el largo del mensaje (payload+no payload) 
  PX4.write(buf, len); // manda el mensaje 
  #ifdef  ConsolaMon
  Consola.print(F("Enviando data a la GCS ")); // Imprime que lo mandó
  #endif


  
  
  delay(50); // Esto tiene que estar porque si no Arduino se suicida
  digitalWrite(ArdEnablePin,HIGH); // Se desconecta Arduino
//  delay(10);
//  digitalWrite(PXEnablePin,LOW); // Se conecta PX
 


}



/*

void print_date(uint64_t time_unix) {
  time_unix = time_unix - 10800000; //UTM-3
  int horario;
  horario = hour(time_unix);
  Consola.print(horario);
  Consola.print(":");
  horario = minute(time_unix);
  Consola.print(horario);
  Consola.print(":");
  horario = second(time_unix);
  Consola.print(horario);
  if(isAM(time_unix)){
     Consola.print("(AM)");
  }else{
    Consola.print("(PM)");
  }
  Consola.print("\t");
  switch(weekday(time_unix)){
    case 0:{
      Consola.print("(Sunday)");
      break;
    }
    case 1:{
      Consola.print("(Monday)");
      break;
    }
    case 2:{
      Consola.print("(Tuesday)");
      break;
    }
    case 3:{
      Consola.print("(Wednesday)");
      break;
    }
    case 4:{
      Consola.print("(Thursday)");
      break;
    }
    case 5:{
      Consola.print("(Friday)");
      break;
    }
    case 6:{
      Consola.print("(Saturday)");
      break;
    }
    default:
    break;
  }
  horario = day(time_unix);
  Consola.print(horario);
  Consola.print("/");
  horario = month(time_unix);
  Consola.print(horario);
  Consola.print("/");
  horario = year(time_unix);
  Consola.print(horario);
}*/
