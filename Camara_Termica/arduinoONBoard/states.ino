double aux_x, aux_y; //auxiliares para cálculo del nuevo wp
int detour = 5;
int huso = 21;
long int wait_time = 100000;
long int time_state; //tiempo desde que entre en un estado
int state;

void states(void) {

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
  if(seq == 1){
    state = 1;
  }
}


//state 1, busca un cuerpo caliente y releva sensores de distancia
void state_1(void){
  take_picture();
  search_body();
  sensors_read();
  if (r > 0){
    state = 2;
    //agrega el punto que corresponde
    set_wp_body();
    //funcion MAVLink para enviar el newwp
    send_wp(); 
    time_state = millis();
  }else if(sensor_act[2]  && sensor_act[3]){
    //si sensor frontal y derecha activados
    state = 5;
    set_wp_left();
    //funcion MAVLink para enviar el newwp 
    send_wp();
    wait_turn(90);
    time_state = millis();
  }else if(sensor_act[2]  && sensor_act[1]){
    //si sensor frontal e izquierda activados
    state = 4;
    set_wp_rigth();
    //funcion MAVLink para enviar el newwp
    send_wp();
    wait_turn(-90);
    time_state = millis();
  }else if(sensor_act[2]){
    //si solo sensor frontal activado
    state = 3;
    set_wp_rigth();
    //funcion MAVLink para enviar el newwp
    send_wp();
    wait_turn(-90);
    time_state = millis();
  }else if(seq == (count+1)){
    state = 6;
    new_wp.lat = home_wp.lat;
    new_wp.lon = home_wp.lon;
    new_wp.alt = actual_wp.alt;
    //funcion MAVLink para enviar el newwp 
    send_wp();
  }
  
}

//state 2, seguimiento de cuerpo
void state_2(void){
  if (time_state > wait_time){
    take_picture();
    search_body();
    time_state = millis() - time_state;
    if (r > 0){
      //agregar el punto que corresponde
      set_wp_body();
      //funcion MAVLink para enviar el newwp
      send_wp();
    }else {
      state = 1;
      //recuperar mision
      recover_mission();
    }
  }
}

//state 3, giro a la derecha sin saber si se puede
void state_3(void){
  if (time_state > wait_time){
    sensors_read();
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
    }else{
      state = 1;
      //recuperar mision
      recover_mission();
    }
  }
   
}

//state 4, giro a la derecha por ser lo único posible
void state_4(void){
  if (time_state > wait_time){
    sensors_read();
    time_state = millis() - time_state; 
    if(sensor_act[1]){
      //si solo sensor izquierda activado
      set_wp_front();
      //funcion MAVLink para enviar el newwp
      send_wp(); 
    }else{
      state = 1;
      //recuperar mision
      recover_mission();
    }
  }
   
}

//giro a la izquierda por ser lo único posible
void state_5(void){
  if (time_state > wait_time){
    sensors_read();
    time_state = millis() - time_state; 
    if(sensor_act[3]){
      //si solo sensor derecha activado
      set_wp_front();
      //funcion MAVLink para enviar el newwp
      send_wp(); 
    }else{
      state = 1;
      //recuperar mision
      recover_mission();
    }
  }
   
}

//aterrizaje
void state_landing(void){
if (seq == (count+1)){
    take_picture();
    search_body();
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
    if(sensor_dist[0] < 20){
      //apagar todos los motores;
      turn_off_motors();
    }  
}
}


void set_wp_body(void){

        wgs84_utm(actual_wp.lat, actual_wp.lon, &aux_x ,&aux_y);
        aux_x = aux_x + sqrt(sq(D.a - 40) + sq(D.b - 30))*actual_wp.alt*0.011*sin(actual_yaw - atan((D.a - 40)/(D.b - 30)));
        aux_y = aux_y + sqrt(sq(D.a - 40) + sq(D.b - 30))*actual_wp.alt*0.011*cos(actual_yaw - atan((D.a - 40)/(D.b - 30)));
        utm_wgs84(aux_y, aux_x, huso,&new_wp.lat ,&new_wp.lon);
        new_wp.alt = actual_wp.alt;
}

void set_wp_left(void){

    wgs84_utm(actual_wp.lat, actual_wp.lon, &aux_y ,&aux_x);
    if (actual_yaw < 90){
      aux_x = aux_x - detour*cos(actual_yaw); 
      aux_y = aux_y - detour*sin(actual_yaw);
    }else if (actual_yaw < 180){
      aux_x = aux_x + detour*sin(actual_yaw-90); 
      aux_y = aux_y - detour*cos(actual_yaw-90);
    }else if (actual_yaw < 270){
      aux_x = aux_x + detour*cos(actual_yaw-180); 
      aux_y = aux_y + detour*sin(actual_yaw-180);
    }else{
      aux_x = aux_x - detour*sin(actual_yaw-270); 
      aux_y = aux_y + detour*cos(actual_yaw-270);
    }
    utm_wgs84(aux_y, aux_x, huso, &new_wp.lat, &new_wp.lon);
    new_wp.alt = actual_wp.alt;
}

void set_wp_rigth(void){

    wgs84_utm(actual_wp.lat, actual_wp.lon, &aux_y ,&aux_x);
    if (actual_yaw < 90){
      aux_x = aux_x + detour*cos(actual_yaw); 
      aux_y = aux_y + detour*sin(actual_yaw);
    }else if (actual_yaw < 180){
      aux_x = aux_x - detour*sin(actual_yaw-90); 
      aux_y = aux_y + detour*cos(actual_yaw-90);
    }else if (actual_yaw < 270){
      aux_x = aux_x - detour*cos(actual_yaw-180); 
      aux_y = aux_y - detour*sin(actual_yaw-180);
    }else{
      aux_x = aux_x + detour*sin(actual_yaw-270); 
      aux_y = aux_y - detour*cos(actual_yaw-270);
    }
    utm_wgs84(aux_y, aux_x, huso, &new_wp.lat, &new_wp.lon);
    new_wp.alt = actual_wp.alt;
}

void set_wp_front(void){ 

    wgs84_utm(actual_wp.lat, actual_wp.lon, &aux_y ,&aux_x);
    if (actual_yaw < 90){
      aux_x = aux_x + detour*sin(actual_yaw); 
      aux_y = aux_y - detour*cos(actual_yaw);
    }else if (actual_yaw < 180){
      aux_x = aux_x + detour*cos(actual_yaw-90); 
      aux_y = aux_y + detour*sin(actual_yaw-90);
    }else if (actual_yaw < 270){
      aux_x = aux_x - detour*sin(actual_yaw-180); 
      aux_y = aux_y + detour*cos(actual_yaw-180);
    }else{
      aux_x = aux_x - detour*cos(actual_yaw-270); 
      aux_y = aux_y - detour*sin(actual_yaw-270);
    }
    utm_wgs84(aux_y, aux_x, huso, &new_wp.lat ,&new_wp.lon);
    new_wp.alt = actual_wp.alt;
}

void set_wp_back(void){ 

    wgs84_utm(actual_wp.lat, actual_wp.lon, &aux_y ,&aux_x);
    if (actual_yaw < 90){
      aux_x = aux_x - 2*detour*sin(actual_yaw); 
      aux_y = aux_y + 2*detour*cos(actual_yaw);
    }else if (actual_yaw < 180){
      aux_x = aux_x - detour*cos(actual_yaw-90); 
      aux_y = aux_y - detour*sin(actual_yaw-90);
    }else if (actual_yaw < 270){
      aux_x = aux_x + detour*sin(actual_yaw-180); 
      aux_y = aux_y - detour*cos(actual_yaw-180);
    }else{
      aux_x = aux_x + detour*cos(actual_yaw-270); 
      aux_y = aux_y + detour*sin(actual_yaw-270);
    }
    utm_wgs84(aux_y, aux_x, huso, &new_wp.lat ,&new_wp.lon);
    new_wp.alt = actual_wp.alt;
}

void bat_check(){
  if(bat < 25){
    state = 6;
    new_wp.lat = home_wp.lat;
    new_wp.lon = home_wp.lon;
    new_wp.alt = actual_wp.alt;
    //funcion MAVLink para enviar el newwp 
    send_wp();
  }
}

