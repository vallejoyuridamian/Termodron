int calcularRojo(int temp){

  if (temp < tmin){
    return 0;
  }
  
  if (temp > tp){
  return 255;
  }
  
 return int(255*temp/(tp-tmin)-255*tp/(tp-tmin)+255);

} 

int calcularVerde(int temp){
  
  if (temp < tp){
  return 255;
  }
  
  if (temp > tmax){
  return 0;
  } 

  return int(255*temp/(tp-tmax)-255*tp/(tp-tmax)+255);
} 

int calcularAzul(int temp){
 
 return 0;
 
}

void PrepararImagen(){

  // BM
  ImagenBMP[0]=0x42; // "B"
  ImagenBMP[1]=0x4D; // "M"
  // Tamanio del arhivo en bytes (little endian)
  ImagenBMP[2]=0x76; // Siempre va a pesar 14454 bytes por como esta armada
  ImagenBMP[3]=0x38; 
  ImagenBMP[4]=0x00; 
  ImagenBMP[5]=0x00; 
  // Cuatro bytes reservados, los ponemos en 0
  ImagenBMP[6]=0x00; 
  ImagenBMP[7]=0x00; 
  ImagenBMP[8]=0x00; 
  ImagenBMP[9]=0x00; 
  // Cuatro bytes de offset, siempre va a ser 54
  ImagenBMP[10]=0x36; 
  ImagenBMP[11]=0x00; 
  ImagenBMP[12]=0x00; 
  ImagenBMP[13]=0x00;
  //////Header DIB////////
  // Cantidad de bytes en el DIB Header, siempre va a ser 40 
  ImagenBMP[14]=0x28; 
  ImagenBMP[15]=0x00; 
  ImagenBMP[16]=0x00; 
  ImagenBMP[17]=0x00;
  // Cuenta horizontal de Pixels (80)
  ImagenBMP[18]=0x50; //50 
  ImagenBMP[19]=0x00; 
  ImagenBMP[20]=0x00; 
  ImagenBMP[21]=0x00;
  // Cuenta vertical de Pixels (60)
  ImagenBMP[22]=0x3C; //3C
  ImagenBMP[23]=0x00; 
  ImagenBMP[24]=0x00; 
  ImagenBMP[25]=0x00;
  // Cantidad de planos de color (siempre es 1)
  ImagenBMP[26]=0x01; 
  ImagenBMP[27]=0x00;
  // Cantidad de bits por pixel (24)
  ImagenBMP[28]=0x18; 
  ImagenBMP[29]=0x00;
  // Metodo de compresion, BI_RGB es 0, no compresion
  ImagenBMP[30]=0x00; 
  ImagenBMP[31]=0x00;
  ImagenBMP[32]=0x00; 
  ImagenBMP[33]=0x00;
  // Tamanio en bytes del mapa de bits, siempre va a ser 14400 (hay que paddear las filas a multiplos de 4, no importa aca porque justo 60 es 
  ImagenBMP[34]=0x40; 
  ImagenBMP[35]=0x38;
  ImagenBMP[36]=0x00; 
  ImagenBMP[37]=0x00;
  // Resolucion de impresion horizontal, no importa, lo ponemos en 2835
  ImagenBMP[38]=0x13; 
  ImagenBMP[39]=0x0B;
  ImagenBMP[40]=0x00; 
  ImagenBMP[41]=0x00;
  // Idem para la vertical 
  ImagenBMP[42]=0x13; 
  ImagenBMP[43]=0x0B;
  ImagenBMP[44]=0x00; 
  ImagenBMP[45]=0x00;
  // Colores en la paleta, ponemos 0
  ImagenBMP[46]=0x00; 
  ImagenBMP[47]=0x00;
  ImagenBMP[48]=0x00; 
  ImagenBMP[49]=0x00;
  // Colores importantes, ponemos 0
  ImagenBMP[50]=0x00; 
  ImagenBMP[51]=0x00;
  ImagenBMP[52]=0x00; 
  ImagenBMP[53]=0x00;
}


void RAWThermalToCelsius(){

  for (int i = 0;i<image_x;i++){
    for (int k = 0;k<image_y;k++){
      Celsius_thermal_image[i][k]=0.053*(thermal_image[i][k]-8192)+25;  
    // Temperature T = slope * (raw - 8192) + ambTemp
    // According to Max Ritter's work, the slope he calculated is 0.0217. I recorded the temperature of the difference between raw data and 8192 and the temperature measured by the MLX90614 to apply a polyfit using Matlab and I found the slope I got was 0.026. The result might vary when the environment temperature changes. 
    // https://groups.google.com/forum/#!topic/flir-lepton/T2NFRkeMyR4
    }
  }
}

void CargarImagenABMP(){

  cursorRawThermal_fila=image_x-1;
  cursorRawThermal_columna=0;
    
  for (int i = 54;i<ImagenBMPSize;i++){
   
    // calculo la temperatura 
    temp=Celsius_thermal_image[cursorRawThermal_fila][cursorRawThermal_columna];
    //acomodo para la proxima vuelta
    cursorRawThermal_columna++;
    if (cursorRawThermal_columna == image_y){
      cursorRawThermal_columna=0;  
      cursorRawThermal_fila--;    
    }
    
    ImagenBMP[i]=calcularAzul(temp);
    ImagenBMP[i+1]=calcularVerde(temp);
    ImagenBMP[i+2]=calcularRojo(temp);
    i=i+2;  
  }
}

void ActualizarImagen(){
  Consola.println("IMAGEN ACTUALIZADA");
  PrepararImagen();
  RAWThermalToCelsius();
  CargarImagenABMP();
  flg_Img=true;
  flg_Img_available=true;
  flg_has_att=true;
  enviarReportes.enable();
  
}
