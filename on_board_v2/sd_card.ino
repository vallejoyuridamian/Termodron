#ifdef SD_present
//Función de inicialización de la tarjeta SD
void SD_init(){
  pinMode(csSD, OUTPUT);
  digitalWrite(csSD, HIGH);  
//  SPI.begin(csSD);
//  SPI.setDataMode(csSD, SPI_MODE0);
//  SPI.setBitOrder(csSD, MSBFIRST);
//  SPI.setClockDivider(csSD, 128);
  if (!SD.begin(20000000, csSD)){
    #ifdef debugging
      sermon.println(F("Se ha producido un fallo modulo_SD, RESET BOARD"));
    #endif
    datasend_error(2);
    reset_onboard();
  }else{
    #ifdef debugging
      sermon.println(F("Modulo SD inicializado correctamente"));
    #endif
  }
  if(SD.exists("/b")){
    if(SD.rmdir("/b")){
      #ifdef debugging
      sermon.println(F("Imagenes borradas"));
      #endif
    }
  }
}

//Función para salvar una imagen en la tarjeta SD en formato bmp
void SD_picture_save(){
    if(!SD.exists("/b")){
      SD.mkdir("/b");
    }
    sprintf(image_file, "/b/ima%d.bmp", image_number);
    if(SD.exists(image_file)){
      SD.remove(image_file);
    }
    #ifdef debugging
      sermon.println(image_file);
    #endif
    myFile = SD.open(image_file, FILE_WRITE);//abrimos  el archivo
    if (myFile) {
      #ifdef debugging
        sermon.println(F("Escribiendo SD: "));
      #endif
      create_bmp(myFile);
      myFile.close(); //cerramos el archivo
      
      myFile = SD.open(log_file, FILE_WRITE);//abrir log
      if (myFile) {  
        myFile.print(F("Se guardo imagen:"));
        myFile.println(image_file);
        myFile.close();
      }else{
        error_SDfile(log_file);
      }
      
      image_number = image_number + 1;
    }else{
      error_SDfile(image_file);
    }
    #ifdef debugging
     sermon.println(F("Guardada"));
    #endif
}

void create_bmp(File &bmpFile) {
    byte file[14] = {
        'B','M', // magic
        0,0,0,0, // size in bytes
        0,0, // app data
        0,0, // app data
        40+14,0,0,0 // start of data offset
    };
    byte info[40] = {
        40,0,0,0, // info hd size
        0,0,0,0, // width
        0,0,0,0, // heigth
        1,0, // number color planes
        24,0, // bits per pixel
        0,0,0,0, // compression is none
        0,0,0,0, // image bits size
        0x13,0x0B,0,0, // horz resoluition in pixel / m
        0x13,0x0B,0,0, // vert resolutions (0x03C3 = 96 dpi, 0x0B13 = 72 dpi)
        0,0,0,0, // #colors in pallete
        0,0,0,0, // #important colors
    };

    uint32_t padSize  = (4-(image_y*3)%4)%4;
    //sermon.println(padSize);
    uint32_t sizeData = image_y * image_x * 3 + image_x * padSize;
    uint32_t sizeAll  = sizeData + sizeof(file) + sizeof(info);

    file[ 2] = (byte)((sizeAll      ) & 0xFF);
    file[ 3] = (byte)((sizeAll >>  8) & 0xFF);
    file[ 4] = (byte)((sizeAll >> 16) & 0xFF);
    file[ 5] = (byte)((sizeAll >> 24) & 0xFF);
    info[ 4] = (byte)((image_y      ) & 0xFF);
    info[ 5] = (byte)((image_y >>  8) & 0xFF);
    info[ 6] = (byte)((image_y >> 16) & 0xFF);
    info[ 7] = (byte)((image_y >> 24) & 0xFF);
    info[ 8] = (byte)((image_x      ) & 0xFF);
    info[ 9] = (byte)((image_x >>  8) & 0xFF);
    info[10] = (byte)((image_x >> 16) & 0xFF);
    info[11] = (byte)((image_x >> 24) & 0xFF);
    info[20] = (byte)((sizeData      ) & 0xFF);
    info[21] = (byte)((sizeData >>  8) & 0xFF);
    info[22] = (byte)((sizeData >> 16) & 0xFF);
    info[23] = (byte)((sizeData >> 24) & 0xFF);

    bmpFile.write((byte *)file, sizeof(file));
    bmpFile.write((byte *)info, sizeof(info));

    byte pad[3] = {0,0,0};

    for (int x = image_x - 1; x >= 0; --x) {
        for (int y = 0; y < image_y; ++y) {
            byte pixel[3]; // blue green red
            int temp = 0.053*(image[x][y]-8192)+25;     //0.0327*(image[x][y]) + 27.637*cam_temp - 885.39;
            pixel[0] = calcularAzul(temp);
            pixel[1] = calcularVerde(temp);
            pixel[2] = calcularRojo(temp);
            bmpFile.write((byte *)pixel, 3);
        }
        bmpFile.write((byte *)pad, padSize);
    }
}



//Función para crear un arhivo log en la tarjeta SD
void sd_log_init(void){
  if(SD.exists(log_file)){
    SD.remove(log_file);
  }
  myFile = SD.open(log_file, FILE_WRITE);
  sermon.println("acá si llega");
  if (myFile) {
    sermon.println("acá también llega");
    myFile.println(F("---------Drone Information---------"));
    myFile.print(F("System type:"));
    myFile.println(system_type);
    myFile.print(F("System id:"));
    myFile.println(system_id);
    myFile.print(F("Autopilot type:"));
    myFile.println(autopilot_type);
    myFile.print(F("Frame:"));
    myFile.println(frame);
    /*myFile.println(F("---------Flir Lepton Information---------"));
    char buffer[80];
    sys_getCustomerSerialNumber(buffer, 80);
    myFile.print(F("Customer Serial Number:"));
    myFile.println(buffer);
    sys_getFlirSerialNumber(buffer, 80);
    myFile.print(F("Flir Serial Number:"));
    myFile.println(buffer);
    myFile.print(F("Camera booted:"));
    myFile.println(cameraBooted());
    myFile.print(F("Camera Status:"));
    myFile.println(sys_getCameraStatus());
    myFile.print(F("AGC Enable:"));
    myFile.println(agc_getAGCEnabled());
    myFile.print(F("AGC Policy:"));
    myFile.println(agc_getAGCPolicy());
    myFile.print(F("Camera Uptime:"));
    myFile.println(sys_getCameraUptime());
    myFile.print(F("Aux Temperature:"));
    cam_temp = sys_getAuxTemperature();
    myFile.println(cam_temp);
    myFile.print(F("FPA Temperature:"));
    fpa_temp = sys_getFPATemperature();
    myFile.println(fpa_temp);
    myFile.print(F("Thermal Shutdown Count:"));
    myFile.println(sys_getThermalShutdownCount());
    myFile.print(F("FFC Normalization Status:"));
    myFile.println(sys_getFFCNormalizationStatus());*/

    myFile.close();
  }else{
    error_SDfile(log_file);
  }
}


//Función para guardar coordenadas de los puntos de la mision en el arhivo log en la tarjeta SD
void sd_mission_save(void){
  myFile = SD.open(log_file, FILE_WRITE);
  if (myFile) {  
    myFile.println(F("---------Mission Information---------"));
    myFile.print(F("Home WP:"));
    myFile.print("\t");
    myFile.print(F("lat:"));
    printDouble(home_wp.lat,6);
    myFile.print("\t");
    myFile.print(F("lon:"));
    printDouble(home_wp.lon,6);
    myFile.print("\t");
    myFile.print(F("alt:"));
    printDouble(home_wp.alt,6);
    myFile.print("\n");
    myFile.print(F("Mission count:"));
    myFile.println(count);
    #ifdef waypoint_int
    for(int h = 0; h<count;h++){
      myFile.print(F("WP:"));
      myFile.print(h);
      myFile.print("\t");
      myFile.print(F("lat:"));
      printDouble(mission_wpsint[h].latint,6);
      myFile.print("\t");
      myFile.print(F("lon:"));
      printDouble(mission_wpsint[h].lonint,6);
      myFile.print("\t");
      myFile.print(F("alt:"));
      printDouble(mission_wpsint[h].altint,6);
      myFile.print("\n");
    }
    #else
    for(int h = 0; h<count;h++){
      myFile.print(F("WP:"));
      myFile.print(h);
      myFile.print("\t");
      myFile.print(F("lat:"));
      printDouble(mission_wps[h].lat,6);
      myFile.print("\t");
      myFile.print(F("lon:"));
      printDouble(mission_wps[h].lon,6);
      myFile.print("\t");
      myFile.print(F("alt:"));
      printDouble(mission_wps[h].alt,6);
      myFile.print("\n");
    }
    #endif
    myFile.close();
  }else{
    error_SDfile(log_file);
  }
}


//Función para salvar la hora de los procesos iniciales de Pixhawk
void SD_timePX4_save(){
  myFile = SD.open(log_file, FILE_WRITE);
  if (myFile) {
    myFile.println(F("---------PX4 Time Information---------"));
    myFile.print(F("Unix time:"));
    print_date(time_unix_usec);
    myFile.print(F("Boot time:"));
    print_date(boot_time_us);
    myFile.print(F("Arming time:"));
    print_date(arming_time_utc);
    myFile.print(F("Takeoff time:"));
    print_date(takeoff_time_utc); 
    myFile.close();
  }else{
    error_SDfile(log_file);
  }
}






//Función para actualizar el arhivo log en la tarjeta SD
void sd_log_save(){
    myFile = SD.open(log_file, FILE_WRITE);
    if (myFile) {
      myFile.println(F("--------------------Loop Update--------------------"));
      actual_time_us = time_unix_usec + ((millis()-time_counter)*1000);
      myFile.print(F("Actual time:"));
      print_date(actual_time_us);    

      myFile.print(F("On board state:"));
      myFile.println(state);

      myFile.print(F("PX4 state:"));
      if(PX4_STATUS==MAV_STATE_ACTIVE){
        myFile.print(F("ACTIVO; "));
      }else if(PX4_STATUS==MAV_STATE_STANDBY){
        myFile.print(F("STANDBY; "));           
      }
      if(ARMED_STATUS == ARMED_STATUS_DISARMED){
        myFile.print(F("DESARMADO; "));
      }else if(ARMED_STATUS == ARMED_STATUS_ARMED){
        myFile.print(F("ARMADO; "));          
      }else if(ARMED_STATUS == ARMED_STATUS_UNKNOWN){
        myFile.print(F("DESCONOCIDO; "));         
      }
      
      if (INPUT_STATUS == INPUT_STATUS_MISSION){
        myFile.print(F("AUTONOMO;"));
      }else if (INPUT_STATUS == INPUT_STATUS_MANUAL){
        myFile.print(F("MANUAL;"));
      }else if (INPUT_STATUS == INPUT_STATUS_UNKNOWN){
        myFile.print(F("DESCONOCIDO;"));
      }

      if (Pixhawk_landed_state == MAV_LANDED_STATE_IN_AIR){
        myFile.print(F("AIRE"));
      }else if (Pixhawk_landed_state == MAV_LANDED_STATE_ON_GROUND){
        myFile.print(F("TIERRA"));
      }else if (Pixhawk_landed_state == MAV_LANDED_STATE_UNDEFINED){
        myFile.print(F("INDEFINIDO"));
      }
      myFile.println();

      myFile.print(F("Sensor left (m):"));
      myFile.println(sensor_dist[1], DEC);
      myFile.print(F("Sensor front (m):"));
      myFile.println(sensor_dist[2], DEC);
      myFile.print(F("Sensor rigth (m):"));
      myFile.println(sensor_dist[3], DEC);
      myFile.print(F("Sensor down (m):"));
      myFile.println(sensor_dist[0], DEC);
      myFile.print(F("Number of objects:"));
      myFile.print(r, DEC);
      myFile.print("\t");
      myFile.print(F("Pixels in the biggest object:"));
      myFile.println(s, DEC);
      if(r != 0){
        myFile.print(F("New WP:"));
        myFile.print("\t");
        myFile.print(F("lat:"));
        myFile.print(new_wp.lat, DEC);
        myFile.print("\t");
        myFile.print(F("lon:"));
        myFile.print(new_wp.lon, DEC);
        myFile.print("\t");
        myFile.print(F("alt:"));
        myFile.println(new_wp.alt, DEC);
      }
           
      myFile.print(F("Actual pos:"));
      myFile.print("\t");
      myFile.print(F("lat:"));
      myFile.print(actual_wp.lat, DEC);
      myFile.print("\t");
      myFile.print(F("lon:"));
      myFile.print(actual_wp.lon, DEC);
      myFile.print("\t");
      myFile.print(F("alt:"));
      myFile.print(actual_wp.alt, DEC);
      myFile.print("\t");
      myFile.print(F("orientation:"));
      myFile.println(actual_yaw, DEC);

      myFile.print(F("Attitude:"));
      myFile.print("\t");
      myFile.print(F("roll (°):"));
      myFile.print(att_roll, DEC);
      myFile.print("\t");
      myFile.print(F("yaw (°):"));
      myFile.print(att_yaw, DEC);
      myFile.print("\t");
      myFile.print(F("pitch (°):"));
      myFile.print(att_pitch, DEC);
      myFile.print("\t");
      myFile.print(F("roll speed (°/s):"));
      myFile.print(att_roll_speed, DEC);
      myFile.print("\t");
      myFile.print(F("yaw speed (°/s):"));
      myFile.print(att_yaw_speed, DEC);
      myFile.print("\t");
      myFile.print(F("pitch speed (°/s):"));
      myFile.println(att_pitch_speed, DEC);

      myFile.print(F("Number of mission WP:"));
      myFile.print(count, DEC);
      myFile.print("\t");
      myFile.print(F("Number of current WP:"));
      myFile.println(seq, DEC);
      
      myFile.print(F("Battery remaining (%):"));
      myFile.println(bat, DEC);
      myFile.print(F("Battery temperature (°C):"));
      myFile.println(bat_temp, DEC);
      myFile.print(F("Battery current (mA):"));
      myFile.println(bat_curr, DEC);
      myFile.print(F("Battery voltage of cells (mV):"));
      myFile.print(bat_volt[0], DEC);
      myFile.print("\t");
      myFile.print(bat_volt[1], DEC);
      myFile.print("\t");
      myFile.println(bat_volt[2], DEC);
      //myFile.print(F("Battery consumed energy (J):"));
      //myFile.println(bat_ener, DEC);

      myFile.print(F("Presion absoluta (hPa):"));
      myFile.println(pressure_abs);
      myFile.print(F("Presion diferencial (hPa):"));
      myFile.println(pressure_diff);
      myFile.print(F("Temperatura PX4 (°C):"));
      myFile.println(temperature_px4);
      
      /*myFile.print(F("Camera Status:"));
      myFile.println(sys_getCameraStatus());
      myFile.print(F("Camera Uptime:"));
      myFile.println(sys_getCameraUptime());
      myFile.print(F("Aux Temperature:"));
      cam_temp = sys_getAuxTemperature();
      myFile.println(cam_temp);
      myFile.print(F("FPA Temperature:"));
      fpa_temp = sys_getFPATemperature();
      myFile.println(fpa_temp);
      myFile.print(F("Thermal Shutdown Count:"));
      myFile.println(sys_getThermalShutdownCount());*/
      
      myFile.close();
    }else{
      error_SDfile(log_file);
    }
}


      
////////////////////////////////AUX Functions////////////////////////////////

void printDouble( double val, byte precision){
// prints val with number of decimal places determine by precision
// precision is a number from 0 to 6 indicating the desired decimial places
  if(val < 0.0){
    myFile.print('-');
    val = -val;
  }
  myFile.print (int(val));  //prints the int part
   if(precision > 0) {
    myFile.print("."); // print the decimal point
    float frac;
    frac = (val - int(val));
    while(precision--){
      frac = frac*10; 
      myFile.print(int (frac));
      frac = frac - int(frac);
    }
  }
}


void print_date(uint64_t time_unix) {
  uint64_t utm_uy = 10800;
  time_unix = (time_unix/1E6) - utm_uy; //UTM-3
  int horario;
  horario = hour(time_unix);
  myFile.print(horario);
  myFile.print(":");
  horario = minute(time_unix);
  myFile.print(horario);
  myFile.print(":");
  horario = second(time_unix);
  myFile.print(horario);
  if(isAM(time_unix)){
     myFile.print(F("(AM)"));
  }else{
    myFile.print(F("(PM)"));
  }
  myFile.print("\t");
  switch(weekday(time_unix)){
    case 0:{
      myFile.print(F("(Sunday)"));
      break;
    }
    case 1:{
      myFile.print(F("(Monday)"));
      break;
    }
    case 2:{
      myFile.print(F("(Tuesday)"));
      break;
    }
    case 3:{
      myFile.print(F("(Wednesday)"));
      break;
    }
    case 4:{
      myFile.print(F("(Thursday)"));
      break;
    }
    case 5:{
      myFile.print(F("(Friday)"));
      break;
    }
    case 6:{
      myFile.print(F("(Saturday)"));
      break;
    }
    default:
    break;
  }
  horario = day(time_unix);
  myFile.print(horario);
  myFile.print(F("/"));
  horario = month(time_unix);
  myFile.print(horario);
  myFile.print(F("/"));
  horario = year(time_unix);
  myFile.println(horario);
}
#endif


//Función para calcular cantidad de rojo de un pixel
int calcularRojo(int temp){
  if (temp < tmin_bmp){
    return 0;
  }
  if (temp > tp_bmp){
    return 255;
  }
 return int(255*temp/(tp_bmp-tmin_bmp)-255*tp_bmp/(tp_bmp-tmin_bmp)+255);
} 

//Función para calcular cantidad de verde de un pixel
int calcularVerde(int temp){
  if (temp < tp_bmp){
    return 255;
  }
  if (temp > tmax_bmp){
    return 0;
  } 
  return int(255*temp/(tp_bmp-tmax_bmp)-255*tp_bmp/(tp_bmp-tmax_bmp)+255);
} 

//Función para calcular cantidad de azul de un pixel
int calcularAzul(int temp){
 return 0;
}




//Función para salvar una imagen .csv en la tarjeta SD
void SD_csv_save(){
    if(!SD.exists("/p")){
      SD.mkdir("/p");
    }
    sprintf(image_file, "/p/ima%d.csv", image_number);
    if(SD.exists(image_file)){
      SD.remove(image_file);
    }
    #ifdef debugging
      sermon.println(image_file);
    #endif
    myFile = SD.open(image_file, FILE_WRITE);//abrimos  el archivo
    if (myFile) {
      #ifdef debugging
        sermon.println(F("Escribiendo SD: "));
      #endif
      for(int i=0;i<60;i++){
        for(int j=0;j<80;j++){
          myFile.print((int)image[i][j], DEC);
          myFile.print(";");     
        }
        myFile.print("\n");
      }
      myFile.close(); //cerramos el archivo
      image_number = image_number + 1;
    }else{
      error_SDfile(image_file);
    }
}


//Funcion para actualizar el tamaño del archivo Log
void getLogsize(void){
  myFile = SD.open(log_file);
  if (myFile){
    #ifdef debugging
    sermon.println(F("Leyendo SD..."));
    #endif
    size_log = myFile.size();
    myFile.close();
  }else {
    error_SDfile(log_file);
  }
}


//Funcio para copiar archivo a otra carpeta
void copy2folder(char* name_file, char* name_folder) {
  File myFileIn;
  File myFileOut;
  if (!SD.exists(name_folder)) {
    SD.mkdir(name_folder);
  }
  sprintf(aux_file, "/%d/%s", name_folder,name_file);
  if (SD.exists(aux_file)) {
    SD.remove(aux_file);
  }
  myFileIn = SD.open(name_file, FILE_READ);
  myFileOut = SD.open(aux_file, FILE_WRITE);
  while (myFileIn.available()) {
    myFileOut.write(myFileIn.read());
  }
  myFileIn.close();
  myFileOut.close();
}

