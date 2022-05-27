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

int8_t answer;
int onModulePin= 8;
char aux_str[30];

//Change here your data
const char pin[]="4725";
const char phone_number[]="226";
const char sms_text[]="100";  

void setup(){

  pinMode(onModulePin, OUTPUT);
  Serial.begin(115200);
  Serial3.begin(19200);    

  Serial.println("Starting...");
  power_on();

  delay(3000);

  //sets the PIN code
  sprintf(aux_str, "AT+CPIN=%s", pin);
  sendATcommand(aux_str, "OK", 2000);
  Serial.println(phone_number);
  delay(3000);

  Serial.println("Connecting to the network...");

  while( (sendATcommand("AT+CREG?", "+CREG: 0,1", 500) || 
    sendATcommand("AT+CREG?", "+CREG: 0,5", 500)) == 0 );

  Serial.print("Setting SMS mode...");
  sendATcommand("AT+CMGF=1", "OK", 1000);    // sets the SMS mode to text
  Serial.println("Sending SMS");

  sprintf(aux_str,"AT+CMGS=\"%s\"", phone_number);
  answer = sendATcommand(aux_str, ">", 2000);    // send the SMS number
  if (answer == 1)
  {
    Serial3.println(sms_text);
    Serial3.write(0x1A);
    answer = sendATcommand("", "OK", 20000);
    if (answer == 1)
    {
      Serial.print("Sent ");    
    }
    else
    {
      Serial.print("error ");
    }
  }
  else
  {
    Serial.print("error ");
    Serial.println(answer, DEC);
  }

}


void loop(){

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
    while(answer == 0){     // Send AT every two seconds and wait for the answer
      answer = sendATcommand("AT", "OK", 2000);    
    }
  }

}

int8_t sendATcommand(char* ATcommand, char* expected_answer, unsigned int timeout){

  uint8_t x=0,  answer=0;
  char response[100];
  unsigned long previous;

  memset(response, '\0', 100);    // Initialice the string

  delay(100);

  while( Serial3.available() > 0) Serial3.read();    // Clean the input buffer

  Serial3.println(ATcommand);    // Send the AT command 


    x = 0;
  previous = millis();

  // this loop waits for the answer
  do{
    // if there are data in the UART input buffer, reads it and checks for the answer
    if(Serial3.available() != 0){    
      response[x] = Serial3.read();
      x++;
      // check if the desired answer is in the response of the module
      if (strstr(response, expected_answer) != NULL)    
      {
        answer = 1;
      }
    }
    // Waits for the answer with time out
  }
  while((answer == 0) && ((millis() - previous) < timeout));    

  return answer;
}

