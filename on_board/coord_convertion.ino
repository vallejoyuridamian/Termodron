///////////////////////////////////UTM to WGS84///////////////////////////////////

void utm_wgs84 (double x, double y, int huso, double* lat, double* lon){
 //   - Longitud al Oeste del meridiano de Greenwich, entonces longitud es negativa (-).
 //   - Longitud al Este del meridiano de Greenwich, entonces longitud es positiva (+).
 //   - Latitud al Sur del meridiano de Greenwich, entonces longitud es negativa (-).
 //   - Latitud al Norte del meridiano de Greenwich, entonces longitud es positiva (+).

  // double lat = 0, lon = 0;

  double radtierra = 6366197.724;
  double facesc = 0.9996;

  // Sobre la geometría del elipsoide WGS84
  double a = 6378137.0;
  double b = 6356752.3142;
  
 //  float e = sqrt((a*a) - (b*b))/a; // Excentricidad.
  double e = sqrt((a*a) - (b*b))/b; // Segunda excentricidad.
  double e2 = e * e; // Segunda excentricidad al cuadrado.
  double c = a*a / b; // Radio Polar de Curvatura.

  // Tratamiento De X e Y.  
  double X = x - 500000; // Eliminacion del retraqueo.
  double Y = y - 10000000; // Eliminacion del retraqueo al sur del Ecuador.
   
  // Sobre el huso.
  double lam0 = huso * 6 - 183; // Meridiano central del huso en radianes.

  //Ecuaciones de Coticchia-Surace para el paso de Geográficas a UTM;
  double fi = Y / (radtierra * facesc);
  double cosfi = cos (fi);
  double senfi = sin(fi);
  double cosfi2 = cosfi * cosfi;
  double v = c * facesc / (sqrt(1 + e2 * cosfi2)) ;
  double A = X / v;
  double A1 = sin (2 * fi);
  double A2 = A1 * cosfi2;
  double J2 = fi + (A1 / 2.0);
  double J4 = (3.0 * J2 + A2) / 4.0;
  double J6 = (5.0 * J4 + A2 * cosfi2) / 3.0;
  double alf = (3.0 / 4.0) * e2;
  double bet = (5.0 / 3.0) * alf * alf;
  double gam = (35.0 / 27.0) * alf * alf * alf;

  double Bo = c * facesc * (fi - (alf * J2) + (bet * J4) - (gam * J6));
  
  double B = (Y - Bo) / v;
  double sed = e2 * A * A * cosfi2 / 2.0;
  double eps = A * (1 - (sed / 3.0));
  double eta = B * (1 - sed) + fi;
  double senheps = (exp(eps) - exp(-eps)) / 2.0;
  double Dlam = atan(senheps / cos(eta));
  double tau = atan((cos(Dlam)) * tan(eta));

  *lon = (Dlam * 180 / PI) + lam0;
  *lat = (fi + (1 + (e2 * cosfi2) - (3.0 * e2 * senfi * cosfi * (tau - fi) / 2.0)) * (tau - fi)) * 180 / PI;  
  
  #ifdef debugging
  sermon.println ("Coordenadas WGS84 actuales: ");
  sermon.print (" lat = ");
  sermon.print (*lat,7);
  sermon.println (" (°)");
  sermon.print (" lon = ");
  sermon.print (*lon,7); 
  sermon.println (" (°)");
  #endif
}



///////////////////////////////////WGS84 to UTM///////////////////////////////////

void wgs84_utm (double lat, double lon, double* x, double*y){
  //   - Longitud al Oeste del meridiano de Greenwich, entonces longitud es negativa (-).
  //   - Longitud al Este del meridiano de Greenwich, entonces longitud es positiva (+).
  //   - Latitud al Sur del meridiano de Greenwich, entonces longitud es negativa (-).
  //   - Latitud al Norte del meridiano de Greenwich, entonces longitud es positiva (+).
  
  //double x = 0, y = 0;

  // Sobre la geometría del elipsoide WGS84
  double a = 6378137.0;
  double b = 6356752.3142;  
  // float e = sqrt((a*a) - (b*b))/a; // Excentricidad.
  double e = sqrt((a*a) - (b*b))/b; // Segunda excentricidad.
  double e2 = e * e; // Segunda excentricidad al cuadrado.
  double c = a*a / b; // Radio Polar de Curvatura.

  // Sobre la longitud y latitud. Conversión de grados decimales a radianes.  
  double latRad = lat * PI / 180.0; // Latitud en Radianes.
  double lonRad = lon * PI / 180.0; // Longitud en Radianes.
   
  // Sobre el huso.
  float h = (lon / 6) + 31;
  int huso = int(h);
  int lam0 = huso * 6 - 183; // Meridiano central del huso en radianes.
  double Dlam = lonRad - (lam0 * PI / 180.0);  // Desplazamiento del punto respecto al meridiano central del huso.

  //Ecuaciones de Coticchia-Surace para el paso de Geográficas a UTM;
  double coslatRad = cos (latRad);
  double coslatRad2 = coslatRad * coslatRad;
  double A = coslatRad * sin (Dlam);
  double xi = 0.5 * log ((1 + A) / (1 - A));
  double n = atan(tan(latRad) / cos(Dlam)) - latRad;
  double v = (c / sqrt(1 + e2 * coslatRad2)) * 0.9996;
  double z = (e2/ 2.0) * xi * xi * coslatRad2;
  double A1 = sin (2 * latRad);
  double A2 = A1 * coslatRad2;
  double J2 = latRad + (A1 / 2.0);
  double J4 = (3.0 * J2 + A2) / 4.0;
  double J6 = (5.0 * J4 + A2 * coslatRad2) / 3.0;
  double alf = 0.75 * e2;
  double bet = (5.0 / 3.0) * alf * alf;
  double gam = (35.0 / 27.0) * alf * alf * alf;
  double Bfi = 0.9996 * c * (latRad - alf * J2 + bet * J4 - gam * J6);

  *x = xi * v * (1 + (z / 3.0)) + 500000; // 500.000 es el retranqueo de cada huso sobre el origen de coordenadas en el eje X para que no existan coordenadas negativas. 
  *y = n * v * (1 + z) + Bfi + 10000000;  // Para latitudes al sur del ecuador, se suma 10.000.000 al valor de Y para evitar coordenadas negativas.
  /*
  #ifdef debugging
  sermon.println (" Coordenadas UTM actuales: ");
  sermon.print (" X = ");
  sermon.print (*x,5);
  sermon.println (" (m)");
  sermon.print (" Y = ");
  sermon.print (*y,5); 
  sermon.println (" (m)");
  #endif
  */
}


