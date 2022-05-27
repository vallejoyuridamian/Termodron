void utmwgs84 (double x, double y, int huso){
 //   - Longitud al Oeste del meridiano de Greenwich, entonces longitud es negativa (-).
 //   - Longitud al Este del meridiano de Greenwich, entonces longitud es positiva (+).
 //   - Latitud al Sur del meridiano de Greenwich, entonces longitud es negativa (-).
 //   - Latitud al Norte del meridiano de Greenwich, entonces longitud es positiva (+).

  double lat = 0, lon = 0;

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

  lon = (Dlam * 180 / PI) + lam0;
  lat = (fi + (1 + (e2 * cosfi2) - (3.0 * e2 * senfi * cosfi * (tau - fi) / 2.0)) * (tau - fi)) * 180 / PI;  

  Serial.println (" Coordenadas WGS84 actuales: ");
  Serial.print (" lat = ");
  Serial.print (lat,5);
  Serial.println (" (°)");
  Serial.print (" lon = ");
  Serial.print (lon,5); 
  Serial.println (" (°)");

}

