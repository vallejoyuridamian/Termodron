int huso = 21;
long int time_state; //tiempo desde que ingresó en un estado
bool rel_sensors = true;
double aux_x, aux_y; //auxiliares para cálculo del nuevo wp
double pi_sobre_180 = 0.0174532925199432957692;

void states(void) {
  sermon.print(F("Estado:"));
  sermon.println(state);
  switch (state) {
      case 0:
        state_0();
        break;
      case 1:
        state_1();
        break;
      case 2:
        state_2();
        break;
      case 3:
        state_3();
        break;
      case 4:
        state_4();
        break;     
      case 5:
        state_5();
        break;
      case 6:
        state_landing();
        break;
      default: 
      
      break;
  }
}



//state 0, esperando a que despegue y se mueva a takeoff
void state_0(void){
  if(logsendenable == 1){
    flagdata2GCS = SEND2GCS_LOG;
    senddatatoGCS_task.enable();
  }
  /*if((!flags_received)&&(INPUT_STATUS == INPUT_STATUS_MISSION)){
    flagdata2GCS = SEND2GCS_GET;
    senddatatoGCS_task.enable();
  }
  if((mission_received == 0)&&(INPUT_STATUS == INPUT_STATUS_MISSION)){
    request_all_mission();
    seq = 0;
    mission_received = 1;
  }*/
  if((seq > 0)||(Pixhawk_landed_state == MAV_LANDED_STATE_IN_AIR)){
    if(mission_received == 0){
      dsipart = 0;
      request_all_mission();
      mission_received = 1;
    }
    state = 1;
  }
}


//state 1, busca un cuerpo caliente y releva sensores de distancia
void state_1(void){
  if (rel_sensors){
    
    take_picture();             //Se toma una foto
    //memoriadisp();
    search_body();              //Se busca un cuerpo
    #ifdef SD_present
    SD_picture_save();          //Se guarda en la memoria SD
    #endif           

    //sensors_read();             //Se relevan los sensores
    datasend_sensor();          //Se envía a la GCS
    rel_sensors = false;
  }else{
    if (r > 0){
      /*if((sendimage == 1)&(dsipart==0)){                //Se sacó para la DEMO
        memcpy(Celsius_thermal_image, image, sizeof(image));
        flagdata2GCS == SEND2GCS_IMAGE;
        senddatatoGCS_task.enable();    //Se envía a la GCS  
      }*/
      if (mission_mode == 2){       //Cont
         //No hace nada, sigue la mision
          time_state = millis();
      }else{
          state = 2;
          set_wp_body();//agrega el punto que corresponde
          auxseq = seq;
          #ifdef debugging
          sermon.println(F("Cuerpo detectado, enviando posicion."));
          #endif
          send_wp(); //funcion MAVLink para enviar el newwp
          time_state = millis();
      }
    }else if(sensor_act[2]  && sensor_act[3]){
      //si sensor frontal y derecha activados
      state = 5;
      set_wp_left();
      auxseq = seq; 
      #ifdef debugging
      sermon.println(F("Sensores frontal y derecho activados."));
      #endif
      send_wp();    //funcion MAVLink para enviar el newwp
      wait_turn(90);
      time_state = millis();
    }else if(sensor_act[2]  && sensor_act[1]){
      //si sensor frontal e izquierda activados
      state = 4;
      set_wp_rigth();
      auxseq = seq;
      #ifdef debugging
      sermon.println(F("Sensores frontal e izquierdo activados."));
      #endif
      send_wp();  //funcion MAVLink para enviar el newwp
      wait_turn(-90);
      time_state = millis();
    }else if(sensor_act[2]){
      //si solo sensor frontal activado
      state = 3;
      set_wp_rigth();
      auxseq = seq;
      #ifdef debugging
      sermon.println(F("Sensor frontal activado."));
      #endif
      send_wp();  //funcion MAVLink para enviar el newwp
      wait_turn(-90);
      time_state = millis();
    }else if(seq == (count-1)){
      state = 6;
      if(seq != 0){
        new_wp.lat = home_wp.lat;
        new_wp.lon = home_wp.lon;
        new_wp.alt = actual_wp.alt;
        auxseq = seq;
        count = 1;
        send_wp();
      }
      home_reached = false;
      #ifdef debugging
      sermon.println(F("Volviendo a Home."));
      #endif
      /*new_wp.lat = home_wp.lat;
      new_wp.lon = home_wp.lon;
      new_wp.alt = actual_wp.alt;
      auxseq = seq;
      count = 1;
      send_wp();*/      //funcion MAVLink para enviar el newwp
    }
    rel_sensors = true;
  }  
}


//state 2, seguimiento de cuerpo
void state_2(void){
  if ((millis() - time_state) > wait_time){
    if (mission_mode == 1){       //RTL
        state = 6;
        new_wp.lat = home_wp.lat;
        new_wp.lon = home_wp.lon;
        new_wp.alt = actual_wp.alt;
        //funcion MAVLink para enviar el newwp 
        send_wp();
        #ifdef debugging
          sermon.println(F("RTL"));
        #endif
    }else {                        //Follow
        take_picture();
        search_body();
        #ifdef SD_present
        SD_picture_save();
        #endif
        if((sendimage == 1)&(dsipart==0)){
          datasend_sensor();          //Se envía a la GCS
          memcpy(Celsius_thermal_image, image, sizeof(image));
          flagdata2GCS = SEND2GCS_IMAGE;
          senddatatoGCS_task.enable();      
        }
        time_state = millis() - time_state;
        if (r > 0){
          //agregar el punto que corresponde
          set_wp_body();
          //funcion MAVLink para enviar el newwp
          send_wp();
          #ifdef debugging
            sermon.print(F("Follow agrega waypoint por r:"));
            sermon.println(r);
            sermon.println(s);
          #endif
        /*}else if(seq == (count-1)){
            state = 6;
            new_wp.lat = home_wp.lat;
            new_wp.lon = home_wp.lon;
            new_wp.alt = actual_wp.alt;
            //funcion MAVLink para enviar el newwp
            auxseq = seq;
            #ifdef debugging
            sermon.println(F("Volviendo a Home."));
            #endif
            send_wp();*/
         }else{
            state = 1;
            #ifdef debugging
              sermon.println(F("Follow recupera mision"));
            #endif
            //recuperar mision
            recover_mission();
         }
    }
  }
}

//state 3, giro a la derecha sin saber si se puede
void state_3(void){
  if ((millis() - time_state) > wait_time){
    //sensors_read();
    datasend_sensor();
    time_state = millis() - time_state; 
    if(sensor_act[2] && sensor_act[1]){
      //si sensor frontal e izquierda activados
      state = 5;
      set_wp_back();
      //funcion MAVLink para enviar el newwp
      send_wp(); 
    }else if(sensor_act[1]){
      //si solo sensor izquierda activado
      set_wp_front();
      //funcion MAVLink para enviar el newwp
      send_wp(); 
    }else {
      state = 1;
      //recuperar mision
      recover_mission();
    }
  }   
}

//state 4, giro a la derecha por ser lo único posible
void state_4(void){
  if ((millis() - time_state) > wait_time){
    //sensors_read();
    datasend_sensor();
    time_state = millis() - time_state; 
    if(sensor_act[1]){
      //si solo sensor izquierda activado
      set_wp_front();
      //funcion MAVLink para enviar el newwp
      send_wp(); 
    }else {
      state = 1;
      //recuperar mision
      recover_mission();
    }
  }
   
}

//giro a la izquierda por ser lo único posible
void state_5(void){
  if ((millis() - time_state) > wait_time){
    //sensors_read();
    datasend_sensor();
    time_state = millis() - time_state; 
    if(sensor_act[3]){
      //si solo sensor derecha activado
      set_wp_front();
      //funcion MAVLink para enviar el newwp
      send_wp(); 
    }else {
      state = 1;
      //recuperar mision
      recover_mission();
    }
  }
   
}

//aterrizaje
void state_landing(void){
  if (home_reached || (seq == 0)){
    land();
  }
  //if (seq == (count-1)){
    /*take_picture();
    search_body();
    sensors_read();
    #ifdef SD_present
    SD_picture_save();
    #endif
    //datasend_image();
    if (r > 0){
      //agregar el punto que corresponde
      set_wp_body();
    }else{
      new_wp.lat = actual_wp.lat;
      new_wp.lon = actual_wp.lon;
    }
    if(sensor_act[0]){
      new_wp.alt = sensor_dist[0]/2;
      //funcion MAVLink para enviar el newwp
      send_wp(); 
    }else{
      new_wp.alt = actual_wp.alt/2;
      //funcion MAVLink para enviar el newwp
      send_wp(); 
    }
    if((sensor_dist[0] < 3)||(actual_wp.alt < 3)){
      land();
    }*/
    if(Pixhawk_landed_state == MAV_LANDED_STATE_ON_GROUND){
      disarm();
      seq = 0;
      state = 0;
      mission_received = 0;
    }
}

void bat_check(){
  if(bat < bat_to_home && state != 6 && bat > 0){
    state = 6;
    new_wp.lat = home_wp.lat;
    new_wp.lon = home_wp.lon;
    new_wp.alt = actual_wp.alt;
    //funcion MAVLink para enviar el newwp 
    send_wp();
  }
  if(bat < bat_to_land && state != 6 && bat > 0){
    state = 6;
    new_wp.lat = actual_wp.lat;
    new_wp.lon = actual_wp.lon;
    new_wp.alt = actual_wp.alt;
    //funcion MAVLink para enviar el newwp 
    send_wp();
  }
  
}

//Función para calcular un nuevo waypoint sobre el cuerpo caliente
void set_wp_body(void){
  wgs84_utm(actual_wp.lat, actual_wp.lon, &aux_x ,&aux_y);
  aux_x = aux_x + actual_wp.alt*0.011*((D.b - 40)*cos(pi_sobre_180*actual_yaw) + (30 - D.a)*sin(pi_sobre_180*actual_yaw));
  aux_y = aux_y + actual_wp.alt*0.011*((30 - D.a)*cos(pi_sobre_180*actual_yaw) - (D.b - 40)*sin(pi_sobre_180*actual_yaw));
  utm_wgs84(aux_x, aux_y, huso,&new_wp.lat ,&new_wp.lon);
  new_wp.alt = actual_wp.alt;
}

void set_wp_left(void){
  wgs84_utm(actual_wp.lat, actual_wp.lon, &aux_y ,&aux_x);
  aux_x = aux_x - detour*cos(pi_sobre_180*actual_yaw); 
  aux_y = aux_y + detour*sin(pi_sobre_180*actual_yaw);
  utm_wgs84(aux_y, aux_x, huso, &new_wp.lat, &new_wp.lon);
  new_wp.alt = actual_wp.alt;
}

void set_wp_rigth(void){
  wgs84_utm(actual_wp.lat, actual_wp.lon, &aux_y ,&aux_x);
  aux_x = aux_x + detour*cos(pi_sobre_180*actual_yaw); 
  aux_y = aux_y - detour*sin(pi_sobre_180*actual_yaw);
  utm_wgs84(aux_y, aux_x, huso, &new_wp.lat, &new_wp.lon);
  new_wp.alt = actual_wp.alt;
}

void set_wp_front(void){ 
  wgs84_utm(actual_wp.lat, actual_wp.lon, &aux_y ,&aux_x);
  aux_x = aux_x + detour*sin(pi_sobre_180*actual_yaw); 
  aux_y = aux_y + detour*cos(pi_sobre_180*actual_yaw);
  utm_wgs84(aux_y, aux_x, huso, &new_wp.lat ,&new_wp.lon);
  new_wp.alt = actual_wp.alt;
}

void set_wp_back(void){ 
  wgs84_utm(actual_wp.lat, actual_wp.lon, &aux_y ,&aux_x);
  aux_x = aux_x - 2*detour*sin(pi_sobre_180*actual_yaw); 
  aux_y = aux_y - 2*detour*cos(pi_sobre_180*actual_yaw);
  utm_wgs84(aux_y, aux_x, huso, &new_wp.lat ,&new_wp.lon);
  new_wp.alt = actual_wp.alt;
}



////////////////////////////////AUX Functions////////////////////////////////

void wait_turn(int giro){
  bool timewaitaux = false;
  uint16_t aux_yaw;
  while(!timewaitaux){
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
