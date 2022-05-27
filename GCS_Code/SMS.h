//*****************************************************************************************
//Definición de la función recieveSMS

void recieveSMS(){

  int x;
  Consola.println(F("Setting SMS mode..."));
  sendATcommand("AT+CMGF=1", "OK", 1000);    // sets the SMS mode to text
  sendATcommand("AT+CPMS=\"SM\",\"SM\",\"SM\"", "OK", 1000);    // selects the memory

  answer = sendATcommand("AT+CMGR=1", "+CMGR:", 2000);    // reads the first SMS
  if (answer == 1){
    answer = 0;
    while(Shield3G.available() == 0);
    // this loop reads the data of the SMS
    do{
      // if there are data in the UART input buffer, reads it and checks for the answer
      if(Shield3G.available() > 0){    
        SMS[x] = Shield3G.read();
        x++;
        // check if the desired answer (OK) is in the response of the module
        if (strstr(SMS, "OK") != NULL){
          answer = 1;
        }
      }
    } while(answer == 0);    // Waits for the answer with time out

    SMS[x] = '\0';
    Consola.println(SMS);    

  }
  else{
    Consola.print(F("error "));
    Consola.println(answer, DEC);
  }

}

//*****************************************************************************************
//Definición de la función sendSMS

void sendSMS(char* smsText, const char* phoneNumber){

  Consola.println(F("Connecting to the network..."));

  //mientras "registered, home network" (0,1) o "registered, roaming" (0,5) o 500 ms timeout
  while( (sendATcommand("AT+CREG?", "+CREG: 0,1", 500) || sendATcommand("AT+CREG?", "+CREG: 0,5", 500)) == 0 ); 

  Consola.print(F("Setting SMS mode..."));
  sendATcommand("AT+CMGF=1", "OK", 1000);    // sets the SMS mode as text mode (no PDU mode)
  Consola.println(F("Sending SMS"));

  sprintf(aux_str,"AT+CMGS=\"%s\"", phoneNumber);
  answer = sendATcommand(aux_str, ">", 2000);    // send the SMS number
  if (answer == 1){
    Shield3G.println(smsText);
    Shield3G.write(0x1A);
    answer = sendATcommand("", "OK", 20000);
    if (answer == 1){
      Consola.print(F("SMS Sent Successfully"));    
    }
    else{
      Consola.print(F("error "));
    }
  }
  else{
    Consola.print(F("error "));
    Consola.println(answer, DEC);
  }
  
}

