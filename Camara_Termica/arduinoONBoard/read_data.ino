int read_data(){
  int i;
  int data;
  int payload_length;

  while (read_reg(0x2) & 0x01){
    sermon.println("busy");
  }

  payload_length = read_reg(0x6);
  //sermon.print("payload_length=");
  //sermon.println(payload_length);

  Wire.requestFrom(ADDRESS, payload_length);
  
  for (i = 0; i < (payload_length / 2); i++)
  {
    data = Wire.read() << 8;
    data |= Wire.read();
    //sermon.println(data, DEC);
  }
return data;
}


//Status reg 15:8 Error Code  7:3 Reserved 2:Boot Status 1:Boot Mode 0:busy

int read_reg(unsigned int reg)
{
  int reading = 0;
  set_reg(reg);

  Wire.requestFrom(ADDRESS, 2);

  reading = Wire.read();  // receive high byte (overwrites previous reading)
  //sermon.println(reading);
  reading = reading << 8;    // shift high byte to be high 8 bits
  reading |= Wire.read(); // receive low byte as lower 8 bits
  
  //sermon.print("reg:");
  //sermon.print(reg);
  //sermon.print("== (binary)");
  //sermon.println(reading, BIN);
  return reading;
}


void set_reg(unsigned int reg){
  Wire.beginTransmission(ADDRESS); // transmit to device #4
  Wire.write(reg >> 8 & 0xff);
  Wire.write(reg & 0xff);            // sends one byte

  error = Wire.endTransmission();    // stop transmitting
  errorWire();
}
