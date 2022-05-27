//LANDING_TARGET info para aterrizar
//MISSION_ITEM_REACHED wp alcanzado

void turn_off_motors(){
  
}

void wait_turn(int giro){
  bool timewaitaux = false;
  uint16_t aux_yaw;
  while(timewaitaux){
    aux_yaw = prior_yaw + giro;
    if(360 < aux_yaw){
      aux_yaw = prior_yaw - 360;
    }else if(aux_yaw < 360){
      aux_yaw = 360 - prior_yaw;
    }
    if( (aux_yaw-10)< actual_yaw && actual_yaw <(aux_yaw+10)){
      timewaitaux = true;
    }
  }
}

void up_date(){
  mavlink_message_t msg; 
  mavlink_status_t stat;
  while(sermav.available()>0) { 
    uint8_t c = sermav.read();
    if(mavlink_parse_char(MAVLINK_COMM_0, c, &msg, &stat)) {
      manageMSG(msg);
    }
  }
  #ifdef SD_present
    //sd_log_save();
  #endif
  sermon.println("Actualizado");
}

void request_all_mission(){
  requestMSG( t, MAVLINK_MSG_ID_HOME_POSITION);//
  waitfor(MAVLINK_MSG_ID_HOME_POSITION, tmax);//
  missionrequest();
  waitfor(MAVLINK_MSG_ID_MISSION_COUNT, tmax);
  for (int i = 0; i < count; i++){
    waypointrequest(i);
    waitfor(MAVLINK_MSG_ID_MISSION_ITEM, tmax);
  }
  missionack();
}

void recover_mission() {
    clearall();
    sendwaypointcount(count);
    for (int i = 0; i < count; i++){
      waitfor(MAVLINK_MSG_ID_MISSION_REQUEST, tmax);
      seq = i;
      current = count; // Punto actual
      autocontinue  = 1; // True 
      param1=0; // Tiempo de hold  
      param2=0; // Radio de aceptacion en metros
      param3=0; // 0 para pasar por el WP
      param4=0; // YAW deseado
      command  =  MAV_CMD_NAV_WAYPOINT; // CMD_ID = 16
      sendwaypoint(seq,frame,command,current,autocontinue,param1,param2,param3,param4,mission_wps[i].lat,mission_wps[i].lon,mission_wps[i].alt);
    }
    waitfor(MAVLINK_MSG_ID_MISSION_ACK, tmax);
    sendwaypointcurrent(auxseq);
    waitfor(MAVLINK_MSG_ID_MISSION_CURRENT, tmax);
}

void send_wp() {
    auxseq = seq;
    clearall();
    sendwaypointcount(1);
    waitfor(MAVLINK_MSG_ID_MISSION_REQUEST, tmax);
    seq = 0;
    current=0; // Punto actual
    autocontinue  = 1; // True 
    param1=0; // Tiempo de hold  
    param2=0; // Radio de aceptacion en metros
    param3=0; // 0 para pasar por el WP
    param4=0; // YAW deseado
    command  =  MAV_CMD_NAV_WAYPOINT; // CMD_ID = 16
    sendwaypoint(seq,frame,command,current,autocontinue,param1,param2,param3,param4,new_wp.lat,new_wp.lon,new_wp.alt);
    waitfor(MAVLINK_MSG_ID_MISSION_ACK, tmax);
}





void heartBeatCallback() {
  sendHeartBeat();
}

void sendHeartBeat() { 
  mavlink_message_t msg; // crea un mensaje
  uint8_t buf[MAVLINK_MAX_PACKET_LEN]; // crea un buffer para guardarlo
  mavlink_msg_heartbeat_pack(system_id, component_id, &msg, system_type, autopilot_type, base_mode, custom_mode, system_status); // Arma el mensaje
  uint16_t len = mavlink_msg_to_send_buffer(buf, &msg); //guarda el mensaje en el buffer y devuelve el largo del mensaje (payload+no payload) 
  sermav.write(buf, len); // manda el mensaje 
  sermon.println("Heartbeat enviado"); // Imprime que lo mandó
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
  sermon.println("Apaga msg");
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
void datasend(uint8_t count_data, const uint8_t* data){
  mavlink_message_t msg;
  uint8_t buf[MAVLINK_MAX_PACKET_LEN];
  mavlink_msg_serial_control_pack(system_id, component_id, &msg, SERIAL_CONTROL_DEV_TELEM1, SERIAL_CONTROL_FLAG_REPLY, 0, 0, count_data, data);
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
  sermon.println("Mision recibida OK");
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
    sermon.print("Salio por timeout esperando mensaje #");
    sermon.println(MSG_ID);
  }
}



void manageMSG(mavlink_message_t msg){
  switch(msg.msgid) {
    case MAVLINK_MSG_ID_MISSION_CURRENT: {
      received = 1;
      seq = mavlink_msg_mission_current_get_seq(&msg);
      sermon.print("Wp al que va:");
      sermon.println(seq);
      break;
    }
    case MAVLINK_MSG_ID_MISSION_COUNT: {
      received = 1;
      count = mavlink_msg_mission_count_get_count(&msg);
      sermon.print("Cantidad de WP en la mision:");
      sermon.println(count);
      break;
    }
    case MAVLINK_MSG_ID_BATTERY_STATUS: {
      received = 1;
      bat_temp = mavlink_msg_battery_status_get_temperature(&msg);
      mavlink_msg_battery_status_get_voltages(&msg, bat_volt);
      bat_curr = 10*mavlink_msg_battery_status_get_current_battery(&msg);
      int32_t bat_totalcurr = mavlink_msg_battery_status_get_current_consumed(&msg);
      bat_ener = 100*mavlink_msg_battery_status_get_energy_consumed(&msg);
      bat = mavlink_msg_battery_status_get_battery_remaining(&msg);
      sermon.print("Bateria(%):");
      sermon.println(bat);
      break;
    }
    case MAVLINK_MSG_ID_MISSION_REQUEST: {
      received = 1;
      uint16_t sequ = mavlink_msg_mission_request_get_seq(&msg);
      sermon.print("Numero de WP en la mision:");
      sermon.println(sequ);
      break;
    }
    case MAVLINK_MSG_ID_MISSION_ACK: {
      received = 1;
      sermon.print("Mission ACK recibido");
      break;            
    }
    case MAVLINK_MSG_ID_STATUSTEXT:{
      received = 0;
      severity = mavlink_msg_statustext_get_severity(&msg);
      mavlink_msg_statustext_get_text(&msg, statusText);
      sermon.print("STATUSTEXT severity:");
      sermon.println(severity);
      sermon.print("mensaje: ");
      sermon.println(statusText);
      break;            
    }
    case MAVLINK_MSG_ID_SYSTEM_TIME:{
      received = 0;
      time_unix_usec = mavlink_msg_system_time_get_time_unix_usec(&msg);
      time_boot_ms = mavlink_msg_system_time_get_time_boot_ms(&msg);
      time_ref = (time_unix_usec != 0);
      if(time_ref){
        shutup(2);
      }
      boot_hour_us = time_unix_usec - (time_boot_ms*1000);
      sermon.print("Time UNIX usec:");
      //sermon.println(time_unix_usec);
      break;            
    }
    case MAVLINK_MSG_ID_FLIGHT_INFORMATION :{
      received = 0;
      time_boot_ms = mavlink_msg_flight_information_get_time_boot_ms(&msg);
      if(time_ref){
        actual_time_us = boot_hour_us + (time_boot_ms*1000);
      }
      arming_time_utc = mavlink_msg_flight_information_get_arming_time_utc(&msg);
      takeoff_time_utc = mavlink_msg_flight_information_get_takeoff_time_utc(&msg);
      break;            
    }
    case MAVLINK_MSG_ID_GLOBAL_POSITION_INT :{ //ID del mensaje 33
      received = 1;
      time_boot_ms = mavlink_msg_global_position_int_get_time_boot_ms(&msg);
      if(time_ref){
        actual_time_us = boot_hour_us + (time_boot_ms*1000);
      }
      uint32_t pos_lat=mavlink_msg_global_position_int_get_lat(&msg); // Latitud, expresada en grados * 1E7
      uint32_t pos_lon=mavlink_msg_global_position_int_get_lon(&msg); // Longitud, expresada en grados * 1E7
      uint32_t pos_alt=mavlink_msg_global_position_int_get_alt(&msg); // Altitud en metros, expresada en milimetros, AMSL
      uint32_t pos_relative_alt=mavlink_msg_global_position_int_get_relative_alt(&msg); // Altitud sobre el suelo en metros, expresada como * 1000 (milimetros)
//      uint16_t pos_vx=mavlink_msg_global_position_int_get_vx(&msg); // Velocidad en tierra X (Latitud, norte positivo), expresada en m/s * 100
//      uint16_t pos_vy=mavlink_msg_global_position_int_get_vy(&msg); // Velocidad en tierra Y (Longitud, este positivo), expresada en m/s * 100
//      uint16_t pos_vz=mavlink_msg_global_position_int_get_vz(&msg); // Velocidad en tierra Z (Altitud, abajo positivo), expresada en m/s * 100
      uint16_t pos_hdg; // Direccion del vehiculo (angulo yaw) en grados * 100, 0.0..359.99 degrees. Si no se sabe se setea en UINT16_MAX
      actual_wp.lat = - pos_lat / 1E7;
      actual_wp.lon = - pos_lon / 1E7;
      actual_wp.alt = - pos_relative_alt / 1000; //metros
      actual_yaw = pos_hdg;
      sermon.print("GPS recibido");            
      sermon.print("Latitud: ");
      sermon.print(actual_wp.lat);
      sermon.print("Longitud: ");
      sermon.print(actual_wp.lon);
      sermon.print("Altitud: ");
      sermon.print(actual_wp.alt);
      break;
    }
    case MAVLINK_MSG_ID_HOME_POSITION :{ //ID del mensaje 33
      received = 1;
      home_wp.lat = mavlink_msg_home_position_get_latitude(&msg)/1E7; // Latitud, expresada en grados * 1E7
      home_wp.lon = mavlink_msg_home_position_get_longitude(&msg)/1E7; // Longitud, expresada en grados * 1E7
      home_wp.alt = mavlink_msg_home_position_get_altitude(&msg); // Altitud en metros, expresada en milimetros, AMSL
      sermon.print("Posicion del HOME");            
      sermon.print("Latitud: ");
      sermon.print(home_wp.lat);
      sermon.print("Longitud: ");
      sermon.print(home_wp.lon);
      sermon.print("Altitud: ");
      sermon.print(home_wp.alt);
      break;
    }
    case MAVLINK_MSG_ID_ATTITUDE :{
      received = 1;
      time_boot_ms = mavlink_msg_attitude_get_time_boot_ms(&msg);
      if(time_ref){
        actual_time_us = boot_hour_us + (time_boot_ms*1000);
      }
      att_yaw = mavlink_msg_attitude_get_yaw(&msg);
      att_roll = mavlink_msg_attitude_get_roll(&msg);
      att_pitch = mavlink_msg_attitude_get_pitch(&msg); 
      att_yaw_speed = mavlink_msg_attitude_get_yawspeed(&msg);
      att_roll_speed = mavlink_msg_attitude_get_rollspeed(&msg);
      att_pitch_speed = mavlink_msg_attitude_get_pitchspeed(&msg); 
      break;
    }
    case MAVLINK_MSG_ID_MISSION_ITEM:{
      received = 1;
      uint16_t seq = mavlink_msg_mission_item_get_seq(&msg);
      mission_wps[seq].lat = (mavlink_msg_mission_item_get_x(&msg));// latitud
      mission_wps[seq].lon = (mavlink_msg_mission_item_get_y(&msg));// longitud
      mission_wps[seq].alt = (mavlink_msg_mission_item_get_z(&msg));// altitud
      sermon.print("WP recibido:");
      sermon.println(seq);
      sermon.print("Latitud ");
      sermon.println(mission_wps[seq].lat);
      sermon.print("Longitud ");
      sermon.println(mission_wps[seq].lon);
      sermon.print("Altitud ");
      sermon.println(mission_wps[seq].alt);
      break;            
    }
    default:
    sermon.print("mensaje diferente, ID =");
    sermon.println(msg.msgid);
    break;
  }
}



void datasend_sensor(){
  uint8_t datasensors[70];
  int ii = 0;
  datasensors[ii] = 0x00;
  ii++;
  datasensors[ii] = (r >> 8 ) & 255;
  ii++;
  datasensors[ii] = r & 255;
  ii++;
  datasensors[ii] = (s >> 8 ) & 255;
  ii++;
  datasensors[ii] = s & 255;
  ii++;
  datasensors[ii] = (sensor_dist[0] >> 8 ) & 255;
  ii++;
  datasensors[ii] = sensor_dist[0] & 255;
  ii++;
  datasensors[ii] = (sensor_dist[1] >> 8 ) & 255;
  ii++;
  datasensors[ii] = sensor_dist[1] & 255;
  ii++;
  datasensors[ii] = (sensor_dist[2] >> 8 ) & 255;
  ii++;
  datasensors[ii] = sensor_dist[2] & 255;
  ii++;
  datasensors[ii] = (sensor_dist[3] >> 8 ) & 255;
  ii++;
  datasensors[ii] = sensor_dist[3] & 255;
  ii++;
  datasend(ii, datasensors);
}

void datasend_image(){
  uint8_t dataimage[70];
  for(int j=0;j<80;j++){
    for(int msg_ind=0; msg_ind<2; msg_ind++){
      int ii = 0;
      dataimage[ii] = 0x01;
      ii++;
      dataimage[ii] = 2*j + msg_ind;
      ii++;
      for(int i=(msg_ind*30);i<(msg_ind*30+30);i++){
        dataimage[ii] = (image[i][j] >> 8 ) & 255;
        ii++;
        dataimage[ii] = image[i][j] & 255;
        ii++;
      }
      datasend(ii, dataimage);
    }
  }
}
