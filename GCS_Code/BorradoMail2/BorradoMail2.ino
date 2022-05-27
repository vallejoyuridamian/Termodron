int8_t answer;
int8_t answer_fin;
int onModulePin= 8;
int j;
unsigned long previous;
char aux_str[128];
char command[128];

void setup() {
  pinMode(onModulePin, OUTPUT);
  Serial.begin(115200);
  Serial3.begin(19200);

  Serial.println("Starting...");
  power_on();

  delay(3000);

  do{
  
    sendATcommand("AT+FSCD=C:/Email/", "OK", 2000);
  
    
    while(Serial3.available() > 0) Serial3.read();   
  
      // Send the AT command 
      Serial.println("AT+FSLS");
      Serial3.println("AT+FSLS");
      previous = millis();
      answer = 0;
      
      // this loop reads the first folder
      j = 0;
      do{
        if(Serial3.available() != 0){    
          aux_str[j] = Serial3.read();
          Serial.print(aux_str[j]);
          j++;
          if (strstr(aux_str, "ES: ") != NULL)    
          {
            answer = 1;
          }
        }
      }
      while ((answer == 0) && ((millis() - previous) < 10000));
      previous = millis();
      answer = 0;
      j = 0;
      memset (aux_str, '\0',(sizeof(aux_str)/sizeof(aux_str[0])));
        do{
          if(Serial3.available() != 0){    
              aux_str[j] = Serial3.read();
              if ((aux_str[j] == '\n') && (j > 2))
                {
                  aux_str[j] = '\0';
                  answer = 1;
                  Serial.println(aux_str);
                }
              if ((aux_str[j] == '\n') && (j <= 2))
                {
                  j=-1;
                }
              if ((aux_str[j] != ' ') && (answer == 0))
              {
                j++;  
              }
            }
        }
        while ((answer == 0) && ((millis() - previous) < 5000));
  
    sprintf(command,"AT+FSCD=C:/Email/%s", aux_str);
    sendATcommand(command, "OK", 2000);
    sendATcommand("AT+FSDEL=*.*", "OK", 2000);
    sendATcommand("AT+FSCD=C:/Email/", "OK", 2000);
    sprintf(command,"AT+FSRMDIR=%s", aux_str);
    answer_fin=sendATcommand(command, "OK", 2000);
  }
  while(answer_fin == 1);
  Serial.println ("Fin de borrado de Carpetas");
}


void loop() {
  // put your main code here, to run repeatedly:

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
  char response[1000];
  unsigned long previous;

  memset(response, '\0', 1000);    // Initialize the string

  delay(1000);

  while(Serial3.available() > 0) Serial3.read();    // Clean the input buffer

  //Serial.println(ATcommand);
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
