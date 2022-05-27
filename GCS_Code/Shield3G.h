  
void resetGCS();
int CalcularTrayectoria(Punto* coordenadas,int counter);
void setPin();
int8_t sendATcommand(char* ATcommand, String expected_answer1, unsigned int timeout);
int8_t sendATcommandChar(char* ATcommand, char* expected_answer1, unsigned int timeout);

void power_on(){

  int onModulnePin= 8;
  int rstModulePin= 9;
  uint8_t answer=0;
  int timeout= 30000;
  int contador;
  String aux_str;
  bool listo = false;
  
  // checks if the module is started
  answer = sendATcommand("AT", "OK", 2000);
  if (answer == 0){
    // power on pulse
    digitalWrite(onModulePin,HIGH);
    delay(100);
    digitalWrite(onModulePin,LOW);

   // waits for an answer from the module
  //  while(answer == 0){     // Send AT every two seconds and wait for the answer
  //    answer = sendATcommand("AT", "OK", 2000);    
  //  }
  }
  contador= millis();
  while((not(listo))and(millis()-contador < timeout)){
    if(Shield3G.available()>0){
      aux_str=Shield3G.readString();
      Consola.println(aux_str);
      if (aux_str.indexOf("SIM PIN") >=0){
        setPin();
      }        
      if (aux_str.indexOf("PB DONE") >=0){
        listo = true;
        Consola.println(F("Fin Inicializacion"));  
      }
    }
  }
  if(not(listo)){
    Consola.println(F("Salio por timeout en la inicializacion"));
    resetGCS();
  }
}

void power_off(){

  int onModulePin= 8;
  int rstModulePin= 9;
  uint8_t answer=0;

  if (Shield3G.available() > 0){

    while(Shield3G.available() > 0) Shield3G.read();
    //power off
    digitalWrite(rstModulePin,HIGH);
    delay(3000);
    digitalWrite(rstModulePin,LOW);
  }
  else{
    answer = sendATcommand("AT", "OK", 2000);
    if (answer = 1){
      digitalWrite(rstModulePin,HIGH);
      delay(3000);
      digitalWrite(rstModulePin,LOW);
    }
  }
}

void setSim (){
   
   // sets APN, user name and password
  sprintf(aux_str, "AT+CGSOCKCONT=1,\"IP\",\"%s\"", sim_apn);
  sendATcommand(aux_str, "OK", 2000);

  sprintf(aux_str, "AT+CSOCKAUTH=1,1,\"%s\",\"%s\"", sim_user, sim_password);
  sendATcommand(aux_str, "OK", 2000);
}

void setPin(){
  sprintf(aux_str, "AT+CPIN=%s", sim_pin);
  sendATcommand(aux_str, "OK", 2000);
}


//*****************************************************************************************
//Definición de la función sendATcommand

int8_t sendATcommand(char* ATcommand, String expected_answer1, unsigned int timeout){

  uint8_t x=0,  answer=0;
  //char response[100];
  String response;
  unsigned long previous;

//  memset(response, '\0', 100);    // Initialize the string


  while( Shield3G.available() > 0) Shield3G.read();    // Clean the input buffer

  //Consola.println(ATcommand);    // Print the AT command on console
  Shield3G.println(ATcommand);   // Send the AT command to the shield

  x = 0;
  previous = millis();

  // this loop waits for the answer
  do{
    // if there are data in the UART input buffer, reads it and checks for the answer
    if(Shield3G.available() != 0){    
      //response[x] = Shield3G.read();
      response= Shield3G.readString();//Until('\n');
      //Consola.print(response[x]); //Print the response on the console
      //x++;
      Consola.println(response);
      // check if the desired answer is in the response of the module
      //if (strstr(response, expected_answer1) != NULL){
      if (response.indexOf(expected_answer1) >=0){
        answer = 1;
      }
    }
    // Waits for the answer with time out
 
  } while((answer == 0) && ((millis() - previous) < timeout));    

  return answer;
}


int8_t sendATcommandChar(char* ATcommand, char* expected_answer1, unsigned int timeout){

  uint8_t x=0,  answer=0;
  char response[100];
  unsigned long previous;

  memset(response, '\0', 100);    // Initialize the string


  while(Shield3G.available() > 0) Shield3G.read();    // Clean the input buffer

  //Serial.println(ATcommand);    // Print the AT command on console
  Shield3G.println(ATcommand);   // Send the AT command to the shield

  x = 0;
  previous = millis();

  // this loop waits for the answer
  do{
    // if there are data in the UART input buffer, reads it and checks for the answer
    if(Shield3G.available() != 0){    
      response[x] = Shield3G.read();
      Consola.print(response[x]); //Print the response on the console
      x++;
      // check if the desired answer is in the response of the module
      if (strstr(response, expected_answer1) != NULL){
      //if (response.indexOf(expected_answer1) >=0){
        answer = 1;
      }
    }
    // Waits for the answer with time out
 
  } while((answer == 0) && ((millis() - previous) < timeout));    

  return answer;
}

void resetGCS(){
  
 pinMode(RESET_PIN,OUTPUT);
 #ifdef ConsolaMon
 Consola.println("Reseteando");
 #endif
 digitalWrite(RESET_PIN,LOW);
  
 }

