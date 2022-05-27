bool donecapturing = false;
int frame_id;
bool sync_OK;

//Función que saca una foto
void take_picture(void){
  int retry_sync = 3;
  sync_OK = false;
  //SPI.beginTransaction(csFLIR, SPIFLIRsett);
  while (!sync_OK ){
    if(retry_sync == 0){
      error_sync();
    }
    frame_id = 0;
    #ifdef debugging
    sermon.print("sync");
    sermon.print("\t");
    #endif
    lepton_sync();
    //delay(50);
    sync_OK = true;
    while(donecapturing == false && sync_OK){
      read_lepton_frame();
      buffer_image();
    }
    retry_sync--;
  }
  //SPI.endTransaction();
  #ifdef debugging
  sermon.println("OK");
  #endif
  donecapturing = false;
}


//Función para sincronizar los paquetes de datos provenientes de la Flir Lepton vía SPI
void lepton_sync(void){
  int i;
  int data = 0x0f;

  digital_write_direct(csFLIR, HIGH);
  delay(185);
  
  while (data & 0x0f == 0x0f){
    //SPI.beginTransaction(SPIFLIRsett);
    digital_write_direct(csFLIR, LOW);
    lepton_frame_packet[0] = SPI.transfer(csFLIR, 0x00, SPI_CONTINUE);
    lepton_frame_packet[1] = SPI.transfer(csFLIR, 0x00, SPI_LAST);
    data = lepton_frame_packet[0] << 8;
    data |= lepton_frame_packet[1];
    digital_write_direct(csFLIR, HIGH);
        
    for (i = 1; i < ((VOSPI_FRAME_SIZE) / 2); i++){
      digital_write_direct(csFLIR, LOW);
      lepton_frame_packet[2 * i] = SPI.transfer(csFLIR, 0x00, SPI_CONTINUE);
      lepton_frame_packet[2 * i + 1] = SPI.transfer(csFLIR, 0x00, SPI_LAST);
      digital_write_direct(csFLIR, HIGH);
    }
  }
  buffer_image();
  //SPI.endTransaction();
}


//Función que se encarga de leer un frame de datos de la Flir Lepton
void read_lepton_frame(void){
  int i;
  //SPI.beginTransaction(SPIFLIRsett);
    
  for (i = 0; i < (VOSPI_FRAME_SIZE / 2); i++){
    digital_write_direct(csFLIR, LOW);
    //  send in the address and value via SPI:
    lepton_frame_packet[2 * i] = SPI.transfer(csFLIR, 0x00, SPI_CONTINUE);
    lepton_frame_packet[2 * i + 1] = SPI.transfer(csFLIR, 0x00, SPI_LAST);
    // take the SS pin high to de-select the chip:
    digital_write_direct(csFLIR, HIGH);
  }
  //SPI.endTransaction();
}


//Función que almacena en memoria los frames recibidos de la Flir Lepton
void buffer_image(void){
  //command to get from the lecture the ID of the frame
  int i = lepton_frame_packet[1];
  int j;
  if(i <= 0x3B){
    for (j = image_y-1; j > -1; j--){
      //assign to the variable image[i][j] the data read from the lecture, starts assign from 4th and 5th positions due to the from [0]to[3] are the ID and the CRC
      //image[i][image_y-j-1] = ((lepton_frame_packet[2 * j + 4] << 8) + lepton_frame_packet[2 * j + 5]); //-0x1000
      image[i][j] = ((lepton_frame_packet[2 * j + 4] << 8) + lepton_frame_packet[2 * j + 5]);    
    }
  }
  if (i == 0x3B) {
    donecapturing = true;
    /*#ifdef debugging
    sermon.print("\n");
    #endif*/
  }
  if (i != frame_id){
    sync_OK = false;
  }
  frame_id++;
}


//Función para poner los pines en nivel alto o bajo de forma más rápida
inline void digital_write_direct(int pin, boolean val) {
  if (val) {
    g_APinDescription[pin].pPort -> PIO_SODR = g_APinDescription[pin].ulPin;
  }else{
    g_APinDescription[pin].pPort -> PIO_CODR = g_APinDescription[pin].ulPin;
  }
}
