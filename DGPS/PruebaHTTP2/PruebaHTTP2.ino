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
const char pin_number[] = "4725";
const char apn[] = "prepago.ancel";
const char user_name[] = "bam";
const char password[] = "bam";

char url[ ]="201.217.132.178";
int port= 2101;
char request[ ]="GET /UYMO HTTP/1.1\r\nUser-Agent: termodron\r\nAccept: */*\r\nConnection: close\r\nAuthorization: Basic dGVybW9kcm9uOnRlcm1vZHJvbjE=\r\n\r\n"; //


// termodron:termodron1 = dGVybW9kcm9uOnRlcm1vZHJvbjE=

int8_t answer;
int onModulePin = 8, aux;
int data_size = 0;
int end_file = 0;
char aux_str[50];
String aux_strg;
char data[400];
int http_x;
int http_status; // 0: end, 1 waiting data with header, 2 waiting data without header, 3 timeout waiting data, 4 unknow response
int x = 0;
long previous;

void setup(){
  pinMode(onModulePin, OUTPUT);
  Serial.begin(115200);
  Serial3.begin(115200);

  Serial.println("Starting...");
  power_on();

  delay(3000);

  //sets the PIN code
  sprintf(aux_str, "AT+CPIN=%s", pin_number);
  sendATcommand(aux_str, "OK", 2000);

  delay(3000);

  while( (sendATcommand("AT+CREG?", "+CREG: 0,1", 500) ||
    sendATcommand("AT+CREG?", "+CREG: 0,5", 500)) == 0 );

  // sets APN, user name and password
  sprintf(aux_str, "AT+CGSOCKCONT=1,\"IP\",\"%s\"", apn);
  sendATcommand(aux_str, "OK", 2000);

  sprintf(aux_str, "AT+CSOCKAUTH=1,1,\"%s\",\"%s\"", user_name, password);
  sendATcommand(aux_str, "OK", 2000);

  sendATcommand("AT+NETCLOSE", "OK", 2000);

  sendATcommand("AT+NETOPEN=\"TCP\",2101", "OK", 2000);
  
  /*Serial.println("AT+NETOPEN=\"TCP\",2101"); 
  Serial3.println("AT+NETOPEN=\"TCP\",2101"); 
  while(Serial3.available()==0);
  while(Serial3.available()!=0){
    Serial.print(Serial3.readStringUntil('K'));
  }
  Serial.println();*/

 /* previous = millis();
  sprintf(aux_str, "AT+CHTTPACT=\"%s\",%d", url, port);
  //answer = sendATcommand(aux_str, "+CHTTPACT: 0", 60000);
  //Serial.print(aux_str);
  Serial3.print(aux_str);

  uint8_t i = 0,  answer = 0;
  char response[1000];
  //unsigned long previous;
  do {
    // if there are data in the UART input buffer, reads it and checks for the asnwer
    if (Serial3.available() != 0) {
      response[i] = Serial3.read();
      Serial.print(response[i]);
      i++;
      // check if the desired answer is in the response of the module
    }
    // Waits for the asnwer with time out
  }
  while (((millis() - previous) < 120000));
Serial.println("Timeout");*/
}
  
void loop(){

  // request the url
  
  previous = millis();
  sprintf(aux_str, "AT+CHTTPACT=\"%s\",%d", url, port);
  answer = sendATcommand(aux_str, "+CHTTPACT: REQUEST", 10000);
  Serial.println(answer);
  //Serial.println(aux_str);
  //Serial3.println("AT+CHTTPACT=\"201.217.132.178\",2101");
  //while (Serial3.available() > 0){
     //Serial.print(Serial3.readString());
  //}

  // Sends the request
  Serial.println();
  Serial.println(request);
  Serial3.println(request);
  // Sends <Ctrl+Z>
  Serial3.write(0x1A);
  //Serial3.write(0x00);
  
  /*while (1==1){
    while(Serial3.available()==0);
    while (Serial3.available() > 0){
      Serial.print(Serial3.readString());
    }
    Serial.println();
  }*/
  
  http_status = 1;

  while ((http_status == 1) || (http_status == 2))
  {
    answer = sendATcommand("", "+CHTTPACT: ", 5000);
    //Serial.println(answer);
    if (answer == 0)
    {
      if (http_status == 1)
      {
        http_status = 3;
      }
      if (http_status == 2)
      {
        http_status = 5;
      }
    }
    else
    {
      // answer == 1
      while(Serial3.available()==0);
      while (Serial3.available() != 0){
      Serial.print(Serial3.readString());
    }
      //aux_strg = Serial3.read();
      //Serial.println(aux_strg);
      
      if ((aux_str[0] == 'D') && (http_status == 1))
      {
        // Data packet with header
        while(Serial3.available()<4);
        Serial3.read();  // A
        Serial3.read();  // T
        Serial3.read();  // A
        Serial3.read();  // ,

        // Reads the packet size
        x=0;
        do{
          while(Serial3.available()==0);
          aux_str[x] = Serial3.read();
          x++;
        }
        while((aux_str[x-1] != '\r') && (aux_str[x-1] != '\n'));
        
        aux_str[x-1] = '\0';
        data_size = atoi(aux_str);
        Serial.print(data_size);

        // Now, search the end of the HTTP header (\r\n\r\n)
        do{
          while (Serial3.available() < 3);

          data_size--;
          if (Serial3.read() == '\r')
          {
            data_size--;
            if (Serial3.read() == '\n')
            {
              data_size--;
              if (Serial3.read() == '\r')
              {
                data_size--;
                if (Serial3.read() == '\n')
                {
                  // End of the header found
                  http_status = 2;
                }
              }
            }  
          }
        }
        while ((http_status == 1) && (data_size > 0));

        if (http_status == 2)
        {
          // Reads the data
          http_x = 0;
          do{
            if(Serial3.available() != 0){
              data[http_x] = Serial3.read();
              http_x++;
              data_size--;
            }
            else
            {
              delay(1);
            }
          }
          while(data_size > 0);
          data[http_x] = '\0';
          Serial.print(data);
        }
      }
      else if ((aux_str[0] == 'D') && (http_status == 2))
      {
        // Data packet with header
        while(Serial3.available()<4);
        Serial3.read();  // A
        Serial3.read();  // T
        Serial3.read();  // A
        Serial3.read();  // ,

        // Reads the packet size
        x=0;
        do{
          while(Serial3.available()==0);
          aux_str[x] = Serial3.read();
          x++;
        }
        while((aux_str[x-1] != '\r') && (aux_str[x-1] != '\n'));

        aux_str[x-1] = '\0';
        data_size = atoi(aux_str);
        Serial.print(data_size);
        
        do{
          if(Serial3.available() != 0){
            data[http_x] = Serial3.read();
            http_x++;
          }
          else
          {
            delay(1);
          }
        }
        while(data_size > 0);
        data[http_x] = '\0';

      }
      else if (aux_str[0] == '0')
      {
        // end of the AT command
        http_status = 0;
      }
      else
      {
        // unknow response
        http_status = 4;
        Serial3.print(char(aux_str[0]));
        Serial3.print(char(Serial3.read()));
        Serial3.print(char(Serial3.read()));
        Serial3.print(char(Serial3.read()));
        Serial3.print(char(Serial3.read()));
        Serial3.print(char(Serial3.read()));
        Serial3.print(char(Serial3.read()));
        Serial3.print(char(Serial3.read()));
        Serial3.print(char(Serial3.read()));
      }     
    }   
  }

  previous = millis() - previous;
  
  Serial.println(previous, DEC);
  if (http_status == 0)
  {
    Serial.print(F("HTTP data: "));
    Serial.println(data);
  }
  else
  {
    Serial.print(F("http_status: "));
    Serial.println(http_status, DEC);
  }



  delay(10000);

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


int8_t sendATcommand(char* ATcommand, char* expected_answer1, unsigned int timeout) {

  uint8_t x = 0,  answer = 0;
  char response[1000];
  unsigned long previous;

  memset(response, '\0', 1000);    // Initialize the string

  delay(100);

  while ( Serial3.available() > 0) Serial3.read();   // Clean the input buffer

  // Serial.println(ATcommand);
  Serial3.println(ATcommand);    // Send the AT command

  x = 0;
  previous = millis();

  // this loop waits for the answer
  do {
    // if there are data in the UART input buffer, reads it and checks for the asnwer
    if (Serial3.available() != 0) {
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
  while ((answer == 0) && ((millis() - previous) < timeout));

  return answer;
}

 

 
