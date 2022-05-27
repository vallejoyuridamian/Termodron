void conf_lepton(void){
  int dt;
  lep_command(SYS, 0x10 , GET);
  dt = read_data();  //devuelve la data 
}
