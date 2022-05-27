bool donecapturing = 0;
int frame_id;
bool sync_OK = false;

void take_picture(void){
  //SPI.beginTransaction(csFLIR, SPIFLIRsett);
  while (!sync_OK){
    frame_id = 0;
    sermon.println("sync");
    lepton_sync();
    //delay(50);
    sync_OK = true;
    while(donecapturing == false && sync_OK){
      read_lepton_frame();
      buffer_image();
    }
  }
  //SPI.endTransaction();
  donecapturing = false;
  #ifdef SD_present
    SD_picture_save();
  #endif
  datasend_image();
}



void lepton_sync(void){
  int i;
  int data = 0x0f;

  digitalWriteDirect(csFLIR, HIGH);
  delay(185);
  
  while (data & 0x0f == 0x0f){
    //SPI.beginTransaction(SPIFLIRsett);
    digitalWriteDirect(csFLIR, LOW);
    lepton_frame_packet[0] = SPI.transfer(csFLIR, 0x00, SPI_CONTINUE);
    lepton_frame_packet[1] = SPI.transfer(csFLIR, 0x00, SPI_LAST);
    data = lepton_frame_packet[0] << 8;
    data |= lepton_frame_packet[1];
    digitalWriteDirect(csFLIR, HIGH);
        
    for (i = 1; i < ((VOSPI_FRAME_SIZE) / 2); i++){
      digitalWriteDirect(csFLIR, LOW);
      lepton_frame_packet[2 * i] = SPI.transfer(csFLIR, 0x00, SPI_CONTINUE);
      lepton_frame_packet[2 * i + 1] = SPI.transfer(csFLIR, 0x00, SPI_LAST);
      digitalWriteDirect(csFLIR, HIGH);
    }
  }
  buffer_image();
  //SPI.endTransaction();
}


 void read_lepton_frame(void){
  int i;
  //SPI.beginTransaction(SPIFLIRsett);
  
  for (i = 0; i < (VOSPI_FRAME_SIZE / 2); i++){
    digitalWriteDirect(csFLIR, LOW);
    //  send in the address and value via SPI:
    lepton_frame_packet[2 * i] = SPI.transfer(csFLIR, 0x00, SPI_CONTINUE);
    lepton_frame_packet[2 * i + 1] = SPI.transfer(csFLIR, 0x00, SPI_LAST);
    // take the SS pin high to de-select the chip:
    digitalWriteDirect(csFLIR, HIGH);
  }
  //SPI.endTransaction();
}

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
  sermon.print(i);
  if (i == 0x3B) {
    donecapturing = true;
    sermon.print("\n");
  }
  if (i != frame_id){
    sync_OK = false;
  }
  frame_id++;
}
