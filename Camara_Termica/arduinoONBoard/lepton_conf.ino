void lepton_conf(void){
  int dt;
  lepton_command(SYS, 0x10 , GET);
  dt = read_data();  //devuelve la data 
}


void agc_enable(){
  Wire.beginTransmission(ADDRESS); // transmit to device #4
  Wire.write(0x01);
  Wire.write(0x05);
  Wire.write(0x01);
  Wire.write(0x01);

  error = Wire.endTransmission();    // stop transmitting
  errorWire();
}
