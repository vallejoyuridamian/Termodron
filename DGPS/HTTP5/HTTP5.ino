
//Change here your data

#define SERIAL_BUFFER_SIZE 256


const char pin_number[] = "4725";
const char apn[] = "prepago.ancel";
const char user_name[] = "bam";
const char password[] = "bam";

char url[ ]="201.217.132.178";
int port= 2101;
char request[ ]="GET /UYMO HTTP/1.1\r\nUser-Agent: Termodron\r\nAccept: */*\r\nConnection: close\r\nAuthorization: Basic dGVybW9kcm9uOnRlcm1vZHJvbjE=\r\n\r\n"; //

//termodron:termodron1 = dGVybW9kcm9uOnRlcm1vZHJvbjE=

int timeoutglobal=20000;
int8_t answer;
int onModulePin = 8, aux;
int data_size = 0;
int end_file = 0;
char aux_str[100];
char aux_char;
byte data[2000];
int http_x;
int http_status; // 0: end, 1 waiting data with header, 2 waiting data without header, 3 timeout waiting data, 4 unknow response
int x = 0;
long previous;
long espera,counter; 

void setup(){

  
  memset(data, '\0',2000);
  
  pinMode(onModulePin, OUTPUT);
  Serial.begin(230400);
  Serial3.begin(19200);

  //Serial.print("SERIAL_BUFFER_SIZE: ");

  //Serial.println(SERIAL_BUFFER_SIZE);

  Serial.print("Serial3 available: ");
  Serial.println(Serial3.available());
  
  Serial.println("Starting...");
  power_on();
  
  //sets the PIN code
  sprintf(aux_str, "AT+CPIN=%s", pin_number);
  sendATcommand(aux_str, "OK", 2000);
  
  while( (sendATcommand("AT+CREG?", "+CREG: 0,1", 500) || sendATcommand("AT+CREG?", "+CREG: 0,5", 500)) == 0 );
  
  // sets APN, user name and password
  sprintf(aux_str, "AT+CGSOCKCONT=1,\"IP\",\"%s\"", apn);
  sendATcommand(aux_str, "OK", 2000);
  
  sprintf(aux_str, "AT+CSOCKAUTH=1,1,\"%s\",\"%s\"", user_name, password);
  sendATcommand(aux_str, "OK", 2000);
  
  //sendATcommand("AT+CMMSSYSSET=102400","OK",2000);

  //sendATcommand("AT+IPREX=19200", "", 2000);

  
  sendATcommand("AT+NETCLOSE", "Network closed", 2000);
  
  sendATcommand("AT+NETOPEN=\"TCP\",2101", "Network opened", 20000);
  
  // request the url
  
  sprintf(aux_str, "AT+CHTTPACT=\"%s\",%d", url, port);
  sendATcommand(aux_str, "+CHTTPACT: REQUEST", 25000);

  Serial3.write(request);
  Serial.println(request);
  Serial3.write(0x1A);
}
  
void loop(){

  counter=millis();
  memset(data, '\0',2000);
  memset(aux_str,'\0',100);
  aux_char='\0';
  
  Serial.print("esto llego despues: ");
  while(Serial3.available()!=0){
  Serial.print(Serial3.read(),HEX); // limpia lo que haya pendiente
  }
  Serial.println();
  //sendATcommand("", "+CHTTPACT:", 10000);
  previous=millis();
  espera=millis();
  while((Serial3.available()==0)&&(millis()-previous <timeoutglobal));// nos quedamos esperando datos del serial
  if(millis()-previous > timeoutglobal){
    Serial.println("No hay mas mensajes");  
  }
  espera=millis()-espera;
  aux_str[0] = Serial3.read(); // Se supone que es la D
  if (aux_str[0]!= 'D'){
  Serial.print("Waiting for the D, got: ");
  }
  previous=millis();
  while ((aux_str[0]!='D')&&(millis()-previous <timeoutglobal)){
    if(Serial3.available()>0){
      aux_str[0]=Serial3.read();
      Serial.print(aux_str[0]);
    }
  }
  if(millis()-previous > timeoutglobal){
    Serial.println("Aint got no D *sadface*");  
  }

  previous=millis();
  while((Serial3.available() ==0)&&(millis()-previous<timeoutglobal));
  if(Serial3.available()>0){
    aux_char=Serial3.read();
    Serial.print(aux_char);  // A
  }
  previous=millis();
  while((Serial3.available() ==0)&&(millis()-previous<timeoutglobal));
  if(Serial3.available()>0){
    aux_char=Serial3.read();
    Serial.print(aux_char); // T
  }
  previous=millis();
  while((Serial3.available() ==0)&&(millis()-previous<timeoutglobal));
  if(Serial3.available()>0){
    aux_char=Serial3.read();
    Serial.print(aux_char); // A
  }
  previous=millis();
  while((Serial3.available() ==0)&&(millis()-previous<timeoutglobal));
  if(Serial3.available()>0){
    aux_char=Serial3.read(); //,
    Serial.print(aux_char);
  }
  Serial.println();
  // Reads the packet size
  x=0;
  do{
    previous=millis();
    while((Serial3.available()==0)&&(millis()-previous <timeoutglobal));
    if(millis()-previous >timeoutglobal){
    Serial.println("Timeout leyendo el packet size");
    }
    aux_str[x] = Serial3.read();
    x++;
  }while((aux_str[x-1] != '\r') && (aux_str[x-1] != '\n'));

  previous=millis();
  while((Serial3.available() ==0)&&(millis()-previous<timeoutglobal));
  if(Serial3.available()>0){
  Serial3.read();  // \n
  }
  
  aux_str[x-1] = '\0';
  data_size = atoi(aux_str);
  Serial.print("data_size: ");
  Serial.println(data_size);
  http_x = 0;

  Serial.print("Serial available principio mensaje: ");
  Serial.println(Serial3.available());
  Serial.println();
  previous=millis();
  while((data_size > 0)&&(millis()-previous < timeoutglobal)){
    if(Serial3.available() > 0){
      
    /* if (Serial3.available() > 10){
      Serial.println();
      Serial.print("Serial3 available mayor a 10 leyendo: ");
      Serial.println(Serial3.available());

      }*/
      
      
      //Serial.print(Serial3.read(),HEX);
      //aux_char='\0';
      
     // aux_char = Serial3.read(); // aca esta el payload
      
     // Serial.print(aux_char,HEX);
      
      data[http_x] = Serial3.read(); // aca esta el payload
      
      //Serial.print(data[http_x],HEX);
      http_x++;
      data_size--;
    }
    else
      delay(1);
  }

  for (int k=0;k<http_x+1;k++){
    if(data[k]<16){
     Serial.print("0");
    } 
    Serial.print(data[k],HEX);
  }

  
  Serial.println();
  Serial.print("data_size fin loop: ");
  Serial.println(data_size);
  Serial.print("Serial available fin loop: ");
  Serial.println(Serial3.available());
  Serial.println();
  counter=millis()-counter;
  Serial.print("El loop duro: ");
  Serial.print(counter);
  Serial.print(" milisegundos, de los cuales ");
  Serial.print(espera);
  Serial.println(" estuvimos esperando un mensaje");
  

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
 
 // Serial.print("Serial3 available principio AT command: ");
 // Serial.println(Serial3.available());
  
  //while ( Serial3.available() > 0) Serial3.read();   // Clean the input buffer

  //Serial.print("Serial3 available buffer limpio: ");
 // Serial.println(Serial3.available());
  
  
  Serial.println(ATcommand);
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
     if (Serial3.available() > 60){
      
      Serial.print("Serial3 available mayor a 60 leyendo: ");
      Serial.println(Serial3.available());

      }
   
     
      // check if the desired answer is in the response of the module
      if (strstr(response, expected_answer1) != NULL){
        answer = 1;
        Serial.println();
      }
      //if(strstr(response, "+CHTTPACT:")== NULL){ // si no es un chttpact lo imprimo aca
      //  Serial.print(response[x]);
      //}          
    }
    // Waits for the asnwer with time out
  }while ((answer == 0) && ((millis() - previous) < timeout));
  //Serial.print("Serial3 available fin AT command: ");
  //Serial.println(Serial3.available());
  
  return answer;
}
