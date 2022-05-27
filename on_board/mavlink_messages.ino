//Función de actualizacion del estado del Pixhawk
void up_date(){
  mavlink_message_t msg; 
  mavlink_status_t stat;
  while(sermav.available()>0) {     
    uint8_t c = sermav.read();
    if(mavlink_parse_char(MAVLINK_COMM_0, c, &msg, &stat)) {
      manageMSG(msg);   //Chequeo de mensajes disponibles provenientes del Pixhawk
    }
  }
  if(start_log){
    bat_check();        //Chequeo de la carga de la batería
    //Nacho lepton_status();    //Chequeo del estado de la Flir Lepton
    #ifdef SD_present
      sd_log_save();    //Se actualiza el log.txt
    #endif
  }
  #ifdef debugging
//    sermon.println("Actualizado");
  #endif
}

//Función para solicitar al Pixhawk todos los waypoint de la mision
void request_all_mission(){
  requestMSG(per_mav_msg/2, MAVLINK_MSG_ID_HOME_POSITION);//
  waitfor(MAVLINK_MSG_ID_HOME_POSITION, timeout_mav_msg);//
  //condicion si no encuentra el mensaje de Home
  missionrequest();
  waitfor(MAVLINK_MSG_ID_MISSION_COUNT, timeout_mav_msg);
  for (int i = 0; i < count; i++){
    waypointrequest(i);
    waitfor(MAVLINK_MSG_ID_MISSION_ITEM, timeout_mav_msg);
  }
  missionack();
  shutup(MAVLINK_MSG_ID_HOME_POSITION);     //requestMSG( 0, MAVLINK_MSG_ID_HOME_POSITION);     //Dejar de pedirlo
}

//Funcion para Volver a enviar al Pixhawk su mision original
void recover_mission() {
    clearall();
    sendwaypointcount(count);
    //for (int i = 0; i < count; i++){
    while(seq < count){
      waitfor(MAVLINK_MSG_ID_MISSION_REQUEST, timeout_mav_msg);
      if(seq==auxseq){
        current = 1; // Punto actual TRUE
      }else{
        current = 0;
      }
      autocontinue  = 1; // True 
      param1=0; // Tiempo de hold  
      param2=0; // Radio de aceptacion en metros
      param3=0; // 0 para pasar por el WP
      param4=0; // YAW deseado
      command  =  MAV_CMD_NAV_WAYPOINT; // CMD_ID = 16
      sendwaypoint(seq,frame,command,current,autocontinue,param1,param2,param3,param4,mission_wps[seq].lat,mission_wps[seq].lon,mission_wps[seq].alt);
    }
    waitfor(MAVLINK_MSG_ID_MISSION_ACK, timeout_mav_msg);
    sendwaypointcurrent(auxseq);
    waitfor(MAVLINK_MSG_ID_MISSION_CURRENT, timeout_mav_msg);
}

//Funcion para enviar un waypoint al cual el Pixhawk debe desplazarse
void send_wp() {
    clearall();
    sendwaypointcount(1);
    waitfor(MAVLINK_MSG_ID_MISSION_REQUEST, timeout_mav_msg);
    //si no acepta la mision
    if(received==1){
      seq = 0;
      current = 1; // Punto actual
      autocontinue  = 1; // True 
      param1=0; // Tiempo de hold  
      param2=0; // Radio de aceptacion en metros
      param3=0; // 0 para pasar por el WP
      param4=0; // YAW deseado
      command  =  MAV_CMD_NAV_WAYPOINT; // CMD_ID = 16
      sendwaypoint(seq,frame,command,current,autocontinue,param1,param2,param3,param4,new_wp.lat,new_wp.lon,new_wp.alt);
      waitfor(MAVLINK_MSG_ID_MISSION_ACK, timeout_mav_msg);  
    }else{
      #ifdef debugging 
        sermon.println("Waypoint rejected"); 
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
  sermon.println("Heartbeat enviado"); 
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
  sermon.println("Apaga msg");
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



//Para el envio de data a la GCS ATENCION!! Para que ande hay que solicitarAntena() y devolverAntena()
void datasend(const uint8_t* data){
  mavlink_message_t msg;
  uint8_t buf[MAVLINK_MAX_PACKET_LEN];
  mavlink_msg_file_transfer_protocol_pack(system_id, component_id, &msg,0,0,0, data);
  uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
  sermav.write(buf, len);
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
  #ifdef debugging
  sermon.println("Mision recibida OK");
  #endif
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
    sermon.print("Salio por timeout esperando mensaje #");
    sermon.println(MSG_ID);
    #endif
  }
}



void manageMSG(mavlink_message_t msg){
  //sermon.println(msg.msgid);
  switch(msg.msgid) {
    case MAVLINK_MSG_ID_HEARTBEAT: {
      received = 1;
      mode_flag = mavlink_msg_heartbeat_get_base_mode(&msg);
      #ifdef debugging
      sermon.println("Heartbeat recibido");
      #endif
      break;
    }
    case MAVLINK_MSG_ID_HOME_POSITION :{ //ID del mensaje 33
      received = 1;
      home_wp.lat = mavlink_msg_home_position_get_latitude(&msg)/1E7; // Latitud, expresada en grados * 1E7
      home_wp.lon = mavlink_msg_home_position_get_longitude(&msg)/1E7; // Longitud, expresada en grados * 1E7
      home_wp.alt = mavlink_msg_home_position_get_altitude(&msg)/1000; // Altitud en metros, expresada en milimetros, AMSL
      #ifdef SD_present
      myFile = SD.open(log_file, FILE_WRITE);
      sd_home_save();
      #endif
      
      #ifdef debugging
      sermon.print("Posicion del HOME");            
      sermon.print("  Latitud: ");
      sermon.print(home_wp.lat);
      sermon.print("  Longitud: ");
      sermon.print(home_wp.lon);
      sermon.print("  Altitud: ");
      sermon.println(home_wp.alt);
      #endif
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
      #ifdef debugging
      sermon.print("Bateria(%):");
      sermon.println(bat);
      #endif
      break;
    }
    case MAVLINK_MSG_ID_STATUSTEXT:{
      received = 1;
      severity = mavlink_msg_statustext_get_severity(&msg);
      mavlink_msg_statustext_get_text(&msg, statusText);
      #ifdef debugging
      sermon.print("STATUSTEXT severity:");
      sermon.println(severity);
      sermon.print("mensaje: ");
      sermon.println(statusText);
      #endif
      break;            
    }
    case MAVLINK_MSG_ID_SYSTEM_TIME:{
      received = 1;
      time_unix_usec = mavlink_msg_system_time_get_time_unix_usec(&msg);
      time_boot_ms = mavlink_msg_system_time_get_time_boot_ms(&msg);
      time_ref = (time_unix_usec != 0);
      if(time_ref){
        shutup(MAVLINK_MSG_ID_SYSTEM_TIME);   //requestMSG( 0, MAVLINK_MSG_ID_SYSTEM_TIME);     //Dejar de pedirlo
      }
      boot_hour_us = time_unix_usec - (time_boot_ms*1000);
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
      int32_t pos_lat=mavlink_msg_global_position_int_get_lat(&msg); // Latitud, expresada en grados * 1E7
      int32_t pos_lon=mavlink_msg_global_position_int_get_lon(&msg); // Longitud, expresada en grados * 1E7
      int32_t pos_alt=mavlink_msg_global_position_int_get_alt(&msg); // Altitud en metros, expresada en milimetros, AMSL
      int32_t pos_relative_alt=mavlink_msg_global_position_int_get_relative_alt(&msg); // Altitud sobre el suelo en metros, expresada como * 1000 (milimetros)
//      uint16_t pos_vx=mavlink_msg_global_position_int_get_vx(&msg); // Velocidad en tierra X (Latitud, norte positivo), expresada en m/s * 100
//      uint16_t pos_vy=mavlink_msg_global_position_int_get_vy(&msg); // Velocidad en tierra Y (Longitud, este positivo), expresada en m/s * 100
//      uint16_t pos_vz=mavlink_msg_global_position_int_get_vz(&msg); // Velocidad en tierra Z (Altitud, abajo positivo), expresada en m/s * 100
      uint16_t pos_hdg; // Direccion del vehiculo (angulo yaw) en grados * 100, 0.0..359.99 degrees. Si no se sabe se setea en UINT16_MAX
      actual_wp.lat = pos_lat / 1E7;
      actual_wp.lon = pos_lon / 1E7;
      actual_wp.alt = pos_relative_alt / 1000; //metros
      actual_yaw = pos_hdg;
      #ifdef debugging
      sermon.print("GPS recibido");            
      sermon.print("  Latitud: ");
      sermon.print(actual_wp.lat,7);
      sermon.print("  Longitud: ");
      sermon.print(actual_wp.lon,7);
      sermon.print("  Altitud: ");
      sermon.println(actual_wp.alt);
      #endif
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
      seq = mavlink_msg_mission_item_get_seq(&msg);
      mission_wps[seq].lat = (mavlink_msg_mission_item_get_x(&msg));// latitud
      mission_wps[seq].lon = (mavlink_msg_mission_item_get_y(&msg));// longitud
      mission_wps[seq].alt = (mavlink_msg_mission_item_get_z(&msg));// altitud
      #ifdef debugging
      sermon.print("WP recibido:");
      sermon.println(seq);
      sermon.print("  Latitud: ");
      sermon.print(mission_wps[seq].lat,7);
      sermon.print("  Longitud: ");
      sermon.print(mission_wps[seq].lon,7);
      sermon.print("  Altitud: ");
      sermon.println(mission_wps[seq].alt);
      #endif
      break;            
    }
    case MAVLINK_MSG_ID_MISSION_CURRENT: {
      received = 1;
      seq = mavlink_msg_mission_current_get_seq(&msg);
      #ifdef debugging
      sermon.print("Wp al que va:");
      sermon.println(seq);
      #endif
      break;
    }
    case MAVLINK_MSG_ID_MISSION_COUNT: {
      received = 1;
      count = mavlink_msg_mission_count_get_count(&msg);
      #ifdef debugging
      sermon.print("Cantidad de WP en la mision:");
      sermon.println(count);
      #endif
      break;
    }
    case MAVLINK_MSG_ID_MISSION_ACK: {
      received = 1;
      #ifdef debugging
      sermon.println("Mission ACK recibido");
      #endif
      break;            
    }
    case MAVLINK_MSG_ID_MISSION_REQUEST: {
      received = 1;
      seq = mavlink_msg_mission_request_get_seq(&msg);
      #ifdef debugging
      sermon.print("Mission Request waypoint:");
      sermon.println(seq);
      #endif
      break;            
    }
    case MAVLINK_MSG_ID_PING: {
      received = 1;
      #ifdef debugging
      sermon.print("PPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPing");
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
      received = 1;
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
      }
      break;            
    }
    default:
    #ifdef debugging
    sermon.print("Mensaje diferente, ID =");
    sermon.println(msg.msgid);
    #endif
    break;
  }
}


////////////////////////////////AUX Functions////////////////////////////////



void datasend_sensor(){
  uint8_t datasensors[DATATOGCSSIZE];
  int iDataSensor=0;
  datasensors[iDataSensor] = 0x00; // el 0 para avisar que es un sensor
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
  solicitarAntena();
  datasend(datasensors);
  devolverAntena();
}

void datasend_image(){
  
  uint8_t dataimage[DATATOGCSSIZE];
  int iDataImage=0;
    
  dataimage[iDataImage] = 1; // Primer byte indica que es una imagen
  iDataImage++;
  solicitarAntena(); // Desconectamos a Pixhawk y conectamos a Arduino
  
  for(int fila = 0 ;fila < image_x ; fila++){ // recorro todas las filas (de 0 a 59) y las voy mandando de a 2, adentro se aumenta de nuevo
  
    dataimage[iDataImage]  = fila;
    iDataImage++; // acá debe quedar en 2 siempre
    
    //guardamos una fila 
  
    for(int columna=0;columna<image_y;columna++){ // recorro toda la fila (de la columna 0 a la 79) y la voy copiando al dataimage
      dataimage[iDataImage]  = (image[fila][columna]>> 8 ) & 255; // image tiene elementos de 16 bits y data de 8, así que hay que hacer un shift para pasarlos
      iDataImage++;
      dataimage[iDataImage]  = (image[fila][columna]) & 255;
      iDataImage++;
    }
  
    fila++; // aumentamos la fila y guardamos otra
  
    for(int columna=0;columna<image_y;columna++){ 
      dataimage[iDataImage]  = (image[fila][columna]>> 8 ) & 255; 
      iDataImage++;
      dataimage[iDataImage]  = (image[fila][columna]) & 255;
      iDataImage++;
    }
    datasend(dataimage); // acá se mandan las dos filas
    iDataImage=1; // reseteamos el indice de la imagen para el próximo mensaje
  }
  devolverAntena(); // Desconectamos a Arduino y conectamos a Pixhawk (ya está el delay adentro)
}

void datasend_error(int type_err){    //1:Resync; 2:SD; 3:Mavlink;
  uint8_t dataerror[70];
  dataerror[0] = 0x02;
  dataerror[1] = type_err;
  solicitarAntena();
  datasend(dataerror);
  devolverAntena();
}



void solicitarAntena(){
  digitalWrite(PXEnablePin,HIGH); // Desconectamos a Pixhawk
  digitalWrite(ArdEnablePin,LOW);// Conectamos a Arduino
}

void devolverAntena(){
  delay(50); // Delay para permitir al buffer de salida escribir todo el mensaje
  digitalWrite(ArdEnablePin,HIGH);// Desconectamos a Arduino
  digitalWrite(PXEnablePin,LOW); // Conectamos a Pixhawk
}
