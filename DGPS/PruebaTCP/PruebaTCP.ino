/*  
 *  3G + GPS shield
 *  
 *  Copyright (C) Libelium Comunicaciones Distribuidas S.L. 
 *  http://www.libelium.com 
 *  
 *  This program is free software: you can redistribute it and/or modify 
 *  it under the terms of the GNU General Public License as published by 
 *  the Free Software Foundation, either version 3 of the License, or 
 *  (at your option) any later version. 
 *  a
 *  This program is distributed in the hope that it will be useful, 
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of 
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 *  GNU General Public License for more details.
 *  
 *  You should have received a copy of the GNU General Public License 
 *  along with this program.  If not, see http://www.gnu.org/licenses/. 
 *  
 *  Version:           2.0
 *  Design:            David Gascón 
 *  Implementation:    Alejandro Gallego & Victor Boria
 */

//Change here your data
const char pin[]="4725";
const char apn[]="prepago.ancel";
const char user_name[]="bam";
const char password[]="bam";
const char TCP_server_1[ ]="201.217.132.178";
const char TCP_port_1[ ]="2101";
char TCP_message_1[ ]="ping";

int8_t answer;
int onModulePin= 8;
char aux_str[50];


void setup(){

  pinMode(onModulePin, OUTPUT);
  Serial.begin(115200);   
  Serial3.begin(115200); 

  Serial.println("Starting...");
  power_on();

  delay(3000);

  //sets the PIN code
  snprintf(aux_str, sizeof(aux_str), "AT+CPIN=%s", pin);
  sendATcommand(aux_str, "OK", 2000);

  delay(3000);

  while( (sendATcommand("AT+CREG?", "+CREG: 0,1", 500) || 
    sendATcommand("AT+CREG?", "+CREG: 0,5", 500)) == 0 );

  // sets APN, user name and password
  snprintf(aux_str, sizeof(aux_str), "AT+CGSOCKCONT=1,\"IP\",\"%s\"", apn);
  sendATcommand(aux_str, "OK", 2000);

  snprintf(aux_str, sizeof(aux_str), "AT+CSOCKAUTH=1,1,\"%s\",\"%s\"", user_name, password);
  sendATcommand(aux_str, "OK", 2000);
}

void loop(){

  sprintf(aux_str, "AT+NETOPEN=\"TCP\",%s", TCP_port_1);
  answer = sendATcommand(aux_str, "Network opened", 20000);

  if (answer == 1)
  {
    Serial.println("Network opened");
    sprintf(aux_str, "AT+TCPCONNECT=\"%s\",%s", TCP_server_1, TCP_port_1);
    answer = sendATcommand(aux_str, "Connect ok", 20000);
    if (answer == 1)
    {
      Serial.println("Socket opened");
      sprintf(aux_str, "AT+TCPWRITE=%d", strlen(TCP_message_1));
      answer = sendATcommand(aux_str, ">", 20000);
      if (answer == 1)
      {
        sendATcommand(TCP_message_1, "Send OK", 20000);                
      }
      sendATcommand("AT+NETCLOSE", "OK", 20000);
    }
    else
    {
      Serial.println("Error opening the socket");    
    }
  }
  else
  {
    Serial.println("Error opening the network");    
  }

}

void power_on(){

  uint8_t answer=0;

  // checks if the module is started
  answer = sendATcommand("AT", "OK", 2000);
  if (answer == 0)
  {
    // power on pulse
    digitalWrite(onModulePin,HIGH);
    delay(3000);
    digitalWrite(onModulePin,LOW);

    // waits for an answer from the module
    while(answer == 0){    
      // Send AT every two seconds and wait for the answer
      answer = sendATcommand("AT", "OK", 2000);    
    }
  }

}


int8_t sendATcommand(char* ATcommand, char* expected_answer1, unsigned int timeout)
{

  uint8_t x=0,  answer=0;
  char response[100];
  unsigned long previous;

  memset(response, '\0', 100);    // Initialize the string

  delay(100);

  while( Serial3.available() > 0) Serial3.read();    // Clean the input buffer

  Serial3.println(ATcommand);    // Send the AT command 


    x = 0;
  previous = millis();

  // this loop waits for the answer
  do{

    if(Serial3.available() != 0){    
      response[x] = Serial3.read();
      Serial.print(response[x]);
      x++;
      // check if the desired answer is in the response of the module
      if (strstr(response, expected_answer1) != NULL)    
      {
        answer = 1;
        Serial.println();
      }
    }
    // Waits for the asnwer with time out
  }
  while((answer == 0) && ((millis() - previous) < timeout));    

  return answer;
}
