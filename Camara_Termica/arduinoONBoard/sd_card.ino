#ifdef SD_present
void SD_picture_save(){
    if(!SD.exists("/p")){
      SD.mkdir("/p");
    }
    sprintf(image_file, "/p/ima%d.csv", image_number);
    if(SD.exists(image_file)){
      SD.remove(image_file);
    }
    sermon.println(image_file);
    myFile = SD.open(image_file, FILE_WRITE);//abrimos  el archivo
    if (myFile) { 
      sermon.println("Escribiendo SD: ");
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
      errorwriteSDfile();     
    }
}


void sd_log_save(){
    myFile = SD.open("log.txt", FILE_WRITE);
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
      datasend_sensor();      

      
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
      myFile.print(current, DEC);
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
      errorwriteSDfile();
    }
}


      
void sd_log_init(void){
  if(SD.exists("log.txt")){
    SD.remove("log.txt");
  }
  myFile = SD.open("log.txt", FILE_WRITE);
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
    errorwriteSDfile();
  }
}    


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
