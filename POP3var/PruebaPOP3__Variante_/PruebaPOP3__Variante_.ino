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

//Write here you server and account data
const char pop3_server[ ] = "mail.montevideo.com.uy";      
const char user_name[ ] = "termodron1";   
const char password[ ] = "Termo1";    
const char pop3_port[ ] = "110";     

//Write here you SIM card data
const char sim_pin[] = "4725";
const char sim_apn[] = "prepago.ancel";
const char sim_user[] = "bam";
const char sim_password[] = "bam";

char data[64];
String atcommand;
String from;
String date;
String subject;
String code_subject;
String to;
String body;
char path[50];
String totalemails; //[10];
int totalemailsint;
String lasttotalemails = "0";
int8_t answer;
int onModulePin= 8;
int counter;
unsigned long previous;
char aux_str[128];
String aux_strg;
int code;
struct coord{
  float latitud;
  float longitud;
  float altitud;
};
coord coordenadas[10];

void setup(){

  pinMode(onModulePin, OUTPUT);
  Serial.begin(115200);
  Serial3.begin(19200);

  Serial.println("Starting...");
  power_on();

  delay(3000);

  //sets the PIN code
  sprintf(aux_str, "AT+CPIN=%s", sim_pin);
  sendATcommand(aux_str, "OK", 2000);

  delay(3000);

  while( (sendATcommand("AT+CREG?", "+CREG: 0,1", 500) || 
    sendATcommand("AT+CREG?", "+CREG: 0,5", 500)) == 0 );

  // sets pop3 server
  sprintf(data, "AT+POP3SRV=\"%s\",\"%s\",\"%s\",%s", pop3_server, user_name, password, pop3_port);
  sendATcommand(data, "OK", 2000);

  // sets APN, user name and password
  sprintf(aux_str, "AT+CGSOCKCONT=1,\"IP\",\"%s\"", sim_apn);
  sendATcommand(aux_str, "OK", 2000);

  sprintf(aux_str, "AT+CSOCKAUTH=1,1,\"%s\",\"%s\"", sim_user, sim_password);
  sendATcommand(aux_str, "OK", 2000);

  delay(2000);

  Serial.println("AT+CPING=\"www.google.com\",1,4"); 
  Serial3.println("AT+CPING=\"www.google.com\",1,4"); 
  while(Serial3.available()==0);
  while(Serial3.available()!=0){
    Serial.print(Serial3.readString());
  }
  Serial.println();
}             //Descomentar con el loop
  
void loop(){

// logs into the server*********************************************************
  answer = sendATcommand("AT+POP3IN", "OK",10000);
  if (answer == 1)
  {
    Serial.println("Logged into the server");

    // gets the total of emails
  sendATcommand("AT+POP3NUM", "+POP3: ",10000);
  if (answer == 1)
    {
      totalemails = Serial3.readStringUntil(',');
      
      Serial.print("Total emails in the POP3 server: ");                
      Serial.println(totalemails);

    }
    else
    {
      Serial.println("Error getting emails counter");   
    }

    if (totalemails != lasttotalemails)
      {
      aux_strg = "AT+POP3GET=" + totalemails;
      aux_strg.toCharArray(aux_str, 128);
      answer = sendATcommand(aux_str, "C:/Email/",10000);
      if (answer == 1)
        {
          counter = 0;
          while(Serial3.available()==0);
          do{
            path[counter] = Serial3.read();
            counter++;
            if ((path[counter-1] == ' ') || (path[counter-1] == ','))
              {
                counter--;    
              }
            while(Serial3.available()==0);
            }
          while (path[counter-1] != 0x0D);
        
          path[counter-1] = '\0';
        
          Serial.print("Email folder: ");                
          Serial.println(path);
          lasttotalemails = totalemails;
        }
      else
      {
        Serial.println("Error getting the email");   
      }
      }
    else
      {
        Serial.println("There's no new emails on Inbox");
        answer=0;
      }
    sendATcommand("AT+POP3OUT", "OK",10000);
  }

  if (answer == 1)
  {

    sendATcommand("AT+FSCD=C:/Email", "OK", 2000);
    //sendATcommand("AT+FSLS", "OK", 10000);
    
    sprintf(data, "AT+POP3READ=0,\"%s\"", path);
    
    // Clean the input buffer
    while( Serial3.available() > 0) Serial3.read();   

    // Send the AT command 
    Serial.println(data);
    Serial3.println(data);
    counter = 0;
    previous = millis();
    answer = 0;
    //memset(email, '\0', (sizeof(email)/sizeof(email[0])));
    // this loop waits for the answer
    do{

      if(Serial3.available() != 0){    
        Serial3.readStringUntil('\n');
        Serial3.readStringUntil('\n');
        Serial3.readStringUntil('\n');
        Serial3.readStringUntil(' ');
        code_subject = Serial3.readStringUntil('_');
        code = code_subject.toInt();
        subject = Serial3.readStringUntil('\n');
        Serial3.readStringUntil('\n');
        switch (code){
          case 10:
            {
              counter = 0;
              Serial.println("Se debe cargar la mision:");
              aux_strg = Serial3.readStringUntil(';');
              //Serial.print(aux_strg);
              while(aux_strg != "FIN"){
                coordenadas[counter].latitud = aux_strg.toFloat();
                Serial.print("Latitud: ");
                Serial.println(coordenadas[counter].latitud);
                aux_strg = Serial3.readStringUntil(';');
                coordenadas[counter].longitud = aux_strg.toFloat();
                Serial.print("Longitud: ");
                Serial.println(coordenadas[counter].longitud);
                aux_strg = Serial3.readStringUntil('\n');
                coordenadas[counter].altitud = aux_strg.toFloat(); 
                Serial.print("Altitud: ");
                Serial.println(coordenadas[counter].altitud);
                counter = counter++;
                Serial.println();
                aux_strg = Serial3.readStringUntil(';');
              }
              
            }
            break;
           default:
            break;
        }
        body = Serial3.readString();
        //counter++;
        
        // check if "OK" is in the response of the module
        //if (strstr(email, "#") != NULL)    
        //{
          answer = 1;
        //}

        // check if "ERROR" is in the response of the module
        //if (strstr(email, "ERROR") != NULL)    
        //{
          //answer = 2;
        //}
      }
      // Waits for the answer with time out
    }
    while((answer == 0) && ((millis() - previous) < 10000) && (counter < 1023));

    if (answer == 1)
    {
      Serial.print("Asunto: ");
      Serial.println(subject);
      Serial.print("Email: ");
      Serial.println(body);
    }
    else if (counter == 1023)
    {
      Serial.println("Buffer limit reached!! Data received:");
      Serial.println();       
    }
    else
    {
      Serial.println("Error");
    }

  }


delay (30000);
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


int8_t sendATcommand(char* ATcommand, char* expected_answer1,unsigned int timeout){

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
      Serial.print(response[x]); //Print the response on the console
      x++;
      // check if the desired answer is in the response of the module
      if (strstr(response, expected_answer1) != NULL)    
      {
        answer = 1;
        //Serial.print(response);
        Serial.println();
      }
    }
    // Waits for the answer with time out
  }
  while((answer == 0) && ((millis() - previous) < timeout));    

  return answer;
}
