bool sensors_read(void) 
{
  int dist_min = 5;//2cm datasheet
  int dist_max = 650;//700cm datasheet
                    //15° detection angle 
  bool near = false;
  long duration;
  int timeoutsensors = 50000; //us son 0.05s (Vsonido=342.2m/s, 7*2=14m en 0.041s)
  for (int i=0; i<4; i++){
    digitalWrite(sensor_trig[i], LOW);  //para generar un pulso limpio ponemos a LOW 4us
    delayMicroseconds(4);
    digitalWrite(sensor_trig[i], HIGH);  //generamos Trigger (disparo) de 10us
    delayMicroseconds(10);
    digitalWrite(sensor_trig[i], LOW);
  
    duration = pulseIn(sensor_echo[i], HIGH, timeoutsensors);  //medimos el tiempo entre pulsos, en microsegundos
  
    sensor_dist[i] = duration * 10 / 292/ 2;   //convertimos a distancia (cm)
    if (sensor_dist[i] > dist_min & sensor_dist[i] < dist_max){
      near = true;
      sensor_act[i] = true;
    }else{
      sensor_act[i] = false;
    }
  }
  return near;
}
