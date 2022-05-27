//Funcion para configurar la Flir Lepton
void lepton_set_conf() {
  agc_setAGCEnabled(false);
  agc_setAGCPolicy(AGC_HEQ);
  sys_setTelemetryEnabled(false);
}

//Funcion para obtener la configuración la Flir Lepton
void lepton_get_conf() {
  char buffer[80];
  sys_getCustomerSerialNumber(buffer, 80);
  sermon.print(F("Customer Serial Number:"));
  sermon.println(buffer);
  sys_getFlirSerialNumber(buffer, 80);
  sermon.print(F("Flir Serial Number:"));
  sermon.println(buffer);
  
  lepton_status();
    
  /*oem_getSystemPartNumber(buffer, 80);
  sermon.print("OEM System Part Number:");
  sermon.println(buffer);
  oem_getCameraSoftwareRevision(buffer, 80);
  sermon.print("Camera Software Revision:");
  sermon.println(buffer);
  sermon.print("OEM Status:");
  sermon.println(oem_getStatus());*/  
}

//Funcion para obtener el estado de la Flir Lepton
void lepton_status() {

  // Occasionally flat field correction normalization needs ran
  /*if (getShouldRunFFCNormalization()){
      sys_runFFCNormalization();
  }*/
  
  sermon.print(F("Camera booted:"));
  sermon.println(cameraBooted());
  sermon.print(F("Camera Status:"));
  sermon.println(sys_getCameraStatus());
  
  sermon.print(F("AGC Enable:"));
  sermon.println(agc_getAGCEnabled());
  sermon.print(F("AGC Policy:"));
  sermon.println(agc_getAGCPolicy());
  

  sermon.print(F("Camera Uptime:"));
  sermon.println(sys_getCameraUptime());
  sermon.print(F("Aux Temperature:"));
  cam_temp = sys_getAuxTemperature();
  sermon.println(cam_temp);
  sermon.print(F("FPA Temperature:"));
  fpa_temp = sys_getFPATemperature();
  sermon.println(fpa_temp);
  sermon.print(F("Telemetry Enable:"));
  sermon.println(sys_getTelemetryEnabled());
  sermon.print(F("Thermal Shutdown Count:"));
  sermon.println(sys_getThermalShutdownCount());
  sermon.print(F("Shutter Position:"));
  sermon.println(sys_getShutterPosition());
  sermon.print(F("FFC Normalization Status:"));
  sermon.println(sys_getFFCNormalizationStatus());
  
}
