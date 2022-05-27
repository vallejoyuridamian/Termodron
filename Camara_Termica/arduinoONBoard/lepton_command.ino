void lepton_command(unsigned int moduleID, unsigned int baseID, unsigned int command){
  Wire.beginTransmission(ADDRESS);

  // Command Register is a 16-bit register located at Register Address 0x0004
  Wire.write(0x00);
  Wire.write(0x04);

  //OEM module ID
  if (moduleID == OEM) {
    Wire.write(0x48);
  }
  else{
    Wire.write(moduleID & 0x0f);
  }
  
  Wire.write( ((baseID ) & 0xfc) | (command & 0x3));

  error = Wire.endTransmission();    // stop transmitting
  errorWire();
}
