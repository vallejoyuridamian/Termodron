void take_picture(void){     
  lepton_sync();
  delay(50);
  while(donecapturing == false){
    read_lepton_frame();
    buffer_image();
  } 
}

void buffer_image(void){
  //command to get from the lecture the ID of the frame
  int i = lepton_frame_packet[1];
  int j;
  
  for (j = image_y-1; j > -1; j--){
    //assign to the variable image[i][j] the data read from the lecture, starts assign from 4th and 5th positions due to the from [0]to[3] are the ID and the CRC
    //image[i][image_y-j-1] = ((lepton_frame_packet[2 * j + 4] << 8) + lepton_frame_packet[2 * j + 5]); //-0x1000
    image[i][j] = ((lepton_frame_packet[2 * j + 4] << 8) + lepton_frame_packet[2 * j + 5]);
  }
  
  if (i == 0x3B) {
    donecapturing = true;
  }
}

//Deberá manejar errores de la cámara y reiniciarla si es necesario
