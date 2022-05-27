bool change_chpt(void){
  int detour = 5;// desviacion en metros
  CPchanged = false;
  if (s!=0){
    newchpt_h_1 = h_max;
    newchpt_h_2 = h_max;
    newchpt_lon_1 = chpt_lon[2] + sqrt(sq(D.a - 40) + sq(D.b - 30))*h_n*0.011*cos(orientation - atan((D.a - 40)/(D.b - 30))) * 360 / (cos(chpt_lat[2])*40075017);
    newchpt_lat_1 = chpt_lat[2] + sqrt(sq(D.a - 40) + sq(D.b - 30))*h_n*0.011*sin(orientation - atan((D.a - 40)/(D.b - 30))) * 360 / 40007161;
    CPchanged = true;
  }
  if(near){
    if(sensor_dist[1] < dist_max){
      if (h_n < h_max){    //la primera opción es subir, si lo permite el rango.
        newchpt_h_1 = h_max;
        newchpt_h_2 = h_max;
        newchpt_lon_1 = chpt_lon[2];
        newchpt_lon_2 = chpt_lon[3];
        newchpt_lat_1 = chpt_lat[2];
        newchpt_lat_2 = chpt_lat[3];
        CPchanged = true;
      }else if (sensor_dist[2] > dist_max){    //la segunda es moverse a la derecha.
        newchpt_h_1 = h_n;
        newchpt_h_2 = h_n;
        newchpt_lon_1 = chpt_lon[2] + detour*cos(orientation) * 360 / (cos(chpt_lat[2])*40075017);
        newchpt_lon_2 = chpt_lon[3] + detour*cos(orientation) * 360 / (cos(chpt_lat[2])*40075017);
        newchpt_lat_1 = chpt_lat[2] + detour*sin(orientation) * 360 / 40007161;
        newchpt_lat_2 = chpt_lat[3] + detour*sin(orientation) * 360 / 40007161;
        CPchanged = true;
      }else if (sensor_dist[3] > dist_max){    //la tercera es moverse a la izquierda.
        newchpt_h_1 = h_n;
        newchpt_h_2 = h_n;
        newchpt_lon_1 = chpt_lon[2] - detour*cos(orientation) * 360 / (cos(chpt_lat[2])*40075017);
        newchpt_lon_2 = chpt_lon[3] - detour*cos(orientation) * 360 / (cos(chpt_lat[2])*40075017);
        newchpt_lat_1 = chpt_lat[2] - detour*sin(orientation) * 360 / 40007161;
        newchpt_lat_2 = chpt_lat[3] - detour*sin(orientation) * 360 / 40007161;
        CPchanged = true;
      }

    }else if(sensor_dist[2] < 50){
      if (sensor_dist[3] > 50){   
        newchpt_h_1 = h_n;
        newchpt_h_2 = h_n;
        newchpt_lon_1 = ;
        newchpt_lon_2 = ;
        newchpt_lat_1 = ;
        newchpt_lat_2 = ;
        CPchanged = true;
      }
    }else if(sensor_dist[3] < 50){
        newchpt_h_1 = h_n;
        newchpt_h_2 = h_n;
        newchpt_lon_1 = ;
        newchpt_lon_2 = ;
        newchpt_lat_1 = ;
        newchpt_lat_2 = ;
        CPchanged = true;
    }
  }
  return CPchanged;
}


