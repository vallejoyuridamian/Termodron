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
      sermon.println("Se ha producido un fallo modulo_SD, RESET BOARD");
    #endif
    datasend_error(2);
    reset_onboard();
  }else{
    #ifdef debugging
      sermon.println("modulo_SD OK");
    #endif
  }
  if(SD.exists("/b")){
    SD.remove("/b");
  }
}

//Función para salvar una imagen en la tarjeta SD
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
        sermon.println("Escribiendo SD: ");
      #endif
      create_bmp(myFile);
      myFile.close(); //cerramos el archivo
      
      myFile = SD.open(log_file, FILE_WRITE);//abrir log
      if (myFile) {  
        myFile.print("Se guardo imagen:");
        myFile.print(image_file);
        myFile.print("\n");
        myFile.close();
      }else{
        error_SDfile(log_file);
      }
      
      image_number = image_number + 1;
    }else{
      error_SDfile(image_file);
    }
    #ifdef debugging
     sermon.println("Guardada");
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
            //int temp = Celsius_thermal_image[x][y];
            int temp = (3.2*(image[x][y])-(23221+25*cam_temp))/100;
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
  if (myFile) {
    myFile.print("System type:");
    myFile.println(system_type);
    myFile.print("System id:");
    myFile.println(system_id);
    myFile.print("Autopilot type:");
    myFile.println(autopilot_type);
    myFile.print("Frame:");
    myFile.println(frame);

    myFile.close();
  }else{
    error_SDfile(log_file);
  }
}


//Función para guardar coordenadas del Homeen el arhivo log en la tarjeta SD
void sd_home_save(void){
  myFile = SD.open(log_file, FILE_WRITE);
  if (myFile) {  
    myFile.print("Home WP:");
    myFile.print("\t");
    myFile.print("lat:");
    printDouble(home_wp.lat,6);
    myFile.print("\t");
    myFile.print("lon:");
    printDouble(home_wp.lon,6);
    myFile.print("\t");
    myFile.print("alt:");
    printDouble(home_wp.alt,6);
    myFile.print("\n");
    myFile.close();
  }else{
    error_SDfile(log_file);
  }
}




//Función para actualizar el arhivo log en la tarjeta SD
void sd_log_save(){
    myFile = SD.open(log_file, FILE_WRITE);
    if (myFile) {
      myFile.print("Actual time:");
      print_date(actual_time_us);
      //myFile.print("Unix time:");
      //print_date(time_unix_usec);
      myFile.print("\n");
      myFile.print("Boot time (ms):");
      print_date(time_boot_ms);
      myFile.print("\n");
      myFile.print("Arming time (ms):");
      print_date(arming_time_utc);
      myFile.print("\n");
      myFile.print("Takeoff time (ms):");
      print_date(takeoff_time_utc);      
      myFile.print("\n");
      
      myFile.print("Sensor left (m):");
      myFile.print(sensor_dist[1], DEC);
      myFile.print("\n");
      myFile.print("Sensor front (m):");
      myFile.print(sensor_dist[2], DEC);
      myFile.print("\n");
      myFile.print("Sensor rigth (m):");
      myFile.print(sensor_dist[3], DEC);
      myFile.print("\n");
      myFile.print("Sensor down (m):");
      myFile.print(sensor_dist[0], DEC);
      myFile.print("\n");
      myFile.print("Number of objects:");
      myFile.print(r, DEC);
      myFile.print("\n");
      myFile.print("Number of elements in the biggest object:");
      myFile.print(s, DEC);
      myFile.print("\n");
      //array con datos anteriores y envio a GCS

      myFile.print("Actual pos:");
      myFile.print("\t");
      myFile.print("lat:");
      myFile.print(actual_wp.lat, DEC);
      myFile.print("\t");
      myFile.print("lon:");
      myFile.print(actual_wp.lon, DEC);
      myFile.print("\t");
      myFile.print("alt:");
      myFile.print(actual_wp.alt, DEC);
      myFile.print("\t");
      myFile.print("alt:");
      myFile.print(actual_yaw, DEC);
      myFile.print("\n");

      myFile.print("Attitude:");
      myFile.print("\t");
      myFile.print("roll:");
      myFile.print(att_roll, DEC);
      myFile.print("\t");
      myFile.print("yaw:");
      myFile.print(att_yaw, DEC);
      myFile.print("\t");
      myFile.print("pitch:");
      myFile.print(att_pitch, DEC);
      myFile.print("\t");
      myFile.print("roll speed:");
      myFile.print(att_roll_speed, DEC);
      myFile.print("\t");
      myFile.print("yaw speed:");
      myFile.print(att_yaw_speed, DEC);
      myFile.print("\t");
      myFile.print("pitch speed:");
      myFile.print(att_pitch_speed, DEC);
      myFile.print("\n");

      myFile.print("Number of mission WP:");
      myFile.print(count, DEC);
      myFile.print("\n");
      myFile.print("Number of current WP:");
      myFile.print(seq, DEC);
      myFile.print("\n");
      
      myFile.print("Current WP:");
      myFile.print("\t");
      myFile.print("lat:");
      myFile.print(current_wp.lat, DEC);
      myFile.print("\t");
      myFile.print("lon:");
      myFile.print(current_wp.lon, DEC);
      myFile.print("\t");
      myFile.print("alt:");
      myFile.print(current_wp.alt, DEC);
      myFile.print("\n");

      myFile.print("Battery remaining (%):");
      myFile.print(bat, DEC);
      myFile.print("\n");
      myFile.print("Battery temperature (°C):");
      myFile.print(bat_temp, DEC);
      myFile.print("\n");
      myFile.print("Battery current (mA):");
      myFile.print(bat_curr, DEC);
      myFile.print("\n");
      myFile.print("Battery voltage of cells (mV):");
      myFile.print(bat_volt[0], DEC);
      myFile.print("\t");
      myFile.print(bat_volt[1], DEC);
      myFile.print("\t");
      myFile.print(bat_volt[2], DEC);
      myFile.print("\n");
      myFile.print("Battery consumed energy (J):");
      myFile.print(bat_ener, DEC);
      myFile.print("\n");
      
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
  time_unix = time_unix - 10800000; //UTM-3
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
     myFile.print("(AM)");
  }else{
    myFile.print("(PM)");
  }
  myFile.print("\t");
  switch(weekday(time_unix)){
    case 0:{
      myFile.print("(Sunday)");
      break;
    }
    case 1:{
      myFile.print("(Monday)");
      break;
    }
    case 2:{
      myFile.print("(Tuesday)");
      break;
    }
    case 3:{
      myFile.print("(Wednesday)");
      break;
    }
    case 4:{
      myFile.print("(Thursday)");
      break;
    }
    case 5:{
      myFile.print("(Friday)");
      break;
    }
    case 6:{
      myFile.print("(Saturday)");
      break;
    }
    default:
    break;
  }
  horario = day(time_unix);
  myFile.print(horario);
  myFile.print("/");
  horario = month(time_unix);
  myFile.print(horario);
  myFile.print("/");
  horario = year(time_unix);
  myFile.print(horario);
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
        sermon.println("Escribiendo SD: ");
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
