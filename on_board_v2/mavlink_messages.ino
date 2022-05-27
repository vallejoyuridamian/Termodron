//Función de actualizacion del estado del Pixhawk
void up_date(){
  mavlink_message_t msg; 
  mavlink_status_t stat;
  if(antenasolicitada){
    while(serant.available()>0) {     
      uint8_t c = serant.read();
      if(mavlink_parse_char(MAVLINK_COMM_0, c, &msg, &stat)) {
        manageMSG(msg);   //Chequeo de mensajes disponibles provenientes del Pixhawk
      }
    }
  }else{
    while(sermav.available()>0) {     
      uint8_t c = sermav.read();
      if(mavlink_parse_char(MAVLINK_COMM_0, c, &msg, &stat)) {
        manageMSG(msg);   //Chequeo de mensajes disponibles provenientes del Pixhawk
      }
    }
  }
  if(mission_received){
    //bat_check();        //Chequeo de la carga de la batería
    //Nacho lepton_status();    //Chequeo del estado de la Flir Lepton
    #ifdef SD_present
      sd_log_save();    //Se actualiza el log.txt
    #endif
  }
  #ifdef debugging
  //sermon.println(F("Actualizado"));
  #endif
}

//Función para solicitar al Pixhawk todos los waypoint de la mision
void request_all_mission(){
  requestMSG(per_mav_msg/2, MAVLINK_MSG_ID_HOME_POSITION);//
  waitfor(MAVLINK_MSG_ID_HOME_POSITION, timeout_mav_msg);//
  //condicion si no encuentra el mensaje de Home
  /*#ifdef waypoint_int
  missionrequestint();  //no existe un msj para request list en int
  #else*/
  missionrequest();
  //#endif
  waitfor(MAVLINK_MSG_ID_MISSION_COUNT, timeout_mav_msg);
  for (int i = 0; i < count; i++){
    #ifdef waypoint_int
    waypointrequestint(i);
    waitfor(MAVLINK_MSG_ID_MISSION_ITEM_INT, timeout_mav_msg);
    while (received == 0){
      waypointrequestint(i);
      waitfor(MAVLINK_MSG_ID_MISSION_ITEM_INT, timeout_mav_msg);
    }
    #else
    waypointrequest(i);
    waitfor(MAVLINK_MSG_ID_MISSION_ITEM, timeout_mav_msg);
    while (received == 0){
      waypointrequest(i);
      waitfor(MAVLINK_MSG_ID_MISSION_ITEM, timeout_mav_msg);
    }
    #endif
  }
  #ifdef SD_present
  sd_mission_save();
  #endif
  missionack();
  shutup(MAVLINK_MSG_ID_HOME_POSITION);     //requestMSG( 0, MAVLINK_MSG_ID_HOME_POSITION);     //Dejar de pedirlo
}

//Funcion para Volver a enviar al Pixhawk su mision original
void recover_mission() {
    clearall();
    int tryrecovery = 0;
    received = 0;
    count = count - auxseq;
    while(received == 0){
      if (tryrecovery == 5){
        state = 6;
        new_wp.lat = home_wp.lat;
        new_wp.lon = home_wp.lon;
        new_wp.alt = actual_wp.alt;
        //funcion MAVLink para enviar el newwp
        auxseq = seq;
        #ifdef debugging
        sermon.println(F("Volviendo a Home."));
        #endif
        send_wp();
        break;
      }
      tryrecovery++;
      sendwaypointcount(count);
      sermon.print("count recovery mission:");
      sermon.println(count);
      #ifdef waypoint_int
      waitfor(MAVLINK_MSG_ID_MISSION_REQUEST_INT, timeout_mav_msg);
      #else
      waitfor(MAVLINK_MSG_ID_MISSION_REQUEST, timeout_mav_msg);
      #endif
      current = 1;
      autocontinue  = 1; // True 
      param1=0; // Tiempo de hold  
      param2=0; // Radio de aceptacion en metros
      param3=0; // 0 para pasar por el WP
      param4=0; // YAW deseado
      command  =  MAV_CMD_NAV_WAYPOINT; // CMD_ID = 16
      #ifdef waypoint_int
      sendwaypointint(0,frame,command,current,autocontinue,param1,param2,param3,param4,mission_wpsint[auxseq].latint,mission_wpsint[auxseq].lonint,mission_wpsint[auxseq].altint);
      mission_wpsint[0].latint = mission_wpsint[auxseq].latint;
      mission_wpsint[0].lonint = mission_wpsint[auxseq].lonint;
      mission_wpsint[0].altint = mission_wpsint[auxseq].altint;
      #else
      sendwaypoint(0,frame,command,current,autocontinue,param1,param2,param3,param4,mission_wps[auxseq].lat,mission_wps[auxseq].lon,mission_wps[auxseq].alt);
      mission_wps[0].lat = mission_wps[auxseq].lat;
      mission_wps[0].lon = mission_wps[auxseq].lon;
      mission_wps[0].alt = mission_wps[auxseq].alt;
      #endif
      
    }
    while(seq < (count-1)){
      #ifdef waypoint_int
      waitfor(MAVLINK_MSG_ID_MISSION_REQUEST_INT, timeout_mav_msg);
      #else
      waitfor(MAVLINK_MSG_ID_MISSION_REQUEST, timeout_mav_msg);
      #endif
      /*if(seq==auxseq){
        current = 1; // Punto actual TRUE
      }else{
        current = 0;
      }*/
      current = 0;
      autocontinue  = 1; // True 
      param1=0; // Tiempo de hold  
      param2=0; // Radio de aceptacion en metros
      param3=0; // 0 para pasar por el WP
      param4=0; // YAW deseado
      command  =  MAV_CMD_NAV_WAYPOINT; // CMD_ID = 16
      #ifdef waypoint_int
      sendwaypointint(seq,frame,command,current,autocontinue,param1,param2,param3,param4,mission_wpsint[auxseq+seq].latint,mission_wpsint[auxseq+seq].lonint,mission_wpsint[auxseq+seq].altint);
      mission_wpsint[seq].latint = mission_wpsint[auxseq+seq].latint;
      mission_wpsint[seq].lonint = mission_wpsint[auxseq+seq].lonint;
      mission_wpsint[seq].altint = mission_wpsint[auxseq+seq].altint;
      #else
      sendwaypoint(seq,frame,command,current,autocontinue,param1,param2,param3,param4,mission_wps[auxseq+seq].lat,mission_wps[auxseq+seq].lon,mission_wps[auxseq+seq].alt);
      mission_wps[seq].lat = mission_wps[auxseq+seq].lat;
      mission_wps[seq].lon = mission_wps[auxseq+seq].lon;
      mission_wps[seq].alt = mission_wps[auxseq+seq].alt;
      #endif
    }
    waitfor(MAVLINK_MSG_ID_MISSION_ACK, timeout_mav_msg);
    sendwaypointcurrent(0);
    waitfor(MAVLINK_MSG_ID_MISSION_CURRENT, timeout_mav_msg);
}

//Funcion para enviar un waypoint al cual el Pixhawk debe desplazarse
void send_wp() {
    clearall();
    sendwaypointcount(1);
    #ifdef waypoint_int
    waitfor(MAVLINK_MSG_ID_MISSION_REQUEST_INT, timeout_mav_msg);
    #else
    waitfor(MAVLINK_MSG_ID_MISSION_REQUEST, timeout_mav_msg);
    #endif
    //si acepta la mision
    if(received == 1){
      seq = 0;
      current = 1; // Punto actual
      autocontinue  = 1; // True 
      param1=0; // Tiempo de hold  
      param2=0; // Radio de aceptacion en metros
      param3=0; // 0 para pasar por el WP
      param4=0; // YAW deseado
      command  =  MAV_CMD_NAV_WAYPOINT; // CMD_ID = 16
      #ifdef waypoint_int
      sendwaypointint(seq,frame,command,current,autocontinue,param1,param2,param3,param4,new_wp.lat*1E7,new_wp.lon*1E7,5);
      #else
      sendwaypoint(seq,frame,command,current,autocontinue,param1,param2,param3,param4,new_wp.lat,new_wp.lon,5);
      #endif
      waitfor(MAVLINK_MSG_ID_MISSION_ACK, timeout_mav_msg);  
    }else{
      #ifdef debugging 
        sermon.println(F("Waypoint rejected")); 
      #endif
      recover_mission();
    }
    
}







void sendHeartBeat() { 
  mavlink_message_t msg; // crea un mensaje
  uint8_t buf[MAVLINK_MAX_PACKET_LEN]; // crea un buffer para guardarlo
  mavlink_msg_heartbeat_pack(system_id, component_id, &msg, system_type, autopilot_type, base_mode, custom_mode, system_status); // Arma el mensaje
  uint16_t len = mavlink_msg_to_send_buffer(buf, &msg); //guarda el mensaje en el buffer y devuelve el largo del mensaje (payload+no payload) 
  sermav.write(buf, len); // manda el mensaje
  #ifdef debugging 
  sermon.println(F("Heartbeat enviado")); 
  #endif
}

void clearall(){
  mavlink_message_t msg;
  uint8_t buf[MAVLINK_MAX_PACKET_LEN];
  mavlink_msg_mission_clear_all_pack(system_id, component_id, &msg, target_system, target_component);
  uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
  sermav.write(buf, len);
}

void shutup() {
  for(int i=1; i < 290; i++){
    mavlink_message_t msg;
    uint8_t buf[MAVLINK_MAX_PACKET_LEN];
    mavlink_msg_command_long_pack(system_id, component_id, &msg,target_system, target_component, MAV_CMD_SET_MESSAGE_INTERVAL, confirmation, i, -1, param3, param4, 0, 0, 0);
    uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
    sermav.write(buf, len);
  }
  #ifdef debugging
  sermon.println(F("Apaga msg"));
  #endif
}

void shutup(int i) {
  mavlink_message_t msg;
  uint8_t buf[MAVLINK_MAX_PACKET_LEN];
  mavlink_msg_command_long_pack(system_id, component_id, &msg,target_system, target_component, MAV_CMD_SET_MESSAGE_INTERVAL, confirmation, i, -1, param3, param4, 0, 0, 0);
  uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
  sermav.write(buf, len);
}

void setMode(uint16_t mode){
  mavlink_message_t msg;
  uint8_t buf[MAVLINK_MAX_PACKET_LEN];
  mavlink_msg_command_long_pack(system_id, component_id, &msg,target_system,target_component,MAV_CMD_DO_SET_MODE,confirmation,mode,0,0,0,0,0,0);
  uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
  sermav.write(buf, len);
}




//Para el pedido de los mensajes periodicos
void requestMSG(float t, int msgid){
  mavlink_message_t msg;
  uint8_t buf[MAVLINK_MAX_PACKET_LEN];
  mavlink_msg_command_long_pack(system_id, component_id, &msg,target_system, target_component, MAV_CMD_SET_MESSAGE_INTERVAL, confirmation, msgid, t, param3, param4, 0, 0, 0);
  uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
  sermav.write(buf, len);
}

//Para el envio de data a la GCS
//Para que ande hay que solicitarAntena() y devolverAntena()
void datasend(const uint8_t* data){
  mavlink_message_t msg;
  uint8_t buf[MAVLINK_MAX_PACKET_LEN];
  mavlink_msg_file_transfer_protocol_pack(system_id, component_id, &msg,0,0,0, data);
  uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
  serant.write(buf, len);
}




void disarm(){
  mavlink_message_t msg;
  uint8_t buf[MAVLINK_MAX_PACKET_LEN];
  mavlink_msg_command_long_pack(system_id, component_id, &msg,target_system,target_component,MAV_CMD_COMPONENT_ARM_DISARM,confirmation,DISARM,0,0,0,0,0,0);
  uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
  sermav.write(buf, len);
}

void missionrequest(){
  mavlink_message_t msg;
  uint8_t buf[MAVLINK_MAX_PACKET_LEN];
  mavlink_msg_mission_request_list_pack(system_id, component_id, &msg, target_system, target_component);
  uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
  sermav.write(buf, len);
}

void waypointrequest(uint16_t seq){
  mavlink_message_t msg;
  uint8_t buf[MAVLINK_MAX_PACKET_LEN];
  mavlink_msg_mission_request_pack(system_id, component_id, &msg, target_system, target_component,seq);
  uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
  sermav.write(buf, len);
}

void waypointrequestint(uint16_t seq){
  mavlink_message_t msg;
  uint8_t buf[MAVLINK_MAX_PACKET_LEN];
  mavlink_msg_mission_request_int_pack(system_id, component_id, &msg, target_system, target_component,seq);
  uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
  sermav.write(buf, len);
}

void sendwaypointcount(uint16_t count){
  mavlink_message_t msg;
  uint8_t buf[MAVLINK_MAX_PACKET_LEN];
  mavlink_msg_mission_count_pack(system_id, component_id, &msg, target_system, target_component,count);
  uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
  sermav.write(buf, len);
}

void sendwaypoint(uint16_t seq, uint8_t frame, uint16_t command,uint8_t current, uint8_t autocontinue, float param1, float param2, float param3, float param4, float x, float y, float z){
  mavlink_message_t msg;
  uint8_t buf[MAVLINK_MAX_PACKET_LEN];
  mavlink_msg_mission_item_pack(system_id, component_id, &msg, target_system, target_component,seq,frame,command,current,autocontinue,param1,param2,param3,param4,x,y,z);
  uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
  sermav.write(buf, len);
}

void sendwaypointint(uint16_t seq, uint8_t frame, uint16_t command,uint8_t current, uint8_t autocontinue, float param1, float param2, float param3, float param4, float x, float y, float z){
  mavlink_message_t msg;
  uint8_t buf[MAVLINK_MAX_PACKET_LEN];
  mavlink_msg_mission_item_int_pack(system_id, component_id, &msg, target_system, target_component,seq,frame,command,current,autocontinue,param1,param2,param3,param4,x,y,z);
  uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
  sermav.write(buf, len);
}

void sendwaypointcurrent(uint16_t seq){
  mavlink_message_t msg;
  uint8_t buf[MAVLINK_MAX_PACKET_LEN];
  mavlink_msg_mission_set_current_pack(system_id, component_id, &msg, target_system, target_component,seq);
  uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
  sermav.write(buf, len);
}

void missionack(){
  mavlink_message_t msg;
  uint8_t type = MAV_MISSION_ACCEPTED; // 0 significa OK
  uint8_t buf[MAVLINK_MAX_PACKET_LEN];
  mavlink_msg_mission_ack_pack(system_id, component_id, &msg, target_system, target_component, type);
  uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
  sermav.write(buf, len);
  #ifdef debugging
  sermon.println(F("Mision recibida OK"));
  #endif
}

void land(){
  mavlink_message_t msg;
  uint8_t buf[MAVLINK_MAX_PACKET_LEN];
  mavlink_msg_command_long_pack(system_id, component_id, &msg,target_system, target_component, MAV_CMD_NAV_LAND, confirmation, 0, 0, param3, param4, actual_wp.lat, actual_wp.lon, 0);
  uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
  sermav.write(buf, len);
}




void waitfor(uint8_t MSG_ID, uint32_t timeout){
  uint32_t timestamp = millis();
  received = 0;
  mavlink_message_t msg; 
  mavlink_status_t stat;
  while(received == 0 && millis() < timestamp+timeout){
    while(sermav.available()>0) { 
      uint8_t c = sermav.read();
      if(mavlink_parse_char(MAVLINK_COMM_0, c, &msg, &stat)) {
        if(msg.msgid == MSG_ID){
          manageMSG(msg);
        }
      }
    }
  }
  if(millis() >= timestamp+timeout){
    #ifdef debugging
    sermon.print(F("Salio por timeout esperando mensaje #"));
    sermon.println(MSG_ID);
    #endif
  }
}



void manageMSG(mavlink_message_t msg){
  switch(msg.msgid) {
    case MAVLINK_MSG_ID_HEARTBEAT: {
      received = 1;
      uint8_t PX4_ARMED_AUX = mavlink_msg_heartbeat_get_base_mode(&msg);
      PX4_STATUS = mavlink_msg_heartbeat_get_system_status(&msg);
      if((PX4_ARMED_AUX)&(MAV_MODE_FLAG_DECODE_POSITION_SAFETY)){
       ARMED_STATUS = ARMED_STATUS_ARMED;  
      }else{
        ARMED_STATUS = ARMED_STATUS_DISARMED;  
      }

      // Aca estaria en modo mision
      if((PX4_ARMED_AUX)&(MAV_MODE_FLAG_GUIDED_ENABLED)){
       INPUT_STATUS= INPUT_STATUS_MISSION;  
      }else{
        if((PX4_ARMED_AUX)&(MAV_MODE_FLAG_MANUAL_INPUT_ENABLED)){
         INPUT_STATUS= INPUT_STATUS_MANUAL;  
        }else{
          INPUT_STATUS = INPUT_STATUS_UNKNOWN;
        }
      }
      #ifdef debugging
      sermon.println(F("Heartbeat recibido:"));
      sermon.print(F("Actividad: "));
      #endif
      if(PX4_STATUS==MAV_STATE_ACTIVE){
        PIXHAWK_STATE = PIXHAWK_MISION;
        #ifdef  debugging
        sermon.print(F("ACTIVO; "));
        #endif
      }else if(PX4_STATUS==MAV_STATE_STANDBY){
        PIXHAWK_STATE = PIXHAWK_STANDBY;  
        #ifdef  debugging
        sermon.print(F("STANDBY; "));
        #endif            
      }

      #ifdef debugging
      if(ARMED_STATUS == ARMED_STATUS_DISARMED){
        sermon.print(F("DESARMADO; "));          
      }else if(ARMED_STATUS == ARMED_STATUS_ARMED){
        sermon.print(F("ARMADO; "));        
      }else if(ARMED_STATUS == ARMED_STATUS_UNKNOWN){
        sermon.print(F("DESCONOCIDO; "));        
      }
      
      if (INPUT_STATUS == INPUT_STATUS_MISSION){
        sermon.print(F("AUTONOMO; "));
      }else if (INPUT_STATUS == INPUT_STATUS_MANUAL){
        sermon.print(F("MANUAL; "));
      }else if (INPUT_STATUS == INPUT_STATUS_UNKNOWN){
        sermon.print(F("DESCONOCIDO; "));
      }

      if (Pixhawk_landed_state == MAV_LANDED_STATE_UNDEFINED){
        sermon.print(F("INDEFINIDO; "));
      }else if (Pixhawk_landed_state == MAV_LANDED_STATE_ON_GROUND){
        sermon.print(F("EN TIERRA; "));
      }else if (Pixhawk_landed_state == MAV_LANDED_STATE_IN_AIR){
        sermon.print(F("EN AIRE; "));
      }
      sermon.println();
      #endif

      break;
    }
    case MAVLINK_MSG_ID_HOME_POSITION :{ //ID del mensaje 33
      received = 1;
      home_wp.lat = mavlink_msg_home_position_get_latitude(&msg)/1E7; // Latitud, expresada en grados * 1E7
      home_wp.lon = mavlink_msg_home_position_get_longitude(&msg)/1E7; // Longitud, expresada en grados * 1E7
      home_wp.alt = mavlink_msg_home_position_get_altitude(&msg)/1E3; // Altitud en metros, expresada en milimetros, AMSL
      
      #ifdef debugging
      sermon.print(F("Posicion del HOME"));            
      sermon.print(F("  Latitud: "));
      sermon.print(home_wp.lat);
      sermon.print(F("  Longitud: "));
      sermon.print(home_wp.lon);
      sermon.print(F("  Altitud: "));
      sermon.println(home_wp.alt);
      #endif
      break;
    }
    case MAVLINK_MSG_ID_BATTERY_STATUS: {
      received = 1;
      bat_temp = mavlink_msg_battery_status_get_temperature(&msg)/1E3;
      mavlink_msg_battery_status_get_voltages(&msg, bat_volt);
      bat_curr = 10*mavlink_msg_battery_status_get_current_battery(&msg);
      int32_t bat_totalcurr = mavlink_msg_battery_status_get_current_consumed(&msg);
      //bat_ener = 100*mavlink_msg_battery_status_get_energy_consumed(&msg);
      bat = mavlink_msg_battery_status_get_battery_remaining(&msg);
      #ifdef debugging
      sermon.print(F("Bateria(%):"));
      sermon.println(bat);
      #endif
      break;
    }
    case MAVLINK_MSG_ID_STATUSTEXT:{
      received = 1;
      severity = mavlink_msg_statustext_get_severity(&msg);
      mavlink_msg_statustext_get_text(&msg, statusText);
      #ifdef debugging
      sermon.print(F("STATUSTEXT severity:"));
      sermon.println(severity);
      sermon.print(F("mensaje: "));
      sermon.println(statusText);
      #endif
      break;            
    }
    case MAVLINK_MSG_ID_SYSTEM_TIME:{
      received = 1;
      time_unix_usec = mavlink_msg_system_time_get_time_unix_usec(&msg);
      time_boot_ms = mavlink_msg_system_time_get_time_boot_ms(&msg);
      boot_time_us = time_unix_usec - (time_boot_ms*1000);
      time_ref = (time_unix_usec != 0);
      if(time_ref){
        shutup(MAVLINK_MSG_ID_SYSTEM_TIME);   //Dejar de pedirlo
        time_counter = millis();
        #ifdef debugging
        sermon.println(F("Tiempo unix recibido(us):"));
        //sermon.println(time_unix_usec);
        #endif
      }
      break;            
    }
    case MAVLINK_MSG_ID_FLIGHT_INFORMATION :{
      received = 1;
      //time_boot_ms = mavlink_msg_flight_information_get_time_boot_ms(&msg);
      if(time_ref){
        //actual_time_us = time_unix_usec + ((millis()-time_counter)*1000);//actual_time_us = boot_time_us + (time_boot_ms*1000);
        //sermon.println(F("Tiempo actual(us):"));
        //sermon.println(actual_time_us);
        shutup(MAVLINK_MSG_ID_FLIGHT_INFORMATION);
        #ifdef SD_present
        SD_timePX4_save;
        #endif
      }
      arming_time_utc = mavlink_msg_flight_information_get_arming_time_utc(&msg);
      takeoff_time_utc = mavlink_msg_flight_information_get_takeoff_time_utc(&msg);
      flight_uuid = mavlink_msg_flight_information_get_flight_uuid(&msg);
      break;            
    }
    case MAVLINK_MSG_ID_GLOBAL_POSITION_INT :{ //ID del mensaje 33
      received = 1;
      /*time_boot_ms = mavlink_msg_global_position_int_get_time_boot_ms(&msg);
      if(time_ref){
        actual_time_us = boot_time_us + (time_boot_ms*1000);
      }*/
      int32_t pos_lat=mavlink_msg_global_position_int_get_lat(&msg); // Latitud, expresada en grados * 1E7
      int32_t pos_lon=mavlink_msg_global_position_int_get_lon(&msg); // Longitud, expresada en grados * 1E7
      int32_t pos_alt=mavlink_msg_global_position_int_get_alt(&msg); // Altitud en metros, expresada en milimetros, AMSL
      int32_t pos_relative_alt=mavlink_msg_global_position_int_get_relative_alt(&msg); // Altitud sobre el suelo en metros, expresada como * 1000 (milimetros)
//      uint16_t pos_vx=mavlink_msg_global_position_int_get_vx(&msg); // Velocidad en tierra X (Latitud, norte positivo), expresada en m/s * 100
//      uint16_t pos_vy=mavlink_msg_global_position_int_get_vy(&msg); // Velocidad en tierra Y (Longitud, este positivo), expresada en m/s * 100
//      uint16_t pos_vz=mavlink_msg_global_position_int_get_vz(&msg); // Velocidad en tierra Z (Altitud, abajo positivo), expresada en m/s * 100
      uint16_t pos_hdg=mavlink_msg_global_position_int_get_hdg(&msg)/1E2; // Direccion del vehiculo (angulo yaw) en grados * 100, 0.0..359.99 degrees. Si no se sabe se setea en UINT16_MAX
      actual_wp.lat = pos_lat / 1E7;
      actual_wp.lon = pos_lon / 1E7;
      actual_wp.alt = pos_relative_alt / 1E3; //metros
      actual_yaw = pos_hdg;
      #ifdef debugging
      sermon.print(F("Wp al que va:"));
      sermon.println(seq);
      #endif
      #ifdef debugging
      sermon.print(F("GPS recibido"));            
      sermon.print(F("  Latitud: "));
      sermon.print(actual_wp.lat,7);
      sermon.print(F("  Longitud: "));
      sermon.print(actual_wp.lon,7);
      sermon.print(F("  Altitud: "));
      sermon.println(actual_wp.alt);
      #endif
      break;
    }
	case MAVLINK_MSG_ID_SCALED_PRESSURE : {
      received = 1;
  	  //time_boot_ms = mavlink_msg_scaled_pressure_get_time_boot_ms(&msg);
  	  pressure_abs = mavlink_msg_scaled_pressure_get_press_abs(&msg);
  	  pressure_diff = mavlink_msg_scaled_pressure_get_press_diff(&msg);
  	  temperature_px4 = mavlink_msg_scaled_pressure_get_temperature(&msg);
      #ifdef debugging
      sermon.print(F("Presion absoluta:"));
  	  sermon.println(pressure_abs);
  	  sermon.print(F("Presion diferencial:"));
  	  sermon.println(pressure_diff);
  	  sermon.print(F("Temperatura PX4:"));
  	  sermon.println(temperature_px4);
      #endif
      break;            
    }
    case MAVLINK_MSG_ID_MISSION_ITEM_REACHED : {
      received = 1;
      home_reached = true;
      #ifdef debugging
      sermon.print(F("Wp alcanzado:"));
      sermon.println(mavlink_msg_mission_item_reached_get_seq(&msg));
      #endif
      break;            
    }
    case MAVLINK_MSG_ID_ATTITUDE :{
      received = 1;
      /*time_boot_ms = mavlink_msg_attitude_get_time_boot_ms(&msg);
      if(time_ref){
        actual_time_us = boot_time_us + (time_boot_ms*1000);
      }*/
      att_yaw = ((mavlink_msg_attitude_get_yaw(&msg)/PI)+1)*180;
      att_roll = ((mavlink_msg_attitude_get_roll(&msg)/PI)+1)*180;
      att_pitch = ((mavlink_msg_attitude_get_pitch(&msg)/PI)+1)*180;
      att_yaw_speed = ((mavlink_msg_attitude_get_yawspeed(&msg)/PI)+1)*180;
      att_roll_speed = ((mavlink_msg_attitude_get_rollspeed(&msg)/PI)+1)*180;
      att_pitch_speed = ((mavlink_msg_attitude_get_pitchspeed(&msg)/PI)+1)*180;
      break;
    }
    case MAVLINK_MSG_ID_MISSION_ITEM:{
      received = 1;
      seq = mavlink_msg_mission_item_get_seq(&msg);
      mission_wps[seq].lat = (mavlink_msg_mission_item_get_x(&msg));// latitud
      mission_wps[seq].lon = (mavlink_msg_mission_item_get_y(&msg));// longitud
      mission_wps[seq].alt = (mavlink_msg_mission_item_get_z(&msg));// altitud
      #ifdef debugging
      sermon.print(F("WP recibido:"));
      sermon.println(seq);
      sermon.print(F("  Latitud: "));
      sermon.print(mission_wps[seq].lat / 1E7);
      sermon.print(F("  Longitud: "));
      sermon.print(mission_wps[seq].lon / 1E7);
      sermon.print(F("  Altitud: "));
      sermon.println(mission_wps[seq].alt);
      #endif
      break;            
    }
    case MAVLINK_MSG_ID_MISSION_ITEM_INT:{
      #ifdef waypoint_int
      received = 1;
      seq = mavlink_msg_mission_item_get_seq(&msg);
      mission_wpsint[seq].latint = (mavlink_msg_mission_item_int_get_x(&msg));// latitud
      mission_wpsint[seq].lonint = (mavlink_msg_mission_item_int_get_y(&msg));// longitud
      mission_wpsint[seq].altint = (mavlink_msg_mission_item_int_get_z(&msg));// altitud
      #ifdef debugging
      sermon.print(F("WP recibido:"));
      sermon.println(seq);
      sermon.print(F("  Latitud: "));
      sermon.print(mission_wpsint[seq].latint / 1E7);
      sermon.print(F("  Longitud: "));
      sermon.print(mission_wpsint[seq].lonint / 1E7);
      sermon.print(F("  Altitud: "));
      sermon.println(mission_wpsint[seq].altint);
      #endif
      #endif
      break;            
    }
    case MAVLINK_MSG_ID_MISSION_CURRENT: {
      received = 1;
      seq = mavlink_msg_mission_current_get_seq(&msg);
      /*#ifdef debugging
      sermon.print(F("Wp al que va:"));
      sermon.println(seq);
      #endif*/
      break;
    }
    case MAVLINK_MSG_ID_MISSION_COUNT: {
      received = 1;
      count = mavlink_msg_mission_count_get_count(&msg);
      #ifdef debugging
      sermon.print(F("Cantidad de WP en la mision:"));
      sermon.println(count);
      #endif
      break;
    }
    case MAVLINK_MSG_ID_MISSION_ACK: {
      received = 1;
      #ifdef debugging
      sermon.println(F("Mission ACK recibido"));
      #endif
      break;            
    }
    case MAVLINK_MSG_ID_MISSION_REQUEST: {
      received = 1;
      seq = mavlink_msg_mission_request_get_seq(&msg);
      #ifdef debugging
      sermon.print(F("Mission Request waypoint:"));
      sermon.println(seq);
      #endif
      break;            
    }
    case MAVLINK_MSG_ID_MISSION_REQUEST_INT: {
      #ifdef waypoint_int
      received = 1;
      seq = mavlink_msg_mission_request_int_get_seq(&msg);
      #ifdef debugging
      sermon.print(F("Mission Request INT waypoint:"));
      sermon.println(seq);
      #endif
      #endif
      break;            
    }
    case MAVLINK_MSG_ID_PING: {
      received = 1;
      #ifdef debugging
      sermon.println(F("Ping"));
      #endif
      break;            
    }
    case MAVLINK_MSG_ID_COMMAND_ACK :{ //ID del mensaje 77
        #ifdef  debugging
        sermon.print(F("Command Acknowledge recibido, "));
        #endif
        uint8_t result_command_ack= mavlink_msg_command_ack_get_result(&msg);
        switch(result_command_ack) {
          case MAV_RESULT_ACCEPTED:{
            #ifdef  debugging
            sermon.println(F("MAV_RESULT_ACCEPTED")); //Command ACCEPTED and EXECUTED
            #endif
            break;
          }
          case MAV_RESULT_TEMPORARILY_REJECTED:{
            #ifdef  debugging
            sermon.println(F("MAV_RESULT_TEMPORARILY_REJECTED")); //Command TEMPORARY REJECTED/DENIED
            #endif
            break;
          }
          case MAV_RESULT_DENIED:{
            #ifdef  debugging
            sermon.println(F("MAV_RESULT_DENIED")); //Command PERMANENTLY DENIED
            #endif
            break;
          }
          case MAV_RESULT_UNSUPPORTED:{
            #ifdef  debugging
            sermon.println(F("MAV_RESULT_UNSUPPORTED")); //Command UNKNOWN/UNSUPPORTED
            #endif
            break;
          }
          case MAV_RESULT_FAILED:{
            #ifdef  debugging
            sermon.println(F("MAV_RESULT_FAILED")); //Command executed, but failed
            #endif
            break;
          }
          default:
          break;
        }
        break;
    }
    case MAVLINK_MSG_ID_SERIAL_CONTROL:{
      /*received = 1;
      //serial_control_device = mavlink_msg_serial_control_get_device(&msg);
      //serial_control_flags = mavlink_msg_serial_control_get_flags(&msg);
      //serial_control_timeout = mavlink_msg_serial_control_get_timeout(&msg);
      //serial_control_baudrate = mavlink_msg_serial_control_get_baudrate(&msg);
      serial_control_count = mavlink_msg_serial_control_get_count(&msg);
      mavlink_msg_serial_control_get_data(&msg, serial_control_data);
      if((serial_control_data[0] == 128)){
          mission_mode = serial_control_data[1];
      }else{
        #ifdef debugging
          sermon.println("Serial Control Data Error");
        #endif
      }*/
      break;            
    }
    case MAVLINK_MSG_ID_FILE_TRANSFER_PROTOCOL: {  //ID del mensaje #110
      uint8_t datareceivedGCS[DATATOGCSSIZE];
      mavlink_msg_file_transfer_protocol_get_payload(&msg, datareceivedGCS);
      #ifdef debugging
      sermon.println(F("File transfer protocol"));
      #endif
      processDataFromGCS(datareceivedGCS);
    break;
    }
    default:
    #ifdef debugging
    sermon.print(F("Mensaje diferente, ID ="));
    sermon.println(msg.msgid);
    #endif
    break;
  
  case MAVLINK_MSG_ID_EXTENDED_SYS_STATE: { // #245
          
    Pixhawk_landed_state = mavlink_msg_extended_sys_state_get_landed_state(&msg);

    if(apurarEstadoExtendido){
      requestMSG(MAVLINK_MSG_ID_EXTENDED_SYS_STATE,1E5);
      apurarEstadoExtendido = false;
    }
    /*#ifdef debugging
    sermon.print(F("Estado extendido del dron: "));
    switch(Pixhawk_landed_state) {   
      case MAV_LANDED_STATE_UNDEFINED:{
        sermon.println(F("Indefinido"));
        break;  
      }
      case MAV_LANDED_STATE_ON_GROUND:{
        sermon.println(F("En tierra"));
        break;  
      }
      case MAV_LANDED_STATE_IN_AIR:{
        sermon.println(F("En aire"));
        break;  
      }  
    }
    #endif*/
    break;            
  }
  }
}


////////////////////////////////AUX Functions////////////////////////////////



void datasend_sensor(){
  uint8_t datasensors[DATATOGCSSIZE];
  int iDataSensor=0;
  datasensors[iDataSensor] = SEND2GCS_SENSOR; // 0
  iDataSensor++;
  datasensors[iDataSensor] = (r >> 8 ) & 255;
  iDataSensor++;
  datasensors[iDataSensor] = r & 255;
  iDataSensor++;
  datasensors[iDataSensor] = (s >> 8 ) & 255;
  iDataSensor++;
  datasensors[iDataSensor] = s & 255;
  iDataSensor++;
  datasensors[iDataSensor] = (sensor_act[0] & 0x01)+(sensor_act[1] & 0x02)+(sensor_act[2] & 0x04)+(sensor_act[3] & 0x08);
  iDataSensor++;
  datasensors[iDataSensor] = (sensor_dist[0] >> 8 ) & 255;
  iDataSensor++;
  datasensors[iDataSensor] = sensor_dist[0] & 255;
  iDataSensor++;
  datasensors[iDataSensor] = (sensor_dist[1] >> 8 ) & 255;
  iDataSensor++;
  datasensors[iDataSensor] = sensor_dist[1] & 255;
  iDataSensor++;
  datasensors[iDataSensor] = (sensor_dist[2] >> 8 ) & 255;
  iDataSensor++;
  datasensors[iDataSensor] = sensor_dist[2] & 255;
  iDataSensor++;
  datasensors[iDataSensor] = (sensor_dist[3] >> 8 ) & 255;
  iDataSensor++;
  datasensors[iDataSensor] = sensor_dist[3] & 255;
  iDataSensor++;
  uint64_t time_unix = (actual_time_us/1E6) - 10800; //UTM-3 
  datasensors[iDataSensor] = (hour(time_unix))& 255;
  iDataSensor++;
  datasensors[iDataSensor] = (minute(time_unix))& 255;
  iDataSensor++;
  datasensors[iDataSensor] = (second(time_unix))& 255;
  iDataSensor++;
  datasensors[iDataSensor] = (isAM(time_unix))& 255;
  iDataSensor++;
  datasensors[iDataSensor] = (day(time_unix))& 255;
  iDataSensor++;
  datasensors[iDataSensor] = (month(time_unix))& 255;
  iDataSensor++;
  datasensors[iDataSensor] = (year(time_unix))& 255;
  iDataSensor++;

  solicitarAntena();
  datasend(datasensors);
  devolverAntena();
}

void datasend_image(int part_picture){
  
  uint8_t dataimage[DATATOGCSSIZE];
  int iDataImage=0;
  const int offsetcol = (image_x *2); //122, comienzo en dataimage de la columna par
    
  dataimage[iDataImage] = SEND2GCS_IMAGE; // 1
  iDataImage++;
    
  for(int columna = (20*(part_picture)); columna < (20*(part_picture+1)); columna+=2){ // se recorren todas las columnas (de 0 a 79) y se envían de a 2
    dataimage[iDataImage]  = columna; // Segundo byte indica la primera de las dos columnas que incluye el mensaje
    iDataImage++;

    for(int fila = 0; fila < image_x; fila++){ // recorro toda la columna (de la fila 0 a la 59) y copia en dataimage
      dataimage[iDataImage]  = ((Celsius_thermal_image[fila][columna])>> 8 ) & 0xFF; // image tiene elementos de 16 bits y data de 8, así que hay que hacer un shift para pasarlos
      dataimage[iDataImage + offsetcol]  = ((Celsius_thermal_image[fila][columna+1])>> 8 ) & 0xFF; 
      iDataImage++;
      dataimage[iDataImage]  = (Celsius_thermal_image[fila][columna]) & 0xFF;
      dataimage[iDataImage + offsetcol]  = (Celsius_thermal_image[fila][columna+1]) & 0xFF;
      iDataImage++;
    }
    
    datasend(dataimage); // acá se envían las dos columnas
    iDataImage = 1; // se resetea el indice de la imagen para el próximo mensaje
    //delay(80);
  }
}

void datasend_error(int type_err){    //1:Resync; 2:SD; 3:Mavlink;
  uint8_t dataerror[DATATOGCSSIZE];
  dataerror[0] = SEND2GCS_ERROR;  //2
  dataerror[1] = type_err;
  solicitarAntena();
  datasend(dataerror);
  devolverAntena();
}


void datasend_mailto(){  
  uint8_t dataMailTo[DATATOGCSSIZE];
  dataMailTo[0] = SEND2GCS_MAIL;  //2

  for(int kChar = 0; kChar < MAILLENGHT; kChar++){
    dataMailTo[kChar + 1] = to_address[kChar];
  }
 
  solicitarAntena();
  datasend(dataMailTo);
  devolverAntena();
}


void datasend_log(int part_log){
  uint8_t datalog[DATATOGCSSIZE];
  int iDataLog=0;
    
  datalog[iDataLog] = SEND2GCS_LOG; // 3
  iDataLog++;
  datalog[iDataLog] = part_log; //
  iDataLog++;

  myFile = SD.open(log_file);
  if (myFile){
    #ifdef debugging
    sermon.println(F("Leyendo SD..."));
    #endif
    datalog[iDataLog] = size_log;
    iDataLog++;
    myFile.seek(part_log*(DATATOGCSSIZE-iDataLog));
    while(iDataLog < DATATOGCSSIZE){
      datalog[iDataLog] = myFile.read();
      iDataLog++;
    }
    myFile.close();
  }else {
    error_SDfile(log_file);
  }
  datasend(datalog);
  iDataLog = 1;
  delay(80);
}

void datasend_get(){
  uint8_t dataget[DATATOGCSSIZE];
  dataget[0] = SEND2GCS_GET;    //4
  solicitarAntena();
  datasend(dataget);
  devolverAntena();
}


void solicitarAntena(){
  antenasolicitada = true;
  digitalWrite(PXEnablePin,HIGH); // Desconectamos a Pixhawk
  digitalWrite(ArdEnablePin,LOW);// Conectamos a Arduino
}

void devolverAntena(){
  delay(50); // Delay para permitir al buffer de salida escribir todo el mensaje
  digitalWrite(ArdEnablePin,HIGH);// Desconectamos a Arduino
  digitalWrite(PXEnablePin,LOW); // Conectamos a Pixhawk
  antenasolicitada = false;
}

void processDataFromGCS(uint8_t * data){
  flags_received = true;
  dsiack = data[0];
  sendimage = data[1];
  mission_mode = data[2];
  logsendenable = data[3];
}




