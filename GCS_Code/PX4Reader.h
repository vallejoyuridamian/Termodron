void stabilizeGimbal();
void configureGimbal();
void shutup(int MSG_ID_SHUTUP);
void requestMSG(int MSG_ID_REQUEST, float timeus);
void actualizarFlags();
void picture_print();
void sendFlagsToOnBoard();
void sendACK();
void sendNACK();
void checkImage(int columna);
void processDataFromOnBoard();
void datarecive_sensor();
void datarecive_image();
void datarecive_error();
void datarecive_log();
void datarecive_mailto();
void ActualizarImagen();
void chequearPin(int pin);
void conmutarPin(int pin);
void pingOnBoard();
void ResetXmodem();


void PX4ReaderCallBack() {

  /*    HABILITAR ESTO PARA VER LOS DATOS CRUDOS EN HEXA
    while(PX4.available()>0){

    uint8_t c = PX4.read();
    //if(c<16){
    //Consola.print('0')
    }
    Consola.println(c,HEX);

    }
  */


  mavlink_message_t msg;
  mavlink_status_t stat;
  while (PX4.available() > 0) {
    uint8_t c = PX4.read();
    chequearPin(pinAntenaViva);

    if (mavlink_parse_char(MAVLINK_COMM_0, c, &msg, &stat)) {

#ifdef  ConsolaMon
      Consola.print("Mensaje recibido, ID= ");
      Consola.println(msg.msgid);
#endif

      switch (msg.msgid) {
        case MAVLINK_MSG_ID_ATTITUDE: {  //ID del mensaje 30

            yaw = mavlink_msg_attitude_get_yaw(&msg) * 180 / PI;
            pitch = mavlink_msg_attitude_get_pitch(&msg) * 180 / PI;
            roll = mavlink_msg_attitude_get_roll(&msg) * 180 / PI;

            if (!configured) {
              configureGimbal();
              configured = true;
            }
            stabilizeGimbal();

            break;
          }
        case MAVLINK_MSG_ID_BATTERY_STATUS: {  //ID del mensaje #147

            carga_bateria = mavlink_msg_battery_status_get_battery_remaining(&msg);
            int16_t current_battery = mavlink_msg_battery_status_get_current_battery(&msg);
            uint16_t voltages[10];
            mavlink_msg_battery_status_get_voltages(&msg, voltages);
#ifdef  ConsolaMon
            Consola.print(F("Carga remanente= "));
            Consola.print(carga_bateria);
            Consola.println(F(" %"));
            Consola.print(F("Tension Celda 1 = "));
            Consola.print(voltages[0]);
            Consola.println(F(" mV"));
            Consola.print(F("Tension Celda 2 = "));
            Consola.print(voltages[1]);
            Consola.println(F(" mV"));
            Consola.print(F("Tension Celda 3 = "));
            Consola.print(voltages[2]);
            Consola.println(F(" mV"));
            Consola.print(F("Corriente = "));
            Consola.print(current_battery * 10);
            Consola.println(F(" mA"));
#endif

            if (((voltages[0] < VminCell) or (voltages[1] < VminCell) or (voltages[2] < VminCell)) and
                ((voltages[0] > 0) or (voltages[1] > 0) or (voltages[2] > 0)) and (Pixhawk_landed_state == MAV_LANDED_STATE_IN_AIR)) {
#ifdef ConsolaMon
              Consola.println(F("****************************"));
              Consola.println(F("ATERRIZANDO POR BAJA BATERIA"));
              Consola.println(F("****************************"));
#endif
              land();
            }


            break;
          }
        case MAVLINK_MSG_ID_MISSION_CURRENT: {  //ID del mensaje #42

            //requestMSG(MAVLINK_MSG_ID_MISSION_CURRENT,1e5);

            break;
          }

        case MAVLINK_MSG_ID_WIND_COV: {  //ID del mensaje #231

            shutup(MAVLINK_MSG_ID_WIND_COV);

            break;
          }


        case MAVLINK_MSG_ID_SYS_STATUS: { // ID del mensaje 1

            //Consola.println("Estado del sistema recibido");

            break;
          }

        case MAVLINK_MSG_ID_ATTITUDE_TARGET: { //ID del mensaje 83

            shutup(MAVLINK_MSG_ID_ATTITUDE_TARGET);

            break;
          }

        case MAVLINK_MSG_ID_HEARTBEAT : { //ID del mensaje #0

            uint8_t PX4_STATUS = mavlink_msg_heartbeat_get_system_status(&msg);

            uint32_t PX4_ARMED_AUX = mavlink_msg_heartbeat_get_base_mode(&msg);

            if ((PX4_ARMED_AUX) & (MAV_MODE_FLAG_DECODE_POSITION_SAFETY)) {
              ARMED_STATUS = ARMED_STATUS_ARMED;
            }
            else {
              ARMED_STATUS = ARMED_STATUS_DISARMED;
            }

            // Aca estaria en modo mision
            if ((PX4_ARMED_AUX) & (MAV_MODE_FLAG_GUIDED_ENABLED)) {
              INPUT_STATUS = INPUT_STATUS_MISSION;
            }
            else {
              if ((PX4_ARMED_AUX) & (MAV_MODE_FLAG_MANUAL_INPUT_ENABLED)) {
                INPUT_STATUS = INPUT_STATUS_MANUAL;
              }
              else {
                INPUT_STATUS = INPUT_STATUS_UNKNOWN;
              }
            }

#ifdef  ConsolaMon
            Consola.print(F("Heartbeat recibido: "));
#endif

#ifdef  ConsolaMon
            Consola.print(F("Actividad: "));
#endif

            if (PX4_STATUS == MAV_STATE_ACTIVE) {
              PIXHAWK_STATE = PIXHAWK_MISION;
#ifdef  ConsolaMon
              Consola.print(F("ACTIVO; "));
#endif

            }

            if (PX4_STATUS == MAV_STATE_STANDBY) {
              PIXHAWK_STATE = PIXHAWK_STANDBY;
#ifdef  ConsolaMon
              Consola.print(F("STANDBY; "));
#endif
            }

            if (ARMED_STATUS == ARMED_STATUS_DISARMED) {
#ifdef  ConsolaMon
              Consola.print(F("DESARMADO; "));
#endif
            }

            if (ARMED_STATUS == ARMED_STATUS_ARMED) {
#ifdef  ConsolaMon
              Consola.print(F("ARMADO; "));
#endif
            }

            if (ARMED_STATUS == ARMED_STATUS_UNKNOWN) {
#ifdef  ConsolaMon
              Consola.print(F("DESCONOCIDO; "));
#endif
            }

            if (Pixhawk_landed_state == MAV_LANDED_STATE_UNDEFINED) {
#ifdef ConsolaMon
              Consola.print(F("INDEFINIDO; "));
#endif
            }
            if (Pixhawk_landed_state == MAV_LANDED_STATE_ON_GROUND) {
#ifdef ConsolaMon
              Consola.print(F("EN TIERRA; "));
#endif
            }
            if (Pixhawk_landed_state == MAV_LANDED_STATE_IN_AIR) {
#ifdef ConsolaMon
              Consola.print(F("EN AIRE; "));
#endif
            }
            if (INPUT_STATUS == INPUT_STATUS_MISSION) {
#ifdef ConsolaMon
              Consola.print(F("AUTONOMO"));
#endif
            }
            if (INPUT_STATUS == INPUT_STATUS_MANUAL) {
#ifdef ConsolaMon
              Consola.print(F("MANUAL"));
#endif
            }

            if (INPUT_STATUS == INPUT_STATUS_UNKNOWN) {
#ifdef ConsolaMon
              Consola.print(F("DESCONOCIDO"));
#endif
            }

#ifdef  ConsolaMon
            Consola.println();
#endif

            break;
          }

        case MAVLINK_MSG_ID_PING : { //ID del mensaje 4

            break;

          }


        case MAVLINK_MSG_ID_GLOBAL_POSITION_INT : { //ID del mensaje #33

            Timestamp_boot_PX = mavlink_msg_global_position_int_get_time_boot_ms(&msg); // Timestamp (milisegundos desde el booteo del sistema)

            latitud_int_1E7 = mavlink_msg_global_position_int_get_lat(&msg); // Latitud, expresada en grados * 1E7
            longitud_int_1E7 = mavlink_msg_global_position_int_get_lon(&msg); // Longitud, expresada en grados * 1E7
            altitud_int = mavlink_msg_global_position_int_get_alt(&msg) / 1000; // Altitud en metros



#ifdef GPSprueba
            Consola.print("GLOBAL_POSITION_INT (lat_lon_alt_tiempo: ");
            Consola.print(latitud_int_1E7);
            Consola.print("\t");
            Consola.print(longitud_int_1E7);
            Consola.print("\t");
            Consola.print(altitud_int);
            Consola.println();

#endif

            break;

          }


        case MAVLINK_MSG_ID_GPS_RAW_INT : { //ID del mensaje #24

            Timestamp_boot_PX = mavlink_msg_global_position_int_get_time_boot_ms(&msg); // Timestamp (milisegundos desde el booteo del sistema)

            latitud_int_1E7 = mavlink_msg_gps_raw_int_get_lat(&msg); // Latitud, expresada en grados * 1E7
            longitud_int_1E7 = mavlink_msg_gps_raw_int_get_lon(&msg); // Longitud, expresada en grados * 1E7
            altitud_int = mavlink_msg_gps_raw_int_get_alt(&msg); // Altitud en metros
            time_usec = mavlink_msg_gps_raw_int_get_time_usec(&msg); // tiempo en us UNIX o desde el booteo
#ifdef GPSprueba
            Consola.print("GPS_RAW_INT (lat_lon_alt_tiempo: ");
            Consola.print(latitud_int_1E7);
            Consola.print("\t");
            Consola.print(longitud_int_1E7);
            Consola.print("\t");
            Consola.print(altitud_int);
            Consola.println();
#endif
            break;
          }

        case MAVLINK_MSG_ID_COMMAND_ACK : { //ID del mensaje 77
#ifdef  ConsolaMon
            Consola.print(F("Command Acknowledge recibido, t= "));
            Consola.println(millis() / 1000);
#endif
            result_command_ack = mavlink_msg_command_ack_get_result(&msg);
            switch (result_command_ack) {
              case MAV_RESULT_ACCEPTED: {
#ifdef  ConsolaMon
                  Consola.println(F("MAV_RESULT_ACCEPTED")); //Command ACCEPTED and EXECUTED
#endif
                  break;
                }
              case MAV_RESULT_TEMPORARILY_REJECTED: {
#ifdef  ConsolaMon
                  Consola.println(F("MAV_RESULT_TEMPORARILY_REJECTED")); //Command TEMPORARY REJECTED/DENIED
#endif
                  break;
                }
              case MAV_RESULT_DENIED: {
#ifdef  ConsolaMon
                  Consola.println(F("MAV_RESULT_DENIED")); //Command PERMANENTLY DENIED
#endif
                  break;
                }
              case MAV_RESULT_UNSUPPORTED: {
#ifdef  ConsolaMon
                  Consola.println(F("MAV_RESULT_UNSUPPORTED")); //Command UNKNOWN/UNSUPPORTED
#endif
                  break;
                }
              case MAV_RESULT_FAILED: {
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

            }
            break;
          }

        case MAVLINK_MSG_ID_MISSION_COUNT : { // #44
            count = mavlink_msg_mission_count_get_count(&msg);
#ifdef  ConsolaMon
            Consola.print(F("La mision tiene "));
            Consola.print(count);
            Consola.println(F(" elementos"));
#endif
            break;
          }


        case MAVLINK_MSG_ID_MISSION_ITEM : { // #39

            uint16_t seq = mavlink_msg_mission_item_get_seq(&msg);
#ifdef  ConsolaMon
            Consola.print("Item ");
            Consola.print(seq);
            Consola.println(F(" recibido"));
#endif
            // latitud
            double lat = mavlink_msg_mission_item_get_x(&msg);
            double lon = mavlink_msg_mission_item_get_y(&msg);
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

        case MAVLINK_MSG_ID_RC_CHANNELS : { // #65
            shutup(MAVLINK_MSG_ID_RC_CHANNELS);
            break;
          }

        case MAVLINK_MSG_ID_RC_CHANNELS_OVERRIDE : { // #70
            shutup(MAVLINK_MSG_ID_RC_CHANNELS_OVERRIDE);
            break;
          }




        case MAVLINK_MSG_ID_SERIAL_CONTROL: { // #126
#ifdef  ConsolaMon
            Consola.println();
            Consola.println("Serial Control Recibido");
            Consola.println();
#endif

            serial_control_count = mavlink_msg_serial_control_get_count(&msg);
            mavlink_msg_serial_control_get_data(&msg, data_on_board);

            if ((data_on_board[0] == 0) & (serial_control_count == 14)) {
              datarecive_sensor();
            } else if (data_on_board[0] == 1) {
              datarecive_image();
            } else if (data_on_board[0] == 2) {
              datarecive_error();
            }  else if (data_on_board[0] == 5) {
              datarecive_mailto();
            } 
            else {
#ifdef  ConsolaMon
              Consola.println("Serial Control Data Error");
#endif
            }
            break;
          }




        case MAVLINK_MSG_ID_EXTENDED_SYS_STATE: { // #245

            Pixhawk_landed_state_previous;
            Pixhawk_landed_state = mavlink_msg_extended_sys_state_get_landed_state(&msg);

            if (apurarEstadoExtendido) {
              requestMSG(MAVLINK_MSG_ID_EXTENDED_SYS_STATE, PERIODO_MAVLINK_MSG_ID_EXTENDED_SYS_STATE);
              apurarEstadoExtendido = false;
            }
#ifdef ConsolaMon
            Consola.print(F("Estado extendido del dron: "));
#endif
            switch (Pixhawk_landed_state) {

              case MAV_LANDED_STATE_UNDEFINED: {
#ifdef ConsolaMon
                  Consola.println(F("Indefinido"));
#endif
                  break;
                }
              case MAV_LANDED_STATE_ON_GROUND: {
#ifdef ConsolaMon
                  Consola.println(F("En tierra"));
#endif
                  if (Pixhawk_landed_state_previous == MAV_LANDED_STATE_IN_AIR) {
                    flg_LandingDetected = true;
                  }
                  break;
                }
              case MAV_LANDED_STATE_IN_AIR: {
#ifdef ConsolaMon
                  Consola.println(F("En aire"));
#endif
                  if (Pixhawk_landed_state_previous == MAV_LANDED_STATE_ON_GROUND) {
                    flg_TakeOffDetected = true;
                  }
                  break;
                }
                Pixhawk_landed_state_previous = Pixhawk_landed_state;

            }
            break;
          }

        case MAVLINK_MSG_ID_HOME_POSITION: { // #242

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


        case MAVLINK_MSG_ID_STATUSTEXT: { // # 253

            statusTextSeverity = mavlink_msg_statustext_get_severity(&msg);
            statusTextTextLen = mavlink_msg_statustext_get_text(&msg, &statusTextText[0]);

#ifdef  ConsolaMon
            Consola.println(F("STATUSTEXT RECIBIDO"));
            Consola.print(F("SEVERITY: "));
            Consola.println(statusTextSeverity);
            Consola.print(F("TEXT: "));
            for (int i = 0; i < statusTextTextLen; i++) {
              Consola.print(statusTextText);
            }
            Consola.println();
#endif

            break;
          }

        case MAVLINK_MSG_ID_HIL_ACTUATOR_CONTROLS: { // ID del mensaje 93
            shutup(MAVLINK_MSG_ID_HIL_ACTUATOR_CONTROLS);
            break;
          }

        case MAVLINK_MSG_ID_HIL_CONTROLS: { // ID del mensaje 91
            shutup(MAVLINK_MSG_ID_HIL_CONTROLS);
            break;
          }

        case MAVLINK_MSG_ID_VFR_HUD: { // ID del mensaje 74
            shutup(MAVLINK_MSG_ID_VFR_HUD);
            break;
          }


        case MAVLINK_MSG_ID_FILE_TRANSFER_PROTOCOL: {  //ID del mensaje #110
            // Aca tenemos la data tranferida
            mavlink_msg_file_transfer_protocol_get_payload(&msg, data_on_board);
            if ((!Xmodem.isEnabled()) and (!XmodemReader.isEnabled())) {
              processDataFromOnBoard();
            }
            else {
              #ifdef ConsolaMon
              Consola.println("IGNORANDO ON-BOARD POR XMODEM");
              #endif
              //ResetXmodem();
            }
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
}

void pingOnBoard() {
  mavlink_message_t msg; // crea un mensaje
  uint8_t buf[MAVLINK_MAX_PACKET_LEN]; // crea un buffer para guardarlo
  mavlink_msg_ping_pack(system_id, component_id, &msg, 0, 0, 1, 1); // Arma el mensaje
  uint16_t len = mavlink_msg_to_send_buffer(buf, &msg); //guarda el mensaje en el buffer y devuelve el largo del mensaje (payload+no payload)
  PX4.write(buf, len); // manda el mensaje
#ifdef  ConsolaMon
  Consola.print(F("PING ENVIADO ")); // Imprime que lo mandó
  Consola.println(millis());
#endif

}

void serialControl() {
  mavlink_message_t msg; // crea un mensaje
  uint8_t buf[MAVLINK_MAX_PACKET_LEN]; // crea un buffer para guardarlo

  mavlink_message_t msg_ob; // crea un mensaje
  uint8_t data_sc[70]; // crea un buffer para guardarlo
  mavlink_msg_ping_pack(system_id, component_id, &msg_ob, 0, 0, 253, 253); // Arma el mensaje
  uint8_t count_sc = mavlink_msg_to_send_buffer(data_sc, &msg_ob); //guarda el mensaje en el buffer y devuelve el largo del mensaje (payload+no payload)


  mavlink_msg_serial_control_pack(system_id, component_id, &msg, SERIAL_CONTROL_DEV_TELEM1, 0, 1e3, 0, count_sc, data_sc); // Arma el mensaje
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
  for (int i = 1; i < 290; i++) {
    mavlink_message_t msg;
    uint8_t buf[MAVLINK_MAX_PACKET_LEN];
    mavlink_msg_command_long_pack(system_id, component_id, &msg, target_system, target_component, MAV_CMD_SET_MESSAGE_INTERVAL, confirmation, i, disable, param3, param4, xf, yf, zf);
    uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
    PX4.write(buf, len);
  }//for
}//shutup

void shutup(int MSG_ID_SHUTUP) {
  mavlink_message_t msg;
  uint8_t buf[MAVLINK_MAX_PACKET_LEN];
  mavlink_msg_command_long_pack(system_id, component_id, &msg, target_system, target_component, MAV_CMD_SET_MESSAGE_INTERVAL, confirmation, MSG_ID_SHUTUP, disable, param3, param4, xf, yf, zf);
  uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
  PX4.write(buf, len);
}//shutup



void requestGPS(double t) {
  mavlink_message_t msg;
  uint8_t buf[MAVLINK_MAX_PACKET_LEN];
  mavlink_msg_command_long_pack(system_id, component_id, &msg, target_system, target_component, MAV_CMD_SET_MESSAGE_INTERVAL, confirmation, MAVLINK_MSG_ID_GLOBAL_POSITION_INT, t, param3, param4, xf, yf, zf);
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

void waypointrequest(uint16_t seq) {
  mavlink_message_t msg;
  uint8_t buf[MAVLINK_MAX_PACKET_LEN];
  mavlink_msg_mission_request_pack(system_id, component_id, &msg, target_system, target_component, seq);
  uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
  PX4.write(buf, len);
}

void sendwaypointack() {
  mavlink_message_t msg;
  uint8_t buf[MAVLINK_MAX_PACKET_LEN];
  mavlink_msg_mission_ack_pack(system_id, component_id, &msg, target_system, target_component, MAV_RESULT_ACCEPTED);
  uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
  PX4.write(buf, len);
}

void sendwaypointcount(uint16_t count) {
  mavlink_message_t msg;
  uint8_t buf[MAVLINK_MAX_PACKET_LEN];
  mavlink_msg_mission_count_pack(system_id, component_id, &msg, target_system, target_component, count);
  uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
  PX4.write(buf, len);
}



void sendwaypointInt(uint16_t seq, uint8_t frame, uint16_t command, uint8_t current, uint8_t autocontinue, double param1, double param2, double param3, double param4, int32_t x, int32_t y, float z) {
  mavlink_message_t msg;
  uint8_t buf[MAVLINK_MAX_PACKET_LEN];
  mavlink_msg_mission_item_int_pack(system_id, component_id, &msg, target_system, target_component, seq, frame, command, current, autocontinue, param1, param2, param3, param4, x, y, z);
  uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
  PX4.write(buf, len);
}

void sendwaypointFloat(uint16_t seq, uint8_t frame, uint16_t command, uint8_t current, uint8_t autocontinue, double param1, double param2, double param3, double param4, float x, float y, float z) {
  mavlink_message_t msg;
  uint8_t buf[MAVLINK_MAX_PACKET_LEN];
  mavlink_msg_mission_item_int_pack(system_id, component_id, &msg, target_system, target_component, seq, frame, command, current, autocontinue, param1, param2, param3, param4, x, y, z);
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



void clearall() {
  mavlink_message_t msg;
  uint8_t buf[MAVLINK_MAX_PACKET_LEN];
  mavlink_msg_mission_clear_all_pack(system_id, component_id, &msg, target_system, target_component);
  uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
  PX4.write(buf, len);
}

void waitfor(uint8_t MSG_ID, uint32_t timeout) {
  uint32_t timestamp = millis();
  received = false;
  mavlink_message_t msg;
  mavlink_status_t stat;
  while ((received == false) && (millis() < timestamp + timeout)) { // TODO este while esta mal
    while (PX4.available() > 0) {
      uint8_t c = PX4.read();
      chequearPin(pinAntenaViva);

      if (mavlink_parse_char(MAVLINK_COMM_0, c, &msg, &stat)) {
        switch (msg.msgid) {

          case MAVLINK_MSG_ID_STATUSTEXT: { // ID del mensaje #253

              statusTextSeverity = mavlink_msg_statustext_get_severity(&msg);
              statusTextTextLen = mavlink_msg_statustext_get_text(&msg, &statusTextText[0]);

#ifdef  ConsolaMon
              Consola.println(F("STATUSTEXT RECIBIDO"));
              Consola.print(F("SEVERITY: "));
              Consola.println(statusTextSeverity);
              Consola.print(F("TEXT: "));
              for (int i = 0; i < statusTextTextLen; i++) {
                Consola.print(statusTextText);
              }
              Consola.println();
#endif

              break;
            }

          case MAVLINK_MSG_ID_MISSION_REQUEST_INT: { // #51
              if (MSG_ID == MAVLINK_MSG_ID_MISSION_REQUEST_INT) {
                received = true;
                uint16_t sequ = mavlink_msg_mission_request_get_seq(&msg);
#ifdef  ConsolaMon
                Consola.print(F("mensaje esperado recibido, ID ="));
                Consola.println(msg.msgid);
                Consola.println(sequ);
#endif
                seq = sequ;
              }
              break;
            }

          case MAVLINK_MSG_ID_MISSION_REQUEST: {  // #40
              if (MSG_ID == MAVLINK_MSG_ID_MISSION_REQUEST) {
                received = true;
                uint16_t sequ = mavlink_msg_mission_request_get_seq(&msg);
                seq = sequ;
#ifdef ConsolaMon
                Consola.print(F("mensaje esperado recibido, ID ="));
                Consola.println(msg.msgid);
                Consola.println(sequ);
#endif
              }
              else {
#ifdef ConsolaMon
                Consola.print(F("mensaje diferente, ID ="));
                Consola.println(msg.msgid);
#endif
              }
              break;
            }//case
          case MAVLINK_MSG_ID_MISSION_ACK: {
              if (MSG_ID == MAVLINK_MSG_ID_MISSION_ACK) {
                received = true;
#ifdef  ConsolaMon
                Consola.print(F("mensaje esperado recibido, ID ="));
                Consola.println(msg.msgid);
#endif
              }//if
              else {
#ifdef  ConsolaMon
                Consola.print(F("mensaje diferente, ID ="));
                Consola.println(msg.msgid);
#endif
              }//else
              break;
            }//case
          case MAVLINK_MSG_ID_MISSION_COUNT: {
              if (MSG_ID == MAVLINK_MSG_ID_MISSION_COUNT) {
                received = true;
#ifdef  ConsolaMon
                Consola.print(F("mensaje esperado recibido, ID ="));
                Consola.println(msg.msgid);
                count = mavlink_msg_mission_count_get_count(&msg);
#endif
              }//if
              else {
#ifdef  ConsolaMon
                Consola.print(F("mensaje diferente, ID ="));
                Consola.println(msg.msgid);
#endif
              }//else
              break;
            }//case
          case MAVLINK_MSG_ID_MISSION_CURRENT: {  //#42
              if (MSG_ID == MAVLINK_MSG_ID_MISSION_CURRENT) {
                received = true;
#ifdef  ConsolaMon
                Consola.print(F("mensaje esperado recibido, ID ="));
                Consola.println(msg.msgid);
                count = mavlink_msg_mission_count_get_count(&msg);
#endif
              }//if
              else {
#ifdef  ConsolaMon
                Consola.print(F("mensaje diferente, ID ="));
                Consola.println(msg.msgid);
#endif
              }//else
              break;
            }//case
          case MAVLINK_MSG_ID_GLOBAL_POSITION_INT : { // #33
              if (MSG_ID == MAVLINK_MSG_ID_GLOBAL_POSITION_INT) {
                GPSLock = 1;
                received = true;
#ifdef  ConsolaMon
                Consola.print(F("mensaje esperado recibido, ID ="));
                Consola.println(msg.msgid);
#endif
                uint32_t pos_time_boot_ms = mavlink_msg_global_position_int_get_time_boot_ms(&msg); // Timestamp (milisegundos desde el booteo del sistema
                int32_t pos_lat = mavlink_msg_global_position_int_get_lat(&msg); // Latitud, expresada en grados * 1E7
                int32_t pos_lon = mavlink_msg_global_position_int_get_lon(&msg); // Longitud, expresada en grados * 1E7
                int32_t pos_alt = mavlink_msg_global_position_int_get_alt(&msg); // Altitud en metros, expresada en milimetros, AMSL
                int32_t pos_relative_alt = mavlink_msg_global_position_int_get_relative_alt(&msg); // Altitud sobre el suelo en metros, expresada como * 1000 (milimetros)
                int16_t pos_vx = mavlink_msg_global_position_int_get_vx(&msg); // Velocidad en tierra X (Latitud, norte positivo), expresada en m/s * 100
                int16_t pos_vy = mavlink_msg_global_position_int_get_vy(&msg); // Velocidad en tierra Y (Longitud, este positivo), expresada en m/s * 100
                int16_t pos_vz = mavlink_msg_global_position_int_get_vz(&msg); // Velocidad en tierra Z (Altitud, abajo positivo), expresada en m/s * 100
                uint16_t pos_hdg; // Direccion del vehiculo (angulo yaw) en grados * 100, 0.0..359.99 degrees. su no se sabe se setea en UINT16_MAX
                xi = pos_lat;
                yi = pos_lon;
                zi = pos_alt;
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

                if (haveToSetHome) {
                  latitud_home_int_1E7 = xi;
                  longitud_home_int_1E7 = yi;
                  altitud_home_int_mm = zi;
                  haveToSetHome = false;
                }

#endif
              }//if
              else {
#ifdef  ConsolaMon
                Consola.print(F("mensaje diferente, ID ="));
                Consola.println(msg.msgid);
#endif
              }//else
              break;
            }//case
          case MAVLINK_MSG_ID_MISSION_ITEM: {
              if (MSG_ID == MAVLINK_MSG_ID_MISSION_ITEM) {
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
                Consola.print(lat * 1E7);
                Consola.println(F(" grados * 1E7"));
                Consola.print(F("Longitud "));
                Consola.print(lon * 1E7);
                Consola.println(F(" grados * 1E7"));
                Consola.print(F("Altitud "));
                Consola.print(alt);
                Consola.println(F(" metros"));
#endif
              }//if
              else {
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
  if (millis() >= timestamp + timeout) {
#ifdef  ConsolaMon
    Consola.print(F("Salio por timeout esperando mensaje #"));
    Consola.println(MSG_ID);
#endif
  } //if
} //waitfor


void missionrequest() {
  mavlink_message_t msg;
  uint8_t buf[MAVLINK_MAX_PACKET_LEN];
  mavlink_msg_mission_request_list_pack(system_id, component_id, &msg, target_system, target_component);
  uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
  PX4.write(buf, len);
}//missionrequest

void missionack() {
  mavlink_message_t msg;
  uint8_t type = MAV_MISSION_ACCEPTED; // 0 significa OK
  uint8_t buf[MAVLINK_MAX_PACKET_LEN];
  mavlink_msg_mission_ack_pack(system_id, component_id, &msg, target_system, target_component, type);
  uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
  PX4.write(buf, len);
  Consola.println(F("Mision recibida OK"));
}//waypointack

void setHome() {
  mavlink_message_t msg;
  uint8_t buf[MAVLINK_MAX_PACKET_LEN];
  mavlink_msg_set_home_position_pack(system_id, component_id, &msg, target_system, xi, yi, zi, 0, 0, 0, 0, 0, 0, 0);
  uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
  PX4.write(buf, len);
}

void arm() {
  mavlink_message_t msg;
  uint8_t buf[MAVLINK_MAX_PACKET_LEN];
  mavlink_msg_command_long_pack(system_id, component_id, &msg, target_system, target_component, MAV_CMD_COMPONENT_ARM_DISARM, confirmation, ARM, 0, 0, 0, 0, 0, 0);
  uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
  PX4.write(buf, len);
}

void disarm() {
  mavlink_message_t msg;
  uint8_t buf[MAVLINK_MAX_PACKET_LEN];
  mavlink_msg_command_long_pack(system_id, component_id, &msg, target_system, target_component, MAV_CMD_COMPONENT_ARM_DISARM, confirmation, DISARM, 0, 0, 0, 0, 0, 0);
  uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
  PX4.write(buf, len);
}



void setMode(uint16_t mode) { //
  mavlink_message_t msg;
  uint8_t buf[MAVLINK_MAX_PACKET_LEN];
  mavlink_msg_command_long_pack(system_id, component_id, &msg, target_system, target_component, MAV_CMD_DO_SET_MODE, confirmation, mode, 0, 0, 0, 0, 0, 0);
  uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
  PX4.write(buf, len);
}

boolean EnviarLandNow() {



  command = MAV_CMD_DO_LAND_START; // CMD_ID = 189
  param1 = 0;
  param2 = 0;
  param3 = 0;
  param4 = 0;
  param5 = 0;
  param6 = 0;
  param7 = 0;

  // Borro la mision actual
  //setMode(MAV_MODE_AUTO_ARMED);
  //disarm();
  // Espero un lock del GPS
  if (PIXHAWK_STATE == PIXHAWK_STANDBY) {
#ifdef  ConsolaMon
    Consola.println(F("Dron ya esta aterrizado"));
#endif
    return false;
  }

  // Comienza el intercamibo de mensajes (se carga la mision)

  received = false;
  sendwaypointreq = 0;

  while ((!received) and (sendwaypointreq < sendwaypointmax)) {

#ifdef ConsolaMon
    Consola.println();
    Consola.print("Enviando comando de aterrizar: ");
    Consola.print(1);
    Consola.print("por vez numero ");
    Consola.println(sendwaypointreq + 1);
#endif

    sendwaypointcount(1);

#ifdef CoordenadasEnFloat
    waitfor(MAVLINK_MSG_ID_MISSION_REQUEST, MISSIONREQUESTTIMEOUT);
#else
    waitfor(MAVLINK_MSG_ID_MISSION_REQUEST_INT, MISSIONREQUESTTIMEOUT);
#endif

    // Punto 0

    current = 0; // Punto actual
    autocontinue  = 1; // True
    param1 = 0; // Tiempo de hold
    param2 = 0.5; // Radio de aceptacion en metros
    param3 = 0; // 0 para pasar por el WP
    param4 = 0; // YAW deseado
#ifdef CoordenadasEnFloat
    sendwaypointFloat(seq, frame, command, current, autocontinue, param1, param2, param3, param4, param5, param6);
#else
    sendwaypointInt(seq, frame, command, current, autocontinue, param1, param2, param3, param4, param4, param5, param6);
#endif
    sendwaypointreq++;
  }

  if (!received) {
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

boolean EnviarMision() {
  // Borro la mision actual
  //setMode(MAV_MODE_AUTO_ARMED);
  //disarm();
  // Espero un lock del GPS
  if (Pixhawk_landed_state != MAV_LANDED_STATE_ON_GROUND) {
#ifdef  ConsolaMon
    Consola.println(F("No se puede garantizar que el dron esté en tierra, no se cargará la misión"));
#endif
    return false;
  }
  clearall();
  haveToSetHome = true;
  waitfor(MAVLINK_MSG_ID_GLOBAL_POSITION_INT, GPSTIMEOUT);
  if (GPSLock == 1) {
#ifdef  ConsolaMon
    Consola.println(F("GPSLock, seteando HOME"));
#endif
    setHome();
  }
  else {
#ifdef  ConsolaMon
    Consola.println(F("No hay GPSLock"));
#endif
    return false;
  }
  // Comienza el intercamibo de mensajes (se carga la mision)
  seq = 0;
  sendwaypointreq = 0;
  received = false;
  while ((!received) and (sendwaypointreq < sendwaypointmax)) {

#ifdef ConsolaMon
    Consola.println();
    Consola.print("Enviando la cuenta de WP: ");
    Consola.print(NPuntos);
    Consola.print(" puntos por vez numero ");
    Consola.println(sendwaypointreq + 1);
#endif

    sendwaypointcount(NPuntos + 1); // +1 para mandarle el home al final
#ifdef CoordenadasEnFloat
    waitfor(MAVLINK_MSG_ID_MISSION_REQUEST, MISSIONREQUESTTIMEOUT);
#else
    waitfor(MAVLINK_MSG_ID_MISSION_REQUEST_INT, MISSIONREQUESTTIMEOUT);
#endif

    // Punto 0
    command = MAV_CMD_NAV_TAKEOFF; // CMD_ID = 22
    current = 0; // Punto actual
    autocontinue  = 1; // True
    param1 = 0; // Tiempo de hold
    param2 = 0.5; // Radio de aceptacion en metros
    param3 = 0; // 0 para pasar por el WP
    param4 = 0; // YAW deseado
#ifdef CoordenadasEnFloat
#ifdef ConsolaMon
    Consola.println();
    Consola.print(F("Enviando punto (float): numero "));
    Consola.print(seq);
    Consola.print(F(" Latitud: "));
    Consola.print(float(Mision[0].latitud) * 1e-6, 7);
    Consola.print(F(", Longitud: "));
    Consola.print(float(Mision[0].longitud) * 1e-6, 7);
    Consola.print(F(", Altitud: "));
    Consola.print(float(Mision[0].altitud));
    Consola.println();
#endif
    sendwaypointFloat(seq, frame, command, current, autocontinue, param1, param2, param3, param4, float(Mision[0].latitud) * 1e-6, float(Mision[0].longitud) * 1e-6, float(Mision[0].altitud));
#else
#ifdef ConsolaMon
    Consola.println();
    Consola.print(F("Enviando punto (int): numero "));
    Consola.print(seq);
    Consola.print(F(" Latitud: "));
    Consola.print(int32_t(Mision[0].latitud));
    Consola.print(F(", Longitud: "));
    Consola.print(int32_t(Mision[0].longitud));
    Consola.print(F(", Altitud: "));
    Consola.print(Mision[0].altitud);
    Consola.println();
#endif
    sendwaypointInt(seq, frame, command, current, autocontinue, param1, param2, param3, param4, Mision[0].latitud, Mision[0].longitud, Mision[0].altitud);
#endif
    sendwaypointreq++;
  }

  if (!received) {
#ifdef ConsolaMon
    Consola.println();
    Consola.println("No fue posible enviar mision porque no se recibio respuesta al WP count.");
#endif
    return false;
  }


  command = MAV_CMD_NAV_WAYPOINT; // CMD_ID = 16

  mission_send_timeout = 0;

  while (seq < NPuntos - 1) { //Nacho

    seqprevious = seq;
#ifdef CoordenadasEnFloat
    waitfor(MAVLINK_MSG_ID_MISSION_REQUEST, MISSIONREQUESTTIMEOUT);
#else
    waitfor(MAVLINK_MSG_ID_MISSION_REQUEST_INT, MISSIONREQUESTTIMEOUT);
#endif
    seqactual = seq;
#ifdef ConsolaMon
    Consola.println();
    Consola.print(F("Enviando punto: numero "));
    Consola.println(seq);
    Consola.print(F("Latitud: "));
    Consola.print(int32_t(Mision[seq].latitud));
    Consola.print(F(", Longitud: "));
    Consola.print(int32_t(Mision[seq].longitud));
    Consola.print(F(", Altitud: "));
    Consola.print(Mision[seq].altitud);
    Consola.println();
#endif

#ifdef CoordenadasEnFloat
#ifdef ConsolaMon
    Consola.println("float");
#endif
    sendwaypointFloat(seq, frame, command, current, autocontinue, param1, param2, param3, param4, float(Mision[seq].latitud) * 1e-6, float(Mision[seq].longitud) * 1e-6, float(Mision[seq].altitud));
#else
#ifdef ConsolaMon
    Consola.println("int");
#endif
    sendwaypointInt(seq, frame, command, current, autocontinue, param1, param2, param3, param4, Mision[seq].latitud, Mision[seq].longitud, Mision[seq].altitud);
#endif


    if (seqprevious == seqactual) {
      mission_send_timeout++;
    }


    if (mission_send_timeout > mission_send_max) {
#ifdef ConsolaMon
      Consola.println("Se envio el ultimo waypoint muchas veces, se cancela el envio");
      Consola.print("Waypoint numero: ");
      Consola.print(seq);
#endif
      break;
    }
  }

  // mandamos el ultimo punto como home
  received = false;
  sendwaypointreq = 0;
  while ((!received) and (sendwaypointreq < sendwaypointmax)) {

#ifdef CoordenadasEnFloat
    waitfor(MAVLINK_MSG_ID_MISSION_REQUEST, MISSIONREQUESTTIMEOUT);
#else
    waitfor(MAVLINK_MSG_ID_MISSION_REQUEST_INT, MISSIONREQUESTTIMEOUT);
#endif

    // Punto N+1
    command = MAV_CMD_NAV_WAYPOINT; // CMD_ID = 22
    current = 0; // Punto actual
    autocontinue  = 1; // True
    param1 = 0; // Tiempo de hold
    param2 = 0.5; // Radio de aceptacion en metros
    param3 = 0; // 0 para pasar por el WP
    param4 = 0; // YAW deseado
#ifdef CoordenadasEnFloat
#ifdef ConsolaMon
    Consola.println();
    Consola.print(F("Enviando punto (float): numero "));
    Consola.print(seq);
    Consola.print(F(" Latitud: "));
    Consola.print(float(Mision[0].latitud) * 1e-6, 7);
    Consola.print(F(", Longitud: "));
    Consola.print(float(Mision[0].longitud) * 1e-6, 7);
    Consola.print(F(", Altitud: "));
    Consola.print(float(Mision[0].altitud));
    Consola.println();
#endif
    sendwaypointFloat(seq, frame, command, current, autocontinue, param1, param2, param3, param4, float(latitud_home_int_1E7) * 1e-6, float(longitud_home_int_1E7) * 1e-6, Mision[0].altitud);
#else
#ifdef ConsolaMon
    Consola.println();
    Consola.print(F("Enviando punto (int): numero "));
    Consola.print(seq);
    Consola.print(F(" Latitud: "));
    Consola.print(int32_t(latitud_home_int_1E7));
    Consola.print(F(", Longitud: "));
    Consola.print(int32_t(longitud_home_int_1E7));
    Consola.print(F(", Altitud: "));
    Consola.print(0);
    Consola.println();
#endif
    sendwaypointInt(seq, frame, command, current, autocontinue, param1, param2, param3, param4, latitud_home_int_1E7, longitud_home_int_1E7, Mision[0].altitud);
#endif
    sendwaypointreq++;
  }

  if (!received) {
#ifdef ConsolaMon
    Consola.println();
    Consola.println("No fue posible enviar mision porque no se recibio respuesta al WP count.");
#endif
    return false;
  }

  /*
    // aca se manda el RTL
    received=false;
    sendwaypointreq=0;
    while((!received)and(sendwaypointreq<sendwaypointmax)){


    #ifdef CoordenadasEnFloat
    waitfor(MAVLINK_MSG_ID_MISSION_REQUEST, MISSIONREQUESTTIMEOUT);
    #else
    waitfor(MAVLINK_MSG_ID_MISSION_REQUEST_INT, MISSIONREQUESTTIMEOUT);
    #endif

    // Punto N (RLT)
    command=MAV_CMD_NAV_RETURN_TO_LAUNCH; // CMD_ID = 20
    #ifdef CoordenadasEnFloat
    sendwaypointFloat(seq,frame,command,current,autocontinue,param1,param2,param3,param4,float(Mision[0].latitud)*1e-6,float(Mision[0].longitud)*1e-6,float(Mision[0].altitud));
    #else
    sendwaypointInt(seq,frame,command,current,autocontinue,param1,param2,param3,param4,Mision[0].latitud,Mision[0].longitud,Mision[0].altitud);
    #endif
    sendwaypointreq++;
    }
  */
  // antes de esperar el ACK mandamos el HOME como ultimo punto para que vuelva y aterrice

  waitfor(MAVLINK_MSG_ID_MISSION_ACK, MISSIONACKTIMEOUT);
  // Pide que se ponga en modo mision
  setMode(MAV_MODE_AUTO_ARMED);
  // Pide que arme
  arm();
  return true;
}

void requestMSG(int MSG_ID_REQUEST, float timeus) {
  mavlink_message_t msg;
  uint8_t buf[MAVLINK_MAX_PACKET_LEN];
  mavlink_msg_command_long_pack(system_id, component_id, &msg, target_system, target_component, MAV_CMD_SET_MESSAGE_INTERVAL, confirmation, MSG_ID_REQUEST, timeus, param3, param4, xf, yf, zf);
  uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
  PX4.write(buf, len);
}

boolean sendRTL() {
  mavlink_message_t msg;
  uint8_t buf[MAVLINK_MAX_PACKET_LEN];
  mavlink_msg_command_long_pack(system_id, component_id, &msg, target_system, target_component, MAV_CMD_NAV_RETURN_TO_LAUNCH, confirmation, 0, 0, param3, param4, xf, yf, zf);
  uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
  PX4.write(buf, len);
}

boolean land() {
  mavlink_message_t msg;
  uint8_t buf[MAVLINK_MAX_PACKET_LEN];
  mavlink_msg_command_long_pack(system_id, component_id, &msg, target_system, target_component, MAV_CMD_NAV_LAND, confirmation, 0, 0, param3, param4, xi * 1e-7, yf * 1e-7, 0);
  uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
  PX4.write(buf, len);
}

boolean takeoff() {
  mavlink_message_t msg;
  uint8_t buf[MAVLINK_MAX_PACKET_LEN];
  mavlink_msg_command_long_pack(system_id, component_id, &msg, target_system, target_component, MAV_CMD_NAV_TAKEOFF, confirmation, 0, 0, param3, param4, xi * 1e-7, yf * 1e-7, 3);
  uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
  PX4.write(buf, len);

}

void configureGimbal() {
  mavlink_message_t msg;
  uint8_t buf[MAVLINK_MAX_PACKET_LEN];
  mavlink_msg_command_long_pack(system_id, component_id, &msg, target_system, target_component, MAV_CMD_DO_MOUNT_CONFIGURE, confirmation, MAV_MOUNT_MODE_MAVLINK_TARGETING, 1, 1, 0, 0, 0, 0);
  uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
  //PX4.write(buf, len);
}

void stabilizeGimbal() {
  mavlink_message_t msg;
  uint8_t buf[MAVLINK_MAX_PACKET_LEN];
  mavlink_msg_command_long_pack(system_id, component_id, &msg, target_system, target_component, MAV_CMD_DO_MOUNT_CONTROL, confirmation, pitch * PI * 1.5, -roll * PI * 0.9, 0, 0, 0, 0, MAV_MOUNT_MODE_MAVLINK_TARGETING);
  uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
  PX4.write(buf, len);
}

void processDataFromOnBoard() {

  /* en dataOnBoard[0] esta la codificacion de la informacion
    0 SENSORES
    1 IMAGEN (RAW14)
    2 ERROR
    3 LOG
    4 GET
    5 MAIL
  */
  switch (data_on_board[0]) {
    case 0: {
        datarecive_sensor();
        break;
      }
    case 1: {
#ifdef ConsolaMon
        Consola.println(data_on_board[1]);
#endif
        datarecive_image();
        break;
      }
    case 2: {
        datarecive_error();
        break;
      }
    case 3: {
        datarecive_log();
        break;
      }
    case 5: {
      datarecive_mailto();
      break;
    } 
  }
}

void datarecive_sensor() {
  int ii = 1;
  r = (data_on_board[ii] << 8) + data_on_board[ii + 1];
  ii = ii + 2;
  s = (data_on_board[ii] << 8) + data_on_board[ii + 1];
  ii = ii + 2;
  sensor_act[0] = (data_on_board[ii] & 0x01);
  sensor_act[1] = (data_on_board[ii] & 0x02);
  sensor_act[2] = (data_on_board[ii] & 0x04);
  sensor_act[3] = (data_on_board[ii] & 0x08);
  ii++;
  sensor_dist[0] = (data_on_board[ii] << 8) + data_on_board[ii + 1];
  ii = ii + 2;
  sensor_dist[1] = (data_on_board[ii] << 8) + data_on_board[ii + 1];
  ii = ii + 2;
  sensor_dist[2] = (data_on_board[ii] << 8) + data_on_board[ii + 1];
  ii = ii + 2;
  sensor_dist[3] = (data_on_board[ii] << 8) + data_on_board[ii + 1];

  ii++;
  tiempo_hora = data_on_board[ii];
  ii++;
  tiempo_minuto = data_on_board[ii];
  ii++;
  tiempo_segundo = data_on_board[ii];
  ii++;
  tiempo_AM = data_on_board[ii]; // si es 1 es AM
  ii++;
  tiempo_dia = data_on_board[ii];
  ii++;
  tiempo_mes = data_on_board[ii];
  ii++;
  tiempo_anio = data_on_board[ii];
  ii++;

}

void datarecive_image() {

  /* el data[1] va a traer el numero de columna, son 80 columnas (0 a 79) y 60 filas (0 a 59) , cada mensaje trae dos columnas
    entonces si llega el 78 se entiende completa la imagen y se debe actualizar la variable Celsius_thermal_image y enviar la imagen al usuario
    si es necesario */

  int fila = 0;
  int columna = data_on_board[1];

  checkImage(columna);

  // Hay que recorrer (casi) todo el vector data_on_board e ir guardando la data

  for (int idata = 2 ; idata  <  DATAONBOARDSIZE - DATANOVALID; idata = idata + 2) {

    thermal_image[fila][columna] = (data_on_board[idata] << 8) + data_on_board[idata + 1]; // cada dato ocupa dos bytes

    if (fila < image_x - 1) {
      fila = fila + 1;
    }
    else {
      fila = 0;
      if ( columna < data_on_board[1] + 1) {
        columna = columna + 1;
      }
    }
  }

  // actualizo la imagen Celsius si es la última transferencia
  if (columna == image_y - 1) {
    ActualizarImagen();
    picture_print();
  }
}


void datarecive_mailto(){

  for (int kChar = 0;kChar < MAILLENGTH; kChar++){
    to_address[kChar] = data_on_board[kChar + 1];
    to_name[kChar] = data_on_board[kChar + 1];
    #ifdef ConsolaMon
    Consola.print(to_name[kChar]);
    #endif
  }
  #ifdef ConsolaMon
  Consola.println();
  #endif  
}




void datarecive_error() {   //1:Resync; 2:SD; 3:Mavlink;
  switch (data_on_board[1]) {
    case 1: {
        memset(reset_onboard_error, '\0', 15);
        sprintf(reset_onboard_error, "Resync error");
        break;
      }
    case 2: {
        memset(reset_onboard_error, '\0', 15);
        sprintf(reset_onboard_error, "SD error");
        break;
      }
    case 3: {
        memset(reset_onboard_error, '\0', 15);
        sprintf(reset_onboard_error, "Mavlink error");
        break;
      }
  }
}

int cont_part_log = 0;
byte data_log[248];

void datarecive_log() {
  if (data_on_board[1] == cont_part_log) {
    for (int i = 3; i < sizeof(data_on_board); i++) {
      data_log[i - 3] = data_on_board[i];
    }
    cont_part_log++;
    if (data_on_board[2] <= (cont_part_log * 248)) {
      cont_part_log = 0;
      //última parte del log
    }
    //enviar la data por mail
    sendACK();
  } else {
    sendNACK();
    //faltó un msj
  }
}

void conmutarPin(int pin) {
  digitalWrite(pin, !digitalRead(pin));
}

void conmutarPinesCallback() {

#ifdef PX4def
  if (PX4AVAILABLE) {
    conmutarPin(pinAntenaViva);
    PX4AVAILABLE = false;
  }
#endif

#ifdef shield3Gdef
  if (SHIELD3G) {
    conmutarPin(pinShieldVivo);
    SHIELD3G = false;
  }
#endif
}

void chequearPin(int pin) {

  if (pin == pinAntenaViva) {
    bytesLeidosDeAntena++;
  }

  if (pin == pinShieldVivo) {
    bytesLeidosDeShield++;
  }

  if (bytesLeidosDeAntena > BYTESCONMUTAR) {
    bytesLeidosDeAntena = 0;
    conmutarPin(pinAntenaViva);
  }

  if (bytesLeidosDeShield > BYTESCONMUTAR) {
    bytesLeidosDeShield = 0;
    conmutarPin(pinShieldVivo);
  }
}

void checkImage(int columna) {


  if (columna == 0) {
    latitud_int_1E7_foto = latitud_int_1E7;
    longitud_int_1E7_foto = longitud_int_1E7;
  }

  if ((columna == 0) or (columna == 20) or (columna == 40) or (columna == 60)) {
    // es el primer mensaje
#ifdef ConsolaMon
    Consola.print(F("Inicio de transferencia de parte de foto, parte "));
    Consola.println((columna + 20) / 20);
#endif
    contadorColumna = 0;
    contadorColumna++;
    columnavieja = columna;
    IMG_ACK_STATUS = IMG_INDEFINIDO;
    return;
  }
  if ((((columna == 18) or (columna == 38) or (columna == 58) or (columna == 78))) and (bsendNACK)) {
    // el el ultimo mensaje de una parte y hubo un nack en algun lado
#ifdef ConsolaMon
    Consola.print(F("Se solicita reenvio de la parte "));
    Consola.println((columna - 18 + 20) / 20);
#endif
    contadorColumna = 0;
    contadorColumna++;
    columnavieja = columna;
    IMG_ACK_STATUS = IMG_INDEFINIDO;
    sendNACK();
    IMG_ACK_STATUS = IMG_INDEFINIDO;
    bsendNACK = true;
    return;
  }
  else { // no es el primer mensaje
    if (columna - columnavieja == 2) {
      contadorColumna++;
      columnavieja = columna;
      IMG_ACK_STATUS = IMG_INDEFINIDO;
      if (contadorColumna == 10) {
        sendACK();
        sendACK();
#ifdef ConsolaMon
        Consola.println(F("Se recibieron 10 datos bien"));
#endif
      }
      return;
    }
    else { //falto un mensaje
      sendNACK();
      bsendNACK = false;
      contadorColumna = 0;
#ifdef ConsolaMon
      Consola.println(F("Falto una columna en el envio de la imagen desde la on-board"));
      Consola.println(F("Columna: "));
      Consola.println(columna);
      Consola.println(F("Columna vieja: "));
      Consola.println(columnavieja);
#endif
      return;
    }
  }
}

void sendACK() {
  IMG_ACK_STATUS = IMG_ACK;
  sendFlagsToOnBoard();
}

void sendNACK() {
  IMG_ACK_STATUS = IMG_NACK;
  sendFlagsToOnBoard();
}



void sendFlagsToOnBoard() {
  actualizarFlags();
  mavlink_message_t msg;
  uint8_t buf[MAVLINK_MAX_PACKET_LEN];
#ifdef ConsolaMon
  Consola.println(F("Datos enviados a la on-board"));
#endif
  mavlink_msg_file_transfer_protocol_pack(system_id, component_id, &msg, 0, 0, 0, dataToOnBoard);
  uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
  PX4.write(buf, len);
}

void actualizarFlags() {

  dataToOnBoard[0] = IMG_ACK_STATUS;
  dataToOnBoard[1] = FOTO_REQUEST;
  dataToOnBoard[2] = TIPO_MISION;
  dataToOnBoard[3] = LOG_ENABLE;

}

void picture_print() {
  for (int i = 0; i < 60; i++) {
    for (int j = 0; j < 80; j++) {
      Consola.print((int)thermal_image[i][j], DEC);
      Consola.print("\t");
    }
    Consola.print("\n");
  }
}


